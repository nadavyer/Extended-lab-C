#include <stdio.h>
#include <limits.h>
#include <unistd.h>

#include <string.h>
#include <stdlib.h>
#include <wait.h>
#include <linux/limits.h>

int main(int argc, char *argv[]) {

    int pip1[2];
    
    __pid_t child1_pid;
    int child1_status;
    __pid_t child2_pid;
    int child2_status;

    char *ls[3] = {"ls", "-l", NULL};
    char *tail[4] = {"tail", "-n", "2", NULL};

    
    pipe(pip1);
    child1_pid = fork();

    if (child1_pid == 0) {
        close(STDOUT_FILENO);
        dup2(pip1[1],STDOUT_FILENO);
        close(pip1[1]);
        execvp(ls[0], ls);
        _exit(0);
    }
    else {
     close(pip1[1]);
    }
   
    child2_pid = fork();
    
    if (child2_pid == 0) {
        close(STDIN_FILENO);
        dup2(pip1[0],STDIN_FILENO);
        close(pip1[0]);
        execvp(tail[0], tail);
        _exit(0);
    }
    else {
        close(pip1[1]);
    }
    waitpid(child1_pid,&child1_status,0);
    waitpid(child2_pid,&child2_status,0);

    return 0;
}
