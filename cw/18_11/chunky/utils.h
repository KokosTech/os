#pragma once

#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

// error safe file descriptor functions

int eopen(const char *pathname, int mode);
ssize_t eread(int fd, void *buf, size_t count);
off_t eseek(int fd, off_t offset, int whence);
int eclose(int fd);

// common utils

char *read_line(int fd);
char *read_line_chunky(int fd);

int print_lines(int fd, size_t n);
int print_lines_chunky(int fd, size_t n);
