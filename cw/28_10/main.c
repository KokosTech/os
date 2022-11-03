#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/uio.h>
#include <unistd.h>
#include <fcntl.h>

#include "utils.h"

int main() {
/*     // dec
    char *str = malloc(50);
    char *str2 = malloc(50);

    // strleny
    printf("GET1: ");
    scanf("%s", str);
    printf("LEN: %zu\n", strleny(str));

    // strcaty
    printf("GET2: ");
    scanf("%s", str2);
    char *new_str = strcaty(str, str2);
    printf("CAT -> %s\n", new_str); */

    // strdupy

/*     int a = 5, b = 6; */
/*     swap(&a, &b); */
/*     printf("swap: %d, %d\n", a, b); */

    // free
/*     free(str);
    free(str2);
    free(new_str); */

/*     int fd = open("test.txt", O_RDONLY);

    if (-1 == fd){
        write(2, "file open failed, or file doesn't exist\n", 12);
        exit(1);
    } */

    char *str1 = malloc(50);
    char *str2 = malloc(50);

    str1 = "based\0";
    str2 = "none\0";

    swap_void(str1, str2, 3);
    

    //readliney(fd);
}