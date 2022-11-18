#include <err.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>

#include "../utils.h"
#include "wc.h"

// [1] filename
// [2] -l / -c
// [3] -l / -c

int main(int argc, char **argv) {
    if (argc < 2) {
        errx(EINVAL, "[wc] not enough arguments provided\n");
    }

    char *filename = argv[1];
    int fd = eopen(filename, O_RDONLY);

    if (argc > 2) {
        for (short i = 2; i < 4 && i < argc; ++i) {
            if (!strcmp("-l", argv[i])) {
                printf("%zu", get_lines(fd));
            } else if (!strcmp("-c", argv[i])) {
                printf("%zu", get_bytes(fd));
            } else {
                eclose(fd);
                errx(EINVAL,
                     "[wc] invalid argument - wc only expects optional "
                     "arguments -l and -c");
            }
            printf(" ");
        }
        printf("\n");
    } else {
        printf("%zu %zu\n", get_lines(fd), get_bytes(fd));
    }

    eclose(fd);
    return 0;
}

