#include <err.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define COMMANDS 4

#include <stdint.h>
#include <string.h>

uint32_t countTokens(char* line) {
    uint32_t count = 0;
    int len = strlen(line);
    for (int i = 0; i < len; ++i) {
        if (line[i] == ' ' && ((i + 1) < len && line[i + 1] != ' ')) {
            count++;
        }
    }

    return ++count;
}

void getTokenSizes(char* line, uint32_t* tokenSizes) {
    int tokenIndex = 0;
    int len = strlen(line);
    for (int i = 0; i < len; i++) {
        if (line[i] == ' ') {
            if ((i + 1) < len && line[i + 1] != ' ') {
                tokenIndex++;
            }
        } else {
            tokenSizes[tokenIndex]++;
        }
    }
}

// if something fails in this function, it will return NULL
// this is checked in main.c, and if it is NULL, the program exits with a code
// of 20
char** tokenizer(char* line) {
    if (line == NULL) {
        return NULL;
    }

    uint32_t count = countTokens(line);
    uint32_t* sizes = calloc(count, sizeof(int));
    if (sizes == NULL) {
        return NULL;
    }
    getTokenSizes(line, sizes);

    char** tokens = malloc(sizeof(char*) * (count + 1));
    if (tokens == NULL) {
        free(sizes);
        return NULL;
    }
    tokens[count] = NULL;
    for (int i = 0; i < count; i++) {
        tokens[i] = malloc(sizeof(char) * (sizes[i] + 1));
        if (tokens[i] == NULL) {
            for (int j = 0; j < i; j++) {
                free(tokens[j]);
            }
            free(tokens);
            free(sizes);
            return NULL;
        }
    }

    free(sizes);

    int tokenIndex = 0;
    int tokenCharIndex = 0;
    int len = strlen(line);
    for (int i = 0; i < len; ++i) {
        if (line[i] == ' ') {
            if ((i + 1) < len && line[i + 1] != ' ') {
                tokens[tokenIndex][tokenCharIndex] = '\0';
                tokenIndex++;
                tokenCharIndex = 0;
            }
        } else {
            tokens[tokenIndex][tokenCharIndex++] = line[i];
        }
    }
    tokens[tokenIndex][tokenCharIndex] = '\0';

    return tokens;
}

void freeTokens(char** tokens) {
    char** tokensCopy = tokens;
    while (*tokensCopy != NULL) {
        free(*tokensCopy);
        tokensCopy++;
    }
    free(tokens);
}

void printTokens(char** tokens) {
    char** tokensCopy = tokens;
    while (*tokensCopy != NULL) {
        printf("%s\n", *tokensCopy);
        tokensCopy++;
    }
}

int main() {
    char* commands[COMMANDS] = {"cut", "sort", "uniq", "wc"};

    char* arguments[COMMANDS] = {"cut -d : -f 7 /etc/passwd", "sort", "uniq",
                                 "wc -l"};

    // maybe have 4 -> one for the main process
    int pipeArr[4][2];
    for (int i = 0; i < 4; i++) {
        if (-1 == pipe(pipeArr[i])) {
            errx(8, "Error with pipe()");
        }
    }

    write(2, "Pipes created\n", 14);

    pid_t children[COMMANDS];
    // what is the third parameter of waitpid?
    // answer: it is the options parameter
    // what is the options parameter
    // answer: it is a bit mask
    for (int i = 0; i < COMMANDS; i++) {
        if (-1 == (children[i] = fork())) {
            errx(9, "Error with fork");
        }

        if (children[i] == 0) {
            // fprintf(stderr, "dup2: %d\n ", i);
            if (i > 1) {
                close(pipeArr[i - 1][1]);
                dup2(pipeArr[i - 1][0], 0);
            }

            if (i < 3) {
                close(pipeArr[i][0]);
                dup2(pipeArr[i][1], 1);
            }

            write(2, "Command: ", 9);
            write(2, commands[i], strlen(commands[i]));
            write(2, "\n", 1);

            char **tokens = tokenizer(arguments[i]);
            for(int i = 0; tokens[i] != NULL; i++) {
                write(2, tokens[i], strlen(tokens[i]));
                write(2, "\n", 1);
            }

            if (-1 == execvp(commands[i], tokens)) {
                errx(10, "Error with exec!");
            }
        } else {
            int status;
            waitpid(children[i], &status, 0);

            write(2, "Status: ", 8);
            char buff[2];
            sprintf(buff, "%d", WEXITSTATUS(status));
            write(2, buff, 1);
            write(2, "\n", 1);
        }
    }

    int status;
    waitpid(children[3], &status, 0);
    // printf("%d", WEXITSTATUS(status));

    // close(pipeArr[3][1]);
    // int buff, readStatus;
    // while(0 != (readStatus = read(pipeArr[3][0], &buff, sizeof(int)))) {
    //     if (-1 == readStatus) {
    //         errx(11, "Error with read");
    //     }
    // }
    // printf("%d", buff);

    return 0;
}