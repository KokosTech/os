#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>

typedef struct array_t {
    char **data;
    size_t size;
} array_t;

char *readliney_optimized(int fd) {
    char c;
    char *res = (char *)malloc(1 * sizeof(char));

    size_t len = 0;
    while (1 == read(fd, &c, 1)) {
        if (c == '\n') {
            res[len] = '\0';
            break;
        }
        // write(1, &c, 1);
        res[len++] = c;
        res = (char *)realloc(res, (len + 1) * sizeof(char));
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

    if(pid == 0) {
        execvp(args->data[0], args->data);
        //perror("execvp");
        return 1;
    } else if(pid > 0) {
        waitpid(pid, NULL, 0);
    } else {
        perror("fork");
        return 1;
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <file>\n", argv[0]);
        return 1;
    }

    int fd = open(argv[1], O_RDONLY);

    char *line = readliney_optimized(fd);
    array_t *tokens = tokenizer(line);

    for (int i = 0; tokens->data[i]; ++i) printf("%s\n", tokens->data[i]);

    run_command(tokens);

    line = readliney_optimized(fd);
    tokens = tokenizer(line);

    for (int i = 0; tokens->data[i]; ++i) printf("%s\n", tokens->data[i]);

    run_command(tokens);

    return 0;
}