#pragma once
#include <stdlib.h>

size_t strleny(char *a);
char *strcaty(char *a, char *b);
void swap(int *a, int *b);
void swap_void(void *a, void *b, size_t count);
char *strdupy(char *a);
char *readliney(int fd);
char *readliney_optimized(int fd);