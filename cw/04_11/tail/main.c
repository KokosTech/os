#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>

#define DEFAULT_LINES 10

// [1] filename
// [2] -n
// [3] <number>

char *read_line(int fd) {
    char c;
    char *res = (char *)malloc(1 * sizeof(char));
    res[0] = '\0';

    size_t len = 0;
    while (1 == read(fd, &c, 1)) {
        if (c == '\n') {
            res[len] = '\0';
            return res;
        }
        res[len] = c;
        ++len;
        res = (char *)realloc(res, (len + 1) * sizeof(char));
    }

    return res;
}

int print_lines(int fd, size_t n) {
    if (-1 == fd) {
        write(2, "[print_lines] file error\n\0", 24);
        exit(1);
    }

    lseek(fd, -1, SEEK_END);
    char c;
    size_t lines = 0;
    while (1 == read(fd, &c, 1)) {
        if (c == '\n') {
            ++lines;
        }
        if (lines == n) {
            break;
        }
        lseek(fd, -2, SEEK_CUR);
    }

    char *line;
    while (1) {
        line = read_line(fd);
        if (line == NULL) {
            write(2, "[print_lines] line error\n\0", 25);
            exit(1);
        }

        if (line[0] == '\0') {
            free(line);
            return 0;
        }

        printf("%s ", line);
        printf("\n");
        free(line);
        line = NULL;
    }

    return 0;
}

int main(int argc, char **argv) {
    if (argc < 2) {
        write(2, "Not enough arguments provided\n\0", 31);
        exit(1);
    }

    char *filename = argv[1];
    int fd = open(filename, O_RDONLY);

    if (-1 == fd) {
        write(2, "file open failed, or file doesn't exist\n\0", 41);
        exit(1);
    }

    if (2 == argc) {
        print_lines(fd, DEFAULT_LINES);
    } else {
        if (!strcmp("-n", argv[2])) {
            // get int from argv[3]
            int n = 0;
            n = atoi(argv[3]);
            if (n < 1) {
                write(2, "Invalid argument\n\0", 18);
                close(fd);
                exit(1);
            }

            print_lines(fd, n);

        } else {
            write(2, "Invalid argument\n\0", 18);
            close(fd);
            exit(1);
        }
    }

    close(fd);
    return 0;
}