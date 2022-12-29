#include <err.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

char *get_path(char *input) {
    if (input == NULL) errx(EINVAL, "[get_path] input is NULL\n");

    struct stat sb = {0};
    if (stat(input, &sb) != 0 && !S_ISDIR(sb.st_mode))
        err(ENOENT, "[get_path] directory doesn't exist\n");

    size_t len = strlen(input) + 1;
    if (len > 0 && input[len - 2] != '/') ++len;

    char *path = malloc(len * sizeof(char));

    if (NULL == path) err(ENOMEM, "[get_path] malloc failed\n");

    strcpy(path, input);
    path[len - 2] = '/';
    path[len - 1] = '\0';

    return path;
}
