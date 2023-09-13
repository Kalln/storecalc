#include "hashtable.h"
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

// Karl Widborg Kortelainen & William Paradell

typedef struct entry entry_t;

struct entry
{
    int key; 
    char *value;
    entry_t *next;
};

struct hash_table 
{
    entry_t *buckets[17];
};

/// @brief Creates an entry for hashtable.
/// @param key key for new entry, (expects that there is no previous entry of this key).
/// @param val value for the entry.
/// @param first_entry first entry in the bucket.
/// @return ptr to new entry.
static entry_t *create_entry(int key, char* val, entry_t *first_entry)
{
    entry_t *new_entry = calloc(1, sizeof(entry_t));
    new_entry->key = key;
    new_entry->value = val;
    new_entry->next = first_entry;
    return new_entry;
}

ioopm_hash_table_t *ioopm_hash_table_create(void)
{
    ioopm_hash_table_t *result = calloc(1, sizeof(ioopm_hash_table_t));
    for (int i = 0; i < 17; i++) 
    {
        entry_t *ent = create_entry(0, NULL, NULL);
        result->buckets[i] = ent;
    }

    return result;
}

void ioopm_hash_table_destroy(ioopm_hash_table_t *ht) 
{
    free(ht);
    return;
}

/// @brief Finds if a key has a corresponding value and returns a pointer to said value
/// @param bucket bucket that key mapped to
/// @param key key to be checked for in the bucket
/// @return pointer to value with the key: key if it exist otherwise NULL
static entry_t *find_entry_for_key(entry_t *bucket, int key)
{

    while (bucket != NULL)
    {
        if (bucket->key == key)
        {
            break;
        }
        bucket = bucket->next;
    }
    return bucket;
}



void ioopm_hash_table_insert(ioopm_hash_table_t *ht, int key, char *value) 
{
    int bucket = key % 17;
    entry_t *existing_entry = find_entry_for_key(ht->buckets[bucket], key);

    if (existing_entry != NULL) /// i.e., it exists
    {
        existing_entry->value = value;
    }
    else
    {
        /// Get a pointer to the first entry in the bucket
        entry_t *first_entry = ht->buckets[bucket];
        /// Create a new entry
        entry_t *new_entry = create_entry(key, value, first_entry);
        /// Make the new entry the first entry in the bucket
        ht->buckets[bucket] = new_entry;
    }
}

char *ioopm_hash_table_lookup(ioopm_hash_table_t *ht, int key)
{
    return NULL;
}