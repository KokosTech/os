#include <err.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <sys/wait.h>
#include <unistd.h>

typedef struct array_t {
    char **data;
    size_t size;
} array_t;

char *readliney_optimized(int fd) {
    ssize_t readStatus;
    char c;

    char *res = (char *)malloc(1 * sizeof(char));
    if (res == NULL) {
        return NULL;
    }

    size_t len = 0;
    while (1 == (readStatus = read(fd, &c, 1))) {
        if (c == '\n') {
            res[len] = '\0';
            break;
        }

        res[len++] = c;
        res = (char *)realloc(res, (len + 1) * sizeof(char));
        if (res == NULL) {
            return NULL;
        }
    }
    if (readStatus == -1) {
        free(res);
        return NULL;
    }

    if (len == 0) {
        free(res);
        return "EOF";
    }

    return res;
}

array_t *tokenizer(char *str) {
    array_t *tokens = (array_t *)malloc(sizeof(array_t));
    if (tokens == NULL) {
        return NULL;
    }

    int i = 0;
    char *token = NULL;
    while (token = strtok(str, " ")) {
        tokens->data = (char **)realloc(tokens->data, sizeof(char *) * (i + 1));
        if (tokens->data == NULL) {
            return NULL;
        }
        tokens->data[i] = malloc(strlen(token) + 1);
        if (tokens->data[i] == NULL) {
            return NULL;
        }
        strcpy(tokens->data[i], token);
        tokens->data[i++][strlen(token)] = '\0';
        tokens->size = i;
        str = NULL;
    }

    tokens->data = (char **)realloc(tokens->data, sizeof(char *) * (i + 1));
    if (tokens->data == NULL) {
        return NULL;
    }
    tokens->data[i] = NULL;
    tokens->size = i;

    return tokens;
}

int run_command(array_t *args) {
    pid_t pid = fork();

    if (pid == 0) {
        int exec_stat = execvp(args->data[0], args->data);

        if (exec_stat == -1) {
            return -1;
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
        return -1;
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <file>\n", argv[0]);
        return 1;
    }

    int fd = open(argv[1], O_RDONLY);
    if (fd == -1) {
        err(1, "open failed");
    }

    char *line = readliney_optimized(fd);
    if (line == NULL) {
        close(fd);
        err(1, "readliney_optimized failed");
    }

    while (line != NULL && strcmp(line, "EOF") != 0) {
        array_t *tokens = tokenizer(line);
        free(line);

        if (tokens == NULL) {
            close(fd);
            free(line);
            err(1, "tokenizer failed");
        }

        if (run_command(tokens) == -1) {
            int i = 0;
            while (tokens->data[i] != NULL) {
                free(tokens->data[i++]);
            }
            free(tokens->data);
            free(tokens);
            close(fd);
            err(1, "run_command failed");
        }

        line = readliney_optimized(fd);
        if (line == NULL) {
            int i = 0;
            while (tokens->data[i] != NULL) {
                free(tokens->data[i++]);
            }
            free(tokens->data);
            free(tokens);
            close(fd);
            err(1, "readliney_optimized failed");
        }

        int i = 0;
        while (tokens->data[i] != NULL) {
            free(tokens->data[i++]);
        }
        free(tokens->data);
        free(tokens);

        if (strcmp(line, "EOF") != 0) {
            free(line);
        }
    }

    close(fd);

    return 0;
}