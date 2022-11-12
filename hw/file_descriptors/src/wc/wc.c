#include <err.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>

#include "../utils.h"

size_t get_lines(int fd) {
    if (-1 == fd) {
        errx(EINVAL, "[get_lines] file error\n");
    }

    size_t res = 0;
    char c;
    while (1 == eread(fd, &c, 1)) {
        if (c == '\n') ++res;
    }

    eseek(fd, 0, SEEK_SET);

    return res;
}

size_t get_bytes(int fd) {
    if (-1 == fd) {
        err(EBADF, "[get_bytes] file error\n");
    }

    size_t res = eseek(fd, 0, SEEK_END);
    eseek(fd, 0, SEEK_SET);

    return res;
}
