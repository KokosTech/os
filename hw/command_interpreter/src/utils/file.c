#include "file.h"

#include <err.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/types.h>

#include "hashmap.h"
// #include "pgc.h"

extern hash_table_t *MEMORY;

// error safe file descriptor functions

int eopen(const char *pathname, int mode) {
    int fd = open(pathname, mode);

    if (-1 == fd) {
        MEMORY = destroy_table(MEMORY);
        err(ENOENT, "file open failed, or file doesn't exist\n");
    }

    return fd;
}

size_t eread(int fd, void *buf, size_t count) {
    if (-1 == fd) {
        MEMORY = destroy_table(MEMORY);
        err(EBADF, "[eread] file error\n");
    }

    long long res = read(fd, buf, count);

    if (-1 == res) {
        eclose(fd);
        MEMORY = destroy_table(MEMORY);
        err(EBADF, "[eread] problem with reading file\n");
    }

    return res;
}

off_t eseek(int fd, off_t offset, int whence) {
    if (-1 == fd) {
        MEMORY = destroy_table(MEMORY);
        err(EBADF, "[eseek] file error\n");
    }

    off_t res = lseek(fd, offset, whence);

    if (-1 == res) {
        eclose(fd);
        MEMORY = destroy_table(MEMORY);
        err(EBADF, "[eseek] problem with seeking file\n");
    }

    return res;
}

int eclose(int fd) {
    if (-1 == fd) {
        MEMORY = destroy_table(MEMORY);
        err(EBADF, "[eclose] file error\n");
    }

    int res = close(fd);

    if (-1 == res) {
        MEMORY = destroy_table(MEMORY);
        err(EBADF, "[eclose] problem with closing file\n");
    }
    return res;
}
