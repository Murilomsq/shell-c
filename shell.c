#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <limits.h>

#define MAX_COMMANDS 5
#define MAX_COMMAND_CHARACTERS 50


void cat(char* input, char* output);
void parse_arguments(char* input, char** commands, int* numOfCommands);
void evaluate_command(char** commands, char* command, int numOfCommands);
void command_handler(char** commands, int numOfCommands);
void cat(char* input, char* output);
void print_file_contents(char* filename);
void echo(char* input, char* output);
void call_func_by_string(char* functionName, char* input, char* output);
void ls(char* input, char* output);
void cd(char* input, char* output);
void pwd();
void cp(char* input, char* output);



void parse_arguments(char* input, char** commands, int* numOfCommands) {
    char* token;
    int i = 0;

    token = strtok(input, " ");
    while (token != NULL) {
        *numOfCommands = *numOfCommands + 1;
        commands[i++] = token;
        token = strtok(NULL, " ");
    }
}

void evaluate_command(char** commands, char* command, int numOfCommands){

    //CAT COMMAND
    if(strcmp(command, "cat") == 0){

        //NO ARGUMENTS
        if(numOfCommands == 1){
            cat("stdin", "stdout");
            return;
        }   
        // ONE ARGUMENT : DISPLAY FILE
        else if(numOfCommands == 2){
            //NORMAL FILE NAME
            print_file_contents(commands[1]);
        }

        // TWO ARGUMENTS
        else if(numOfCommands == 3){
            if(strcmp(commands[1], ">") == 0){      // cat > "filename.txt"
                cat("stdin", commands[2]);
            }
            else{                                   //cat "name" "name"
                printf("Incorrect Format\n");  
                return;
            }
        }
    }
    //ECHO COMMAND
    else if(strcmp(command, "echo") == 0){
        //NO ARGUMENTS
        if(numOfCommands == 1){
            printf("Wrong usage\n");
            return;
        } 
        if(numOfCommands == 2){
            echo(commands[1], NULL);
        }   
    }
    // LS COMMAND
    else if(strcmp(command, "ls") == 0){
        if(numOfCommands == 1){
            ls(".", NULL);
            return;
        } 
        if(numOfCommands == 2){
            ls(commands[1], NULL);
        } 
    }
    // CD COMMAND

    else if(strcmp(command, "cd") == 0){
        if(numOfCommands == 1){
            printf("Wrong usage\n");
            return;
        } 
        if(numOfCommands == 2){
            cd(commands[1], NULL);
        } 
    }
    else if(strcmp(command, "pwd") == 0){
        pwd();
    }

    else if(strcmp(command, "cp") == 0){
        if(numOfCommands != 3){return;}
        cp(commands[1], commands[2]);
    }

    else{
        printf("Command not found\n");
    }
}

void cat(char* input, char* output){

    char buffer[1024];

    if(strcmp(input, "stdin") == 0 && strcmp(output, "stdout") == 0){
        
        while(1){
            scanf("%s", buffer);
            printf("%s\n", buffer);
        }
        return; //unreachable
    }

    if(strcmp(input, "stdin") == 0 && strcmp(output, "stdout") != 0){
        //MODIFY STD OUTPUT
        FILE* file = fopen(output, "w");
        if(file == NULL){
            printf("Error: Could not open file\n");
            return;
        }

        while (fgets(buffer, sizeof(buffer), stdin) != NULL) {
            fputs(buffer, file);
            fflush(file);
        }
        fclose(file);
        return;
    }
}

void echo(char* input, char* output){
    if(output == NULL){
        printf("%s\n", input);
    }
}

void ls(char* input, char* output){
    DIR *mydir;
    struct dirent *myfile;
    struct stat mystat;

    char buf[512];
    mydir = opendir(input);
    while((myfile = readdir(mydir)) != NULL)
    {
        sprintf(buf, "%s/%s", input, myfile->d_name);
        stat(buf, &mystat);
        printf(" %s\n", myfile->d_name);
    }
    closedir(mydir);
}

void cd(char* input, char* output){
    chdir(input);
}

void pwd(){
    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("\n%s\n\n", cwd);
    }
}

void cp(char* input, char* output){
    FILE *fptr1, *fptr2;
    char filename[100], c;
  
    // Open one file for reading
    fptr1 = fopen(input, "r");
    if (fptr1 == NULL)
    {
        printf("Cannot open file %s \n", input);
        exit(0);
    }
  
    // Open another file for writing
    fptr2 = fopen(output, "w");
    if (fptr2 == NULL)
    {
        printf("Cannot open file %s \n", output);
        exit(0);
    }
  
    // Read contents from file
    c = fgetc(fptr1);
    while (c != EOF)
    {
        fputc(c, fptr2);
        c = fgetc(fptr1);
    }
  
    fclose(fptr1);
    fclose(fptr2);
    return;
}

void print_file_contents(char* filename){
    FILE *fptr = fopen(filename, "r");

    if(fptr == NULL){
        printf("Error: Could not open file\n");
        return;
    }

    char buffer[1024];

    while(fgets(buffer, sizeof(buffer), fptr) != NULL){
        printf("%s", buffer);
    }
    fclose(fptr);
}

int main(){

    char command[MAX_COMMAND_CHARACTERS];
    char* commandParser[MAX_COMMANDS];
    int numberOfCommands = 0;

    int status; 
    pid_t pid; 

    char cwd[PATH_MAX];

    while(1){
        numberOfCommands = 0;
        if (getcwd(cwd, sizeof(cwd)) != NULL) {
           printf("%s", cwd);
        }
        printf(" $ ");
        scanf(" %20[^\n]", command); //Prevents overflow
        parse_arguments(command, commandParser, &numberOfCommands);
        evaluate_command(commandParser, commandParser[0], numberOfCommands);

        /*pid = fork(); // fork a new process
        if (pid < 0) {
            exit(1);
        }
        if (pid == 0) { // child process
            evaluate_command(commandParser, commandParser[0], numberOfCommands);
            exit(1);
        } else { // parent process
            waitpid(pid, &status, 0);
        }*/
        
    }

    
    return 0;
}