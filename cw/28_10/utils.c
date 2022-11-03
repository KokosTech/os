#include <stdlib.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include "utils.h"


size_t strleny(char *a) { // bytes before \0
    if(!a) return 0;

    size_t i = 0;
    for(; a[i] != '\0'; ++i);
    return i;
}

char *strcaty(char *a, char *b) { // a + b and \0
    size_t len_a = strleny(a);
    size_t len_b = strleny(b);
    
    char *res = (char *)malloc((len_a + len_b + 1) * sizeof(char));

    for(size_t i = 0; i < len_a; ++i) res[i] = a[i];
    for(size_t i = 0; i < len_b; ++i) res[i + len_a] = b[i];
    res[len_a + len_b] = '\0';

    return res;
}

void swap(int *a, int *b) { // swap a and b
    *a = *b ^ *a;
    *b = *a ^ *b;
    *a = *a ^ *b;
}

void swap_void(void *a, void *b, size_t count) { // swap a and  b (no user checks)
    // skipnah go :)
    for(size_t i = 0; i < count; ++i) {
        ((char *)a)[i] = ((char *)b)[i] ^ ((char *)a)[i];
        ((char *)b)[i] = ((char *)a)[i] ^ ((char *)b)[i];
        ((char *)a)[i] = ((char *)a)[i] ^ ((char *)b)[i];
    }
}

char *strdupy(char *a) { // new mem, copy
    size_t len = strleny(a);
    char *new_str = (char *)malloc(len * sizeof(char));

    for(size_t i = 0; i < len; ++i) new_str[i] = a[i];
    new_str[len] = '\0';
}

char *readliney(int fd) { // read line from fd and return it, the cursor should be a symbol after the new line
    char c;
    char *res = (char *)malloc(1 * sizeof(char));
    res[0] = "";
    while(1 == read(fd, &c, 1)) {
        if (c == '\n'){
            res = strcaty(res, '\0');
            break;
        } 
        write(1, &c, 1);
        res = strcaty(res, &c);
    }

    return res;
}

