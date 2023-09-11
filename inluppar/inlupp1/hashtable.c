#include "hashtable.h"
#include <stdlib.h>
#include <stddef.h>

// Karl Widborg Kortelainen & William Paradell

typedef struct entry entry_t;

struct entry {
    int key; 
    char *value;
    entry_t *next;
};

struct hash_table {
    entry_t *buckets[17];
};



ioopm_hash_table_t *ioopm_hash_table_create(void) {
    ioopm_hash_table_t *result = calloc(1, sizeof(ioopm_hash_table_t));
    return result;
}

void ioopm_hash_table_destroy(ioopm_hash_table_t *ht) {
    free(ht);
    return;
}

void ioopm_hash_table_insert(ioopm_hash_table_t *ht, int key, char *value) {
    //TODO: Stub
}

char ioopm_hash_table_lookup(void) {
    return NULL;
}