#include <stdio.h>

#include "utils/hashmap.h"
#include "utils/pgc.h"

int main() {
    MEMORY = init(100);
    char *string;
    string = (char *)hmalloc(MEMORY, "string", 10);
    strcpy(string, "123456789\0");
    printf("%s\n", string);
    char *string2 = (char *)hmalloc(MEMORY, "string2", strlen(string) + 1);
    strcpy(string2, string);
    hfree(MEMORY, "string");
    MEMORY = destroy_table(MEMORY);
}