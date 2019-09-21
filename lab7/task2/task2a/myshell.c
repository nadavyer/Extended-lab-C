
#include <stdio.h>
#include <limits.h>
#include <unistd.h>
#include "LineParser.h"
#include <string.h>
#include <stdlib.h>
#include <wait.h>
#include <linux/limits.h>
#include <fcntl.h>


int SIZE_OF_HISTORY;

typedef struct Node Node;

struct Node {
    char *string;
    Node *next;
};


int execute_command(cmdLine *command);

void execute_cd_command(cmdLine *command);

void change_home_dir(cmdLine *pLine, char *homeString, char *tilda);

char *str_replace(char *orig, char *rep, char *with);

void execute_history_command(cmdLine *cmd, Node *list_history_head, int *reached_list_cap);

Node *add_command_to_history(char cmd_str[2048], Node *list_history_head, int MAX_HISTORY_CAP, int *reached_list_cap);

char *check_exclemation_mark(char *string, Node *list_history_head);

void free_node(Node *node, int free_next);

int **createPipes(int nPipes);

void releasePipes(int **pipes, int nPipes);

int *leftPipe(int **pipes, cmdLine *pCmdLine);

int *rightPipe(int **pipes, cmdLine *pCmdLine);

void execute_piped_commands(cmdLine *cmd);


int main() {
    SIZE_OF_HISTORY = 0;
    int quit = 0;
    char working_Directory[PATH_MAX];
    char cmd_buffer[2048];
    char *home = getenv("HOME");
    Node *list_history_head = NULL;
    int MAX_HISTORY_CAP = 10;
    int *reached_list_cap = (int *) malloc(sizeof(int));

    *reached_list_cap = 0;
    while (!quit) {
        getcwd(working_Directory, sizeof(working_Directory));
        printf("%s>", working_Directory);

        fgets(cmd_buffer, 2048, stdin);

        cmdLine *command;
        command = parseCmdLines(cmd_buffer);

        if (command != NULL) {
            if (!strcmp(command->arguments[0], "quit")) {
                quit = 1;
            } else {
                change_home_dir(command, home, "~");

                char *cmd_string = check_exclemation_mark(command->arguments[0], list_history_head);

                if (!strcmp(command->arguments[0], cmd_string)) {
                    cmd_string = strdup(cmd_buffer);
                } else {
                    freeCmdLines(command);
                    command = parseCmdLines(cmd_string);
                }

                list_history_head = add_command_to_history(cmd_string, list_history_head, MAX_HISTORY_CAP,
                                                           reached_list_cap);

                if (!strcmp(command->arguments[0], "cd")) {
                    execute_cd_command(command);
                } else if (!strcmp(command->arguments[0], "history")) {
                    execute_history_command(command, list_history_head, reached_list_cap);
                } else {
                    execute_command(command);
                }

                quit = 0;
            }
        }

        freeCmdLines(command);
    }

    free_node(list_history_head, 1);
    free(reached_list_cap);
    return 0;
}


int **createPipes(int nPipes) {
    int **pipes = malloc(nPipes * sizeof(int *));
    int i;
    for (i = 0; i < nPipes; i++) {
        pipes[i] = malloc(2 * sizeof(int));
        if (pipe(pipes[i]) == -1) {
            perror("creating pipe error");
            _exit(EXIT_FAILURE);
        }
    }
    return pipes;
}

void releasePipes(int **pipes, int nPipes) {
    int i;
    for (i = 0; i < nPipes; i++) {
        free(pipes[i]);
    }
    free(pipes);
}


int *leftPipe(int **pipes, cmdLine *pCmdLine) {
    if (pCmdLine->idx == 0) {
        return NULL;
    }
    return pipes[(pCmdLine->idx) - 1];
}

int *rightPipe(int **pipes, cmdLine *pCmdLine) {
    if (pCmdLine->next == NULL) {
        return NULL;
    }
    return pipes[(pCmdLine->idx)];
}

void execute_piped_commands(cmdLine *cmd){
    
}



char *check_exclemation_mark(char *string, Node *list_history_head) {
    if (string[0] == '!') {
        char *stringNumToExec = string + 1;
        int numToExec = atoi(stringNumToExec);

        int count = 0;
        Node *curr = list_history_head;

        while (count != numToExec && curr != NULL) {

            count++;
            curr = curr->next;
        }
        if (count != numToExec) {
            fprintf(stderr, "command %d does not exist", numToExec);
        } else {
            string = strdup(curr->string);
        }
    }

    return string;

}

Node *add_command_to_history(char cmd_str[2048], Node *list_history_head, int MAX_HISTORY_CAP, int *reached_list_cap) {
    if (SIZE_OF_HISTORY <= 8) {
        SIZE_OF_HISTORY++;
    }
    Node *newHistoryNode = (Node *) malloc(sizeof(Node));
    newHistoryNode->next = NULL;
    newHistoryNode->string = cmd_str;
    (*reached_list_cap)++;

    if (list_history_head == NULL) {
        return newHistoryNode;
    }

    int count = 0;
    Node *curr = list_history_head;
    Node *prev = NULL;

    while (curr != NULL) {
        prev = curr;
        curr = curr->next;
        count++;
    }

    prev->next = newHistoryNode;


    if (count == MAX_HISTORY_CAP) {
        curr = list_history_head;
        list_history_head = list_history_head->next;
        free_node(curr, 0);

        *reached_list_cap = 1;
    }

    return list_history_head;

}

void free_node(Node *node, int free_next) {
    if (node == NULL) {
        return;
    }

    if (free_next) {
        free_node(node->next, free_next);
    }

    node->next = NULL;
    free(node->string);
    free(node);
}

void execute_history_command(cmdLine *cmd, Node *list_history_head, int *reached_list_cap) {
    if (cmd->arguments[1] != NULL && !strcmp(cmd->arguments[1], "-d")) {

        int numToCut = atoi(cmd->arguments[2]);
        if (numToCut > SIZE_OF_HISTORY - 1) {
            fprintf(stderr, "%s %d %s\n", "index", numToCut, "is out of history's list bounds (max is 10).");
            return;
        }
        if (numToCut)
            if (*reached_list_cap) {
                numToCut--;
                *reached_list_cap = 0;
            }


        if (numToCut == 0) {
            Node *curr = list_history_head;
            *list_history_head = *list_history_head->next;
            free_node(curr, 0);
        } else if (numToCut != -1) {
            int count = 0;
            Node *curr = list_history_head;
            Node *prev = NULL;

            while (count != numToCut && curr != NULL) {

                count++;
                prev = curr;
                curr = curr->next;
            }

            if (curr != NULL) {
                prev->next = curr->next;
                free_node(curr, 0);
            }
        }
    }

    int count = 0;
    Node *curr = list_history_head;
    while (curr != NULL) {
        fprintf(stdout, "%d. %s", count, curr->string);

        count++;
        curr = curr->next;
    }

}

void change_home_dir(cmdLine *pLine, char *homeString, char *tilda) {
    int count = 0;
    while (pLine->arguments[count] != NULL) {
        char *newArg = str_replace(pLine->arguments[count], tilda, homeString);
        replaceCmdArg(pLine, count, newArg);

        free(newArg);
        count++;
    }
}

void execute_cd_command(cmdLine *command) {
    if (chdir(command->arguments[1])) {
        perror(command->arguments[1]);
    }
}

int execute_command(cmdLine *command) {
    int child_status;
    pid_t child_Pid = fork();

    if (child_Pid == 0) {
        if (command->outputRedirect != NULL) {
            close(STDOUT_FILENO);
            int file_descriptor = open(command->outputRedirect, O_WRONLY);
            dup2(file_descriptor, STDOUT_FILENO);
            close(file_descriptor);
        }
        if (command->inputRedirect != NULL) {
            close(STDIN_FILENO);
            int file_descriptor = open(command->inputRedirect, O_RDONLY);
            dup2(file_descriptor, STDIN_FILENO);
            close(file_descriptor);
        }

        if (command) {
            execvp(command->arguments[0], command->arguments);
        }
        perror("Error");
        exit(EXIT_FAILURE);
    } else if (command->blocking == 1) {
        waitpid(child_Pid, &child_status, 0);

    }
    return child_status;
}

char *str_replace(char *orig, char *rep, char *with) {
    char *result; /* the return string*/
    char *ins;    /* the next insert point*/
    char *tmp;    /* varies*/
    size_t len_rep;  /* length of rep (the string to remove)*/
    size_t len_with; /* length of with (the string to replace rep with)*/
    size_t len_front; /* distance between rep and end of last rep*/
    int count;    /* number of replacements*/

    /* sanity checks and initialization*/
    if (!orig || !rep)
        return NULL;

    len_rep = strlen(rep);

    if (len_rep == 0)
        return NULL; /* empty rep causes infinite loop during count*/

    if (!with)
        with = "";

    len_with = strlen(with);

    /* count the number of replacements needed*/
    ins = orig;
    tmp = strstr(ins, rep);
    for (count = 0; tmp; ++count) {
        ins = tmp + len_rep;
        tmp = strstr(ins, rep);
    }

    tmp = result = malloc(strlen(orig) + (len_with - len_rep) * count + 1);

    if (!result)
        return NULL;

    /* first time through the loop, all the variable are set correctly*/
    /* from here on,*/
/*    tmp points to the end of the result string*/
    /*    ins points to the next occurrence of rep in orig*/
    /*    orig points to the remainder of orig after "end of rep"*/
    while (count--) {
        ins = strstr(orig, rep);
        len_front = ins - orig;
        tmp = strncpy(tmp, orig, len_front) + len_front;
        tmp = strcpy(tmp, with) + len_with;
        orig += len_front + len_rep; /* move to next "end of rep"**/
    }
    strcpy(tmp, orig);
    return result;
}



