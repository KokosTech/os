#pragma once

#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

// error safe file descriptor functions

int eopen(const char *pathname, int mode);
size_t eread(int fd, void *buf, size_t count);
off_t eseek(int fd, off_t offset, int whence);
int eclose(int fd);
