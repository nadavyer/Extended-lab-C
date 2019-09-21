#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <linux/limits.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include "LineParser.h"

int SIZE_OF_LIST;
int firstIter = 1;
typedef struct cmdHistoryNode {
    struct cmdHistoryNode *next;
    struct cmdHistoryNode *prev;
    struct cmdLine *cmd;
} cmdHistoryNode;

cmdHistoryNode *new_node(cmdLine *cmd) {
    cmdHistoryNode *p_history_node = (cmdHistoryNode *) malloc(sizeof(cmdHistoryNode));
    p_history_node->next = NULL;
    p_history_node->prev = NULL;
    p_history_node->cmd = copy_cmd_line(cmd);

    return p_history_node;
}

void free_node(cmdHistoryNode *history_node) {
   /* cmdHistoryNode *next_node = history_node->next;*/
    freeCmdLines(history_node->cmd);
    free(history_node);
}

cmdHistoryNode *addCmdToHistory(cmdHistoryNode *head, cmdLine *cmd) {
    cmdHistoryNode *newHead = new_node(cmd);
    head->prev = newHead;
    newHead->next = head;
    SIZE_OF_LIST++;
    return newHead;
}


void free_list(cmdHistoryNode *historyNode) {

    cmdHistoryNode *curr = historyNode;
    cmdHistoryNode *tmp;
    while (curr) {
        tmp = curr;
        curr = curr->next;
        free_node(tmp);
    }
}


void handleTilda(cmdLine *cmdLine);

char str_replace(char *string, char *pattern, char *replacement);

void handleHistory(cmdLine *cmdLine, cmdHistoryNode *head);

void build_history_str(cmdHistoryNode *node, char buf[]);

void printHistory(cmdHistoryNode *head);

int show_history(cmdHistoryNode *head);

cmdHistoryNode *deleteFromHistory(cmdLine *cmd, cmdHistoryNode *head);


void handleTilda(cmdLine *cmd) {
    char tmp[1024];
    char *home = getenv("HOME");
    int i;

    for (i = 0; i < cmd->argCount; i++) {
        memset(tmp, '\0', strlen(tmp)); /*fill tmp with the home path*/
        char *cur_arg = cmd->arguments[i];
        strcpy(tmp, cur_arg);

        if (str_replace(tmp, "~", home))
            replaceCmdArg(cmd, i, tmp);
    }
}

cmdHistoryNode *deleteFromHistory(cmdLine *cmd, cmdHistoryNode *head) {
    long int stopIndx = strtol(cmd->arguments[2], NULL, SIZE_OF_LIST)+1;
    cmdHistoryNode *curr = head;
    cmdHistoryNode *tmp;
    int i = 0;
    while (i < (SIZE_OF_LIST - stopIndx) && curr !=NULL) {
        curr = curr->next;
        i++;
    }
    if (curr->next == NULL) {
        tmp = curr;
        curr = curr->prev;
        curr->next = NULL;
        while (i > 0 && curr !=NULL) {
            curr = curr->prev;
            i--;
        }
    }
    else if (curr->prev == NULL) {
        tmp = curr;
        curr = curr->next;
        tmp->next->prev = NULL;
    }
    else {
        tmp = curr->prev;
        curr->prev->next = curr->next;
        curr->next->prev = curr->prev;
        curr = tmp;
    }
    SIZE_OF_LIST--;
    return curr;
    }





void handleHistory(cmdLine *cmdLine, cmdHistoryNode *head) {
    if ((cmdLine->argCount > 2) && (strcmp("history", cmdLine->arguments[0]) == 0) &&  (strcmp("-d", cmdLine->arguments[1]) == 0)) {
        head = deleteFromHistory(cmdLine, head);
        if (head)show_history(head);
    } else if (strcmp("history", cmdLine->arguments[0]) == 0){
        show_history(head);
    }
}



int show_history(cmdHistoryNode *head) {
    int i = 0;
    char buf[1024];

    cmdHistoryNode *cur_node = head;
    while (cur_node->next) {
        cur_node = cur_node->next;
    }
    while (cur_node) {
        build_history_str(cur_node, buf);
        printf("%d. %s", i++, buf);
        cur_node = cur_node->prev;
    }
    return 0;
}

void build_history_str(cmdHistoryNode *node, char buf[]) {
    memset(buf, '\0', 1024);

    int i;
    for (i = 0; i < node->cmd->argCount; i++) {
        strcat(buf, node->cmd->arguments[i]);
        strcat(buf, " ");
    }
    strcat(buf, "\n");
}


char str_replace(char *string, char *pattern, char *replacement) {
    char *p = strstr(string, pattern); /*ret the 1st pattern in sting*/
    if (!p)
        return 0;

    char buf[1024];
    memset(buf, '\0', strlen(buf));

    if (string == p) /* pattern was found @start of string */
    {
        strcpy(buf, replacement);
        strcat(buf, p + strlen(pattern));
    } else {
        strncpy(buf, string, strlen(string) - strlen(p));
        strcat(buf, replacement);
        strcat(buf, p + strlen(pattern));
    }

    memset(string, '\0', strlen(string));
    strcpy(string, buf);
    return 1;
}

int execute(cmdLine *pCmdLine) {
    pid_t child_pid;
    int child_status;
    child_pid = fork();

    if (child_pid == 0) {
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

int main(int argc, char **argv) {
    SIZE_OF_LIST = 1;
    int isQuit = -1;

    cmdLine *command;
    cmdHistoryNode *head = NULL;
    char path[PATH_MAX];
    char mystring[2048];

    while (isQuit != 0) {
        getcwd(path, sizeof(path));
        printf("%s>", path);
        fgets(mystring, 2048, stdin);
        command = parseCmdLines(mystring);

        if (firstIter == 1) {
            head = new_node(command);
            handleTilda(command);
            firstIter++;
            isQuit = strcmp(command->arguments[0], "quit");
            if (strcmp(command->arguments[0], "cd") == 0) {
                if (chdir(command->arguments[1])) {
                    perror("cd");
                    _exit(EXIT_FAILURE);
                }
            } else if (strcmp(command->arguments[0], "history") == 0) {

                handleHistory(command, head);

            } else if (isQuit != 0) {
                execute(command);
            }
        } else {

            handleTilda(command);
            isQuit = strcmp(command->arguments[0], "quit");
            if (strcmp(command->arguments[0], "cd") == 0) {
                head = addCmdToHistory(head, command);
                if (chdir(command->arguments[1])) {
                    _exit(EXIT_FAILURE);
                }
            } else if (strcmp(command->arguments[0], "history") == 0) {
                head = addCmdToHistory(head, command);
                handleHistory(command, head);
            } else if (isQuit != 0) {
                head = addCmdToHistory(head, command);
                execute(command);

            }
        }
    }


    free_list(head);
    return 0;
}



