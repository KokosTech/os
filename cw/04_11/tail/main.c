#include <err.h>
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

#include "../utils.h"
#include "tail.h"

// [1] filename
// [2] -n
// [3] <number>

int main(int argc, char **argv) {
    if (argc < 2) {
        errx(EINVAL, "[tail] not enough arguments provided\n");
    }

    char *filename = argv[1];
    int fd = eopen(filename, O_RDONLY);

    if (2 == argc) {
        go_backwards_line(fd, DEFAULT_LINES);
        print_lines(fd, DEFAULT_LINES);
    } else {
        if (!strcmp("-n", argv[2])) {
            int n = 0;
            n = atoi(argv[3]);

            if (n < 1) {
                eclose(fd);
                errx(EINVAL,
                     "[head] invalid argument - number expected after -n");
            }

            go_backwards_line(fd, n);
            print_lines(fd, n);
        } else {
            eclose(fd);
            errx(EINVAL,
                 "[tail] invalid argument - head only expects optinal argument "
                 "-n");
        }
    }

    eclose(fd);
    return 0;
}