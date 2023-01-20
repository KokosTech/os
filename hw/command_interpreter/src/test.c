#include <stdio.h>

#include "utils/hashmap.h"

hash_table_t *MEMORY = NULL;

int main() {
    MEMORY = init(100);
    char *string;
    string = (char *)hmalloc(MEMORY, "string", 10);
    strcpy(string, "123456789\0");
    printf("%s\n", string);
    hfree(MEMORY, "string");
    MEMORY = destroy_table(MEMORY);
}