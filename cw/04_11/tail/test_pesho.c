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


int print_lines(int fd, size_t n) {
    if (-1 == fd) {
        write(2, "[print_lines] file error\n\0", 24);
        exit(1);
    }

    lseek(fd, -1, SEEK_END);

    char c;
    size_t lines = 0;
    while (1 == read(fd, &c, 1)) {
        if (c == '\n') ++lines;
        if (lines == n) break;
        int d = lseek(fd, -1, SEEK_CUR);
        printf("lseek %lld\n", d);
        int p = lseek(fd, -1, SEEK_CUR);
        printf("lseek %lld\n", p);
        if (p == 0) break;
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
    }

    close(fd);
    return 0;
}