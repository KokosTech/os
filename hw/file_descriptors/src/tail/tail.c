#include "tail.h"

#include <err.h>
#include <errno.h>
#include <stdio.h>

#include "../utils.h"

int go_backwards_line(int fd, size_t n) {
    if (-1 == fd) {
        err(EBADF, "[tail] file open failed, or file doesn't exist\n");
    }
    if (0 == eseek(fd, 0, SEEK_END)) return 0; // empty file

    eseek(fd, -1, SEEK_END);

    char c;
    size_t lines = 0;
    while (1 == eread(fd, &c, 1)) {
        if (c == '\n') ++lines;
        if (lines == n || 0 == eseek(fd, -1, SEEK_CUR)) break;

        eseek(fd, -1, SEEK_CUR);
    }

    return 0;
}
