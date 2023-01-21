#pragma once

#include <ctype.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct pair {
    char *mem;
    void *addr;
    struct pair *next;
} pair_t;

typedef struct hash_table {
    pair_t **buckets;
    size_t size;
} hash_table_t;

size_t hash(char *str);
hash_table_t *init(size_t size);
hash_table_t *put(hash_table_t *hash_table, char *mem, void *addr);

bool contains(hash_table_t *hash_table, char *mem);
void *get(hash_table_t *hash_table, char *mem);
void *remove_pair(hash_table_t *hash_table, char *mem);

void *hmalloc(hash_table_t *hash_table, char *mem, size_t size);
void *hrealloc(hash_table_t *hash_table, char *mem, size_t size);
void hfree(hash_table_t *hash_table, char *mem);

hash_table_t *destroy_table(hash_table_t *hash_table);
