
#include "utils.h"

#include <err.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>

// error safe file descriptor functions

int eopen(const char *pathname, int mode) {
    int fd = open(pathname, mode);

    if (-1 == fd) {
        err(ENOENT, "file open failed, or file doesn't exist\n");
    }

    return fd;
}

size_t eread(int fd, void *buf, size_t count) {
    if (-1 == fd) {
        err(EBADF, "[eread] file error\n");
    }

    long long res = read(fd, buf, count);

    if (-1 == res) {
        eclose(fd);
        err(EBADF, "[eread] problem with reading file\n");
    }

    return res;
}

off_t eseek(int fd, off_t offset, int whence) {
    if (-1 == fd) {
        err(EBADF, "[eseek] file error\n");
    }

    off_t res = lseek(fd, offset, whence);

    if (-1 == res) {
        eclose(fd);
        err(EBADF, "[eseek] problem with seeking file\n");
    }

    return res;
}

int eclose(int fd) {
    if (-1 == fd) {
        err(EBADF, "[eclose] file error\n");
    }

    int res = close(fd);

    if (-1 == res) {
        err(EBADF, "[eclose] problem with closing file\n");
    }
    return res;
}

// common utils

char *read_line(int fd) {
    if (-1 == fd) {
        err(EBADF, "[read_line] file error\n");
    }

    char c;
    size_t res_read = read(fd, &c, 1);

    if (0 == res_read) {
        return NULL;
    }

    char *res = (char *)calloc(1, sizeof(char));

    size_t len = 0;
    while (1 == res_read && c != '\n') {
        res[len++] = c;
        res = (char *)realloc(res, (len + 1) * sizeof(char));

        res_read = read(fd, &c, 1);
    }

    if (0 == res_read || c == '\n') {
        res[len++] = '\0';
        return res;
    }

    return res;
}

int print_lines(int fd, size_t n) {
    if (-1 == fd) {
        err(EBADF, "[print_lines] file error\n");
    }

    char *line = read_line(fd);
    for (size_t i = 0; line && i < n; ++i) {
        printf("%s\n", line);
        free(line);
        line = read_line(fd);
    }

    return 0;
}
