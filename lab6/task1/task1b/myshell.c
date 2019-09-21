#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <linux/limits.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include "LineParser.h"

int execute(cmdLine *pCmdLine){
    pid_t child_pid;
    int child_status;
    child_pid = fork();
    
    if (child_pid==0) {
        execvp(pCmdLine->arguments[0], pCmdLine->arguments);
        perror("execvp");
        _exit(EXIT_FAILURE); 
    } else {
        if (pCmdLine->blocking == 1) {
            waitpid(-1, &child_status, 0);
        }
        return child_status;
    }
}
 
int main(int argc, char ** argv) {
    int isQuit = -1;
    while(isQuit != 0) {
        char path[PATH_MAX];
        getcwd(path, sizeof(path));
        printf("%s>", path);
        char mystring [2048];
        fgets (mystring , 2048 , stdin);
        cmdLine* command;
        command = parseCmdLines(mystring);
        isQuit = strcmp(command->arguments[0], "quit");
        if (isQuit != 0) {
            execute(command);
        }
        freeCmdLines(command);
    }
    return 0;
}
