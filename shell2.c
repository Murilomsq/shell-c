#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define MAX_LENGTH 1024 // maximum length of command

int main() {
    char command[MAX_LENGTH]; // command entered by user
    char *args[MAX_LENGTH]; // arguments for the command
    int status; // status of child process
    pid_t pid; // process ID

    while (1) {
        printf("$ "); // shell prompt
        fgets(command, MAX_LENGTH, stdin); // read command from user
        command[strcspn(command, "\n")] = 0; // remove newline character
        if (strcmp(command, "exit") == 0) { // exit if command is "exit"
            exit(0);
        }
        // parse command into arguments
        char *arg = strtok(command, " ");
        int i = 0;
        while (arg != NULL) {
            args[i++] = arg;
            arg = strtok(NULL, " ");
        }
        args[i] = NULL;
        pid = fork(); // fork a new process
        if (pid < 0) {
            perror("fork");
            exit(1);
        }
        if (pid == 0) { // child process
            execvp(args[0], args);
            perror("execvp");
            exit(1);
        } else { // parent process
            waitpid(pid, &status, 0);
        }
    }
    return 0;
}