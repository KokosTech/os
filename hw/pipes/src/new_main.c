#include <err.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define PROCS 4
#define IO 2

int main() {
    printf("Parent started\n");

    int pipes[PROCS - 1][IO];
    pid_t procs[PROCS];
    char *cmds[PROCS][6] = {
        {"cut", "-d", ":", "-f7", "/etc/passwd", NULL},
        {"sort", NULL},
        {"uniq", NULL},
        {"wc", "-l", NULL},
    };

    for (int i = 0; i < PROCS - 1; i++) {
        printf("Creating pipe %d\n", i);

        if (pipe(pipes[i]) < 0) errx(1, "[MAIN][PARENT] Pipe creation failed");
    }

    for (int i = 0; i < PROCS; ++i) {
        if ((procs[i] = fork()) < 0) errx(1, "[MAIN][PARENT] Fork failed");
        if (procs[i] == 0) {
            if (i != 0) {
                if (dup2(pipes[i - 1][0], 0) < 0)
                    errx(1, "[MAIN][CHILD %d] Dup2 failed", i);
                close(pipes[i - 1][0]);
                close(pipes[i - 1][1]);
            }

            if (i != PROCS - 1) {
                if (dup2(pipes[i][1], 1) < 0)
                    errx(1, "[MAIN][CHILD %d] Dup2 failed", i);
                close(pipes[i][0]);
                close(pipes[i][1]);
            }

            if (execvp(cmds[i][0], cmds[i]) < 0)
                errx(1, "[MAIN][CHILD %d] Exec failed", i);

            exit(0);  // useless
        } else {
            if (i != 0) {
                close(pipes[i - 1][0]);
                close(pipes[i - 1][1]);
            }
        }
    }

    for (int i = 0; i < PROCS; ++i) {
        int status;
        if (waitpid(procs[i], &status, 0) < 0)
            errx(1, "[MAIN][PARENT] Wait failed");
        if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
            errx(1, "[MAIN][PARENT] Child %d exited with status %d", i,
                 WEXITSTATUS(status));

        printf("Child %d exited with status %d\n", i, WEXITSTATUS(status));
    }

    return 0;
}