
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <err.h>

#define CHILDS 4

int dup2Wrapper(int oldfd, int newfd) {
    int result;

    if (-1 == (result = dup2(oldfd, newfd))) {
        errx(3, "%s", "An error occurred while trying to use 'dup2()'");
    }

    return result;
}

void execvpWrapper(const char* file, char* const argv[]) {
    if (-1 == execvp(file, argv)) {
        errx(4, "%s", "An error occurred while trying to use 'execvp()'");
    }
}

//char* inbuf = "a";

int main() {

    int pipes[CHILDS - 1][2];

    for (size_t i = 0; i < CHILDS - 1; ++i) {
        if (-1 == pipe(pipes[i])) {
            errx(1, "%s", "An error occurred while trying to use 'pipe()'");
        }
    }

    char* commands[CHILDS][6] = {
        {"cut", "-d:", "-f7", "/etc/passwd", NULL},
        {"sort", NULL},
        {"uniq", NULL},
        {"wc", "-l", NULL}
    };

    pid_t childs[CHILDS];

    for (size_t i = 0; i < CHILDS; ++i) {
        if (-1 == (childs[i] = fork())) {
            errx(2, "%s", "An error occurred while trying to use 'fork()'");
        } else if (0 == childs[i]) {
            if (0 == i) {
                close(pipes[i][0]);
                
                dup2Wrapper(pipes[i][1], STDOUT_FILENO);
                execvpWrapper(commands[i][0], commands[i]);

                close(pipes[i][1]);
            } else if (CHILDS - 2 == i) {
                close(pipes[i - 1][1]);
                //sleep(3);
                dup2Wrapper(pipes[i - 1][0], STDIN_FILENO);
                execvpWrapper(commands[i][0], commands[i]);

                close(pipes[i - 1][0]);                
            } else {
                close(pipes[i - 1][1]);
                close(pipes[i][0]);

                dup2Wrapper(pipes[i - 1][0], STDIN_FILENO);
                dup2Wrapper(pipes[i][1], STDOUT_FILENO);
                execvpWrapper(commands[i][0], commands[i]);

                close(pipes[i - 1][0]);
                close(pipes[i][1]);
            }
        } else {
            if (CHILDS - 1 != i) {
                close(pipes[i][1]);
            }
            
            if (0 != i) {
                close(pipes[i - 1][0]);
            }
        }
    }

    int exitStatus;

    if (-1 == waitpid(childs[CHILDS - 1], &exitStatus, 0)) {
        errx(5, "%s", "An error occurred while trying to use 'waitpid()'");
    }
    
    printf("Last child process exited with status %d\n", WEXITSTATUS(exitStatus));

    return 0;
}
