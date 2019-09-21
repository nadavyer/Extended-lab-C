#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <linux/limits.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include "LineParser.h"

void handleTilda(cmdLine *pLine);
char str_replace(char *string, char *pattern, char *replacement);

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
            waitpid(child_pid, &child_status, 0);
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
        handleTilda(command);
        isQuit = strcmp(command->arguments[0], "quit");
        if (strcmp(command->arguments[0], "cd") == 0) {
            if (chdir(command->arguments[1])) {
                _exit(EXIT_FAILURE);
            }
        } else if (isQuit != 0) {
            execute(command);
        }
        freeCmdLines(command);
    }
    return 0;
}

void handleTilda(cmdLine *cmd) {
    char tmp[1024];
    char *home = getenv("HOME");
    int i;

    for (i = 0; i < cmd->argCount; i++)
    {
        memset(tmp, '\0', strlen(tmp)); /*fill tmp with the home path*/
        char *cur_arg = cmd->arguments[i];
        strcpy(tmp, cur_arg);

        if (str_replace(tmp, "~", home))
            replaceCmdArg(cmd, i, tmp);
    }
}

char str_replace(char *string, char *pattern, char *replacement)
{
    char *p = strstr(string, pattern);
    if (!p)
        return 0;

    char buf[1024];
    memset(buf, '\0', strlen(buf));

    if (string == p) /* pattern was found @start of string */
    {
        strcpy(buf, replacement);
        strcat(buf, p + strlen(pattern));
    }
    else
    {
        strncpy(buf, string, strlen(string) - strlen(p));
        strcat(buf, replacement);
        strcat(buf, p + strlen(pattern));
    }

    memset(string, '\0', strlen(string));
    strcpy(string, buf);
    return 1;
}

