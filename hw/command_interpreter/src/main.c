#include <err.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <sys/wait.h>
#include <unistd.h>

#include "utils/file.h"
#include "utils/hashmap.h"
// #include "utils/pgc.h"

hash_table_t *MEMORY;

typedef struct array_t {
    char **data;
    size_t size;
} array_t;

char *readliney_optimized(int fd) {
    char c;
    char *res = (char *)hmalloc(MEMORY, "res", 1 * sizeof(char));

    size_t len = 0;
    while (1 == eread(fd, &c, 1)) {
        if (c == '\n') {
            res[len] = '\0';
            break;
        }

        res[len++] = c;
        res = (char *)hrealloc(MEMORY, "res", (len + 1) * sizeof(char));
    }

    if (len == 0) {
        printf("WTF\n");
        hfree(MEMORY, "res");
        return NULL;
    }

    return res;
}

array_t *tokenizer(char *str) {
    array_t *tokens = (array_t *)malloc(sizeof(array_t));

    int i = 0;
    char *token = NULL;
    while (token = strtok(str, " ")) {
        tokens->data = (char **)realloc(tokens->data, sizeof(char *) * (i + 1));
        tokens->data[i++] = token;
        tokens->size = i;
        str = NULL;
    }

    tokens->data = (char **)realloc(tokens->data, sizeof(char *) * (i + 1));
    tokens->data[i] = NULL;
    tokens->size = i;

    return tokens;
}

int run_command(array_t *args) {
    pid_t pid = fork();

    if (pid == 0) {
        int exec_stat = execvp(args->data[0], args->data);

        if (exec_stat == -1) {
            destroy_table(MEMORY);
            errx(1, "execvp failed");
        }

        return 1;
    } else if (pid > 0) {
        int status;
        waitpid(pid, &status, 0);

        WIFEXITED(status);
        if (WEXITSTATUS(status) != 0) {
            printf("Command %s failed with status %d :(\n", args->data[0],
                   WEXITSTATUS(status));
        }

    } else {
        destroy_table(MEMORY);
        errx(1, "fork failed");
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <file>\n", argv[0]);
        return 1;
    }

    MEMORY = init(100);

    int fd = eopen(argv[1], O_RDONLY);

    char *line = readliney_optimized(fd);
    printf("line: %s\n", line);
    while (line != NULL) {
        break;
        char **tokens = tokenizer(line);
        run_command(tokens);
        break;

        hfree(MEMORY, "res");
        break;
        line = readliney_optimized(fd);
    }

    //destroy_table(MEMORY);

    return 0;
}