#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>

// [1] filename
// [2] -l / c
// [3] -l /-c

off_t seek(int fd, off_t offset, int whence) {
    if (-1 == fd) {
        write(2, "[seek] file error\n\0", 19);
        exit(1);
    }

    off_t res = lseek(fd, offset, whence);

    if (-1 == res) {
        write(2, "[seek] seek error\n\0", 18);
        close(fd);
        exit(1);
    }

    return res;
}

size_t get_lines(int fd) {
    if (-1 == fd) {
        write(2, "[get_lines] file error\n\0", 24);
        exit(1);
    }

    size_t res = 1;
    char c;
    while (1 == read(fd, &c, 1)) {
        if (c == '\n') ++res;
    }

    return res;
}

size_t get_bytes(int fd) {
    if (-1 == fd) {
        write(2, "[get_lines] file error\n\0", 24);
    }

    size_t res = seek(fd, 0, SEEK_END);
    seek(fd, 0, SEEK_SET);

    return res;
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

    if (argc > 2) {
        for (short i = 2; i < 4 && i < argc; ++i) {
            if (!strcmp("-l", argv[i])) {
                printf("%zu", get_lines(fd));
            } else if (!strcmp("-c", argv[i])) {
                printf("%zu", get_bytes(fd));
            } else {
                write(2, "\ninvalid argument, continuing...\n\0", 34);
            }
            printf(" ");
        }
        printf("\n");
    } else {
        printf("%zu %zu\n", get_lines(fd), get_bytes(fd));
    }

    close(fd);
    return 0;
}