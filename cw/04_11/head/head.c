
#include "head.h"

#include <err.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

#include "../utils.h"

int print_lines(int fd, size_t n) {
    if (-1 == fd) {
        err(EBADF, "[head] file open failed, or file doesn't exist\n");
    }

    char *line = read_line(fd);
    for (size_t i = 0; line && i < n; ++i) {
        printf("%s\n", line);
        free(line);
        line = read_line(fd);
    }

    return 0;
}