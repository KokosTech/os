#include "main.h"

#include <err.h>
#include <errno.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
    if (argc < 4) errx(10, "Arguments not provided");
    grader(argc, argv);
    return 0;
}