#include "hashmap.h"

size_t hash(char *str) {
    size_t hash = 5381;
    int c;

    char *hstr = (char *)malloc(strlen(str) + 1);
    strcpy(hstr, str);

    for(int i = 0; i < strlen(hstr); i++) {
        hash = ((hash << 5) + hash) + hstr[i];
    }

    free(hstr);
    return hash;
}

hash_table_t *init(size_t size) {
    hash_table_t *hash_table = (hash_table_t *)malloc(sizeof(hash_table_t));

    hash_table->size = size;
    hash_table->buckets = (pair_t **)calloc(size, sizeof(pair_t *));

    return hash_table;
}

// HOMEWORK - Create a random hash func for a random struct

hash_table_t *put(hash_table_t *hash_table, char *mem, void *addr) {
    pair_t *new_pair = (pair_t *)calloc(1, sizeof(pair_t));
    new_pair->mem = (char *)malloc(strlen(mem) + 1);
    strcpy(new_pair->mem, mem);
    new_pair->addr = addr;

    // ADD IF HEAD
    size_t bucket_num = hash(mem) % hash_table->size;
    if (hash_table->buckets[bucket_num] == NULL) {
        hash_table->buckets[bucket_num] = new_pair;
        return hash_table;
    }

    new_pair->next = hash_table->buckets[bucket_num];

    // CHECK FOR THE SAME KEY
    pair_t *current = hash_table->buckets[bucket_num];
    while (current != NULL) {
        if (!strcmp(current->mem, mem)) {
            current->addr = addr;
            break;
        }
        current = current->next;
    }

    // IF NOT - ADD A NEW ELLEMENT
    hash_table->buckets[bucket_num] = new_pair;

    return hash_table;
}

bool contains(hash_table_t *hash_table, char *mem) {
    unsigned short bucket_num = hash(mem) % hash_table->size;
    if (hash_table->buckets[bucket_num] == NULL) {
        printf("NO SUCH KEY\n");
        return false;
    }

    pair_t *current = hash_table->buckets[bucket_num];
    while (current != NULL) {
        if (!strcmp(current->mem, mem)) {
            return true;
        }
        current = current->next;
    }
    printf("NO SUCH KEY\n");
    return false;
}

void *get(hash_table_t *hash_table, char *mem) {
    size_t bucket_num = hash(mem) % hash_table->size;
    if (hash_table->buckets[bucket_num] == NULL) {
        printf("NO SUCH KEY\n");
        return 0;
    }

    pair_t *current = hash_table->buckets[bucket_num];
    while (current != NULL) {
        if (!strcmp(current->mem, mem)) {
            return current->addr;
        }
        current = current->next;
    }
    printf("NO SUCH KEY\n");
    return 0;
}

void *remove_pair(hash_table_t *hash_table, char *mem) {
    size_t bucket_num = hash(mem) % hash_table->size;
    if (hash_table->buckets[bucket_num] == NULL) {
        printf("NO SUCH KEY\n");
        return 0;
    }

    pair_t *current = hash_table->buckets[bucket_num];
    pair_t *prev = NULL;
    while (current != NULL) {
        if (!strcmp(current->mem, mem)) {
            if (prev == NULL) {
                hash_table->buckets[bucket_num] = current->next;
            } else {
                prev->next = current->next;
            }
            void *addr = current->addr;
            free(current->mem);
            current->mem = NULL;
            free(current);
            current = NULL;
            return addr;
        }
        prev = current;
        current = current->next;
    }
    printf("NO SUCH KEY\n");
    return 0;
}

void *hmalloc(hash_table_t *hash_table, char *mem, size_t size) {
    void *addr = malloc(size);
    put(hash_table, mem, addr);
    return addr;
}

void hfree(hash_table_t *hash_table, char *mem) {
    void *fmem = remove_pair(hash_table, mem);
    free(fmem);
    fmem = NULL;
}

hash_table_t *destroy_table(hash_table_t *hash_table) {
    for (int i = 0; i < hash_table->size; i++) {
        pair_t *current = hash_table->buckets[i];
        while (current != NULL) {
            pair_t *temp = current;
            current = current->next;

            free(temp->mem);
            temp->mem = NULL;

            free(temp->addr);
            temp->addr = NULL;

            free(temp);
            temp = NULL;
        }
    }
    free(hash_table->buckets);
    hash_table->buckets = NULL;
    free(hash_table);
    hash_table = NULL;
    return hash_table;
}