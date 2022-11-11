#include <stdlib.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include "utils.h"

// bytes before \0
size_t strleny(char *a) { 
    if(!a) return 0;

    size_t i = 0;
    for(; a[i] != '\0'; ++i);
    return i;
}

// a + b and \0
char *strcaty(char *a, char *b) {
    size_t len_a = strleny(a);
    size_t len_b = strleny(b);

    char *res = (char *)malloc((len_a + len_b + 1) * sizeof(char));

    for (size_t i = 0; i < len_a; ++i)
        res[i] = a[i];
    for (size_t i = 0; i < len_b; ++i)
        res[i + len_a] = b[i];

    res[len_a + len_b] = '\0';

    return res;
}

// swap a and b
void swap(int *a, int *b) { 
    *a = *b ^ *a;
    *b = *a ^ *b;
    *a = *a ^ *b;
}

// swap a and  b (no user checks)
void swap_void(void *a, void *b, size_t count) { 
    char *a_ = (char *)a;
    char *b_ = (char *)b;

    // could be optimized with a buffer (chunks)
    for(size_t i = 0; i < count; ++i) {
        a_[i] = b_[i] ^ a_[i];
        b_[i] = a_[i] ^ b_[i];
        a_[i] = a_[i] ^ b_[i];
    }
}

// new mem, copy
char *strdupy(char *a) { 
    size_t len = strleny(a);
    char *new_str = (char *)malloc((len + 1) * sizeof(char));

    for (size_t i = 0; i < len; ++i)
        new_str[i] = a[i];
    new_str[len] = '\0';

    return new_str;
}

// read line from fd and return it, the cursor should be a symbol after the new line (fancy version with memory leaks)
// unoptimized as hell + has more memory leaks than a whor---
char *readliney(int fd) { 
    char c;
    char *res = (char *)malloc(1 * sizeof(char));
    res[0] = "";
    while(1 == read(fd, &c, 1)) {
        if (c == '\n'){
            res = strcaty(res, NULL);
            break;
        } 
        write(1, &c, 1);
        res = strcaty(res, &c);
    }

    return res;
}

// optimized version of readliney (not really optimized, but it's the basic verion without memory leaks)
char *readliney_optimized(int fd) { 
    char c;
    char *res = (char *)malloc(1 * sizeof(char));

    size_t len = 0;
    while(1 == read(fd, &c, 1)) {
        if (c == '\n'){
            res[len] = '\0';
            break;
        } 
        write(1, &c, 1);
        res[len++] = c;
        res = (char *)realloc(res, (len + 1) * sizeof(char));
    }

    return res;
}