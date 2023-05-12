#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

#define PIPES 4
#define IO 2

int main() {
    printf("Parent started\n");
    int pipes[PIPES][IO];

    for (int i = 0; i < PIPES; i++) {
        printf("Creating pipe %d\n", i);
        pipe(pipes[i]);
    }

    for (int i = 0; i < PIPES; i++) {
        if (fork() == 0) {
            printf("Child %d started\n", i);
            dup2(pipes[i][0], 0);
            close(pipes[i][0]);
            close(pipes[i][1]);

            if (i != PIPES - 1) {
                dup2(pipes[i + 1][1], 1);
                close(pipes[i + 1][0]);
                close(pipes[i + 1][1]);
            }

            if (i == 0) {
                char *arr[] = {"cut", "-d",          ":",         "-f",
                               "7",   "/etc/passwd", (char *)NULL};
                               write(2, "Command: ", 9);
                // write(2, arr[0], strlen(arr[0]));
                // write(2, "\n", 1);
                execvp(arr[0], arr);
            } else if (i == 1) {
                // read from stdin - pipe

                // write(2, "Command: ", 9);
                // write(2, "sort", strlen("sort"));
                // write(2, "\n", 1);

                // char c = 0;
                // while (read(0, &c, 1) > 0) {
                    // write(2, &c, 1);
                // }

                execlp("sort", "sort", NULL);
            } else if (i == 2) {
                // write(2, "Command: ", 9);
                // write(2, "uniq", strlen("uniq"));
                // write(2, "\n", 1);
                execlp("uniq", "uniq", NULL);
            } else if (i == 3) {
                // write(2, "Command: ", 9);
                // write(2, "wc", strlen("wc"));
                // write(2, "\n", 1);
                execlp("wc", "wc", "-l", NULL);
            }
        } else {
            close(pipes[i][0]);
            close(pipes[i][1]);
        }
    }
    for (int i = 0; i < PIPES; i++) {
        printf("Waiting for child %d to finish\n", i);
        wait(NULL);
    }

    return 0;
}