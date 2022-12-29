#include "main.h"

#include <err.h>
#include <errno.h>

int main(int argc, char *argv[]) {
    if (argc < 4)
        errx(EINVAL,
             "Not enough arguments provided.\nPlease use the following "
             "format:\n./<program_name> <students' answers dir> <correct "
             "answers file> <results dir>");

    grader(argc, argv);

    return 0;
}
