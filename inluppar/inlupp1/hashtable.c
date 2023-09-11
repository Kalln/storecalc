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
    int bucket = key % 17; // Bucket to insert into.
    entry_t *first_entry = ht->buckets[bucket]; // ptr to first entry in bucket.
    entry_t *cursor = first_entry;

    while (cursor != NULL) {
        if(cursor->key == key) {
            cursor->value = value;
            return;
        }
    }
    cursor = cursor->next;
}

char *ioopm_hash_table_lookup(ioopm_hash_table_t *ht, int key) {
    return NULL;
}