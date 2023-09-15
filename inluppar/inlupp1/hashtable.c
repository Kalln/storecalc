#include "hashtable.h"
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

// Karl Widborg Kortelainen & William Paradell

typedef struct entry entry_t;
#define Success(v)      (option_t) { .success = true, .value = v };
#define Failure()       (option_t) { .success = false };
#define Successful(o)   (o.success == true) // TODO: use this definition
#define Unsuccessful(o) (o.success == false) // TODO: use this definition

#define no_buckets 17

struct entry
{
    int key; 
    char *value;
    entry_t *next;
};

struct hash_table 
{
    entry_t *buckets[no_buckets];
};


/// @brief Creates an entry for hashtable.
/// @param key key for new entry, (expects that there is no previous entry of this key).
/// @param val value for the entry.
/// @param first_entry first entry in the bucket.
/// @return ptr to new entry.
static entry_t *entry_create(int key, char* val, entry_t *first_entry)
{
    entry_t *new_entry = calloc(1, sizeof(entry_t));
    new_entry->key = key;
    new_entry->value = val;
    new_entry->next = first_entry;
    return new_entry;
}

static void entry_destroy(entry_t *entry) 
{
    free(entry);
}

ioopm_hash_table_t *ioopm_hash_table_create(void)
{
    ioopm_hash_table_t *result = calloc(1, sizeof(ioopm_hash_table_t));
    for (int i = 0; i < no_buckets; i++) 
    {
        entry_t *ent = entry_create(0, NULL, NULL);
        result->buckets[i] = ent;
    }

    return result;
}

// more effective and itterative version of bucket destroy.
static void bucket_destroy(entry_t *bucket_to_destroy) 
{
    while (bucket_to_destroy != NULL)
        {
            entry_t *next_ptr = bucket_to_destroy->next;
            entry_destroy(bucket_to_destroy);
            bucket_to_destroy = next_ptr;
        }
}

// recursive version of bucket_destroy F13
static void bucket_destroy_rec(entry_t *bucket_to_destroy) {
    if (bucket_to_destroy == NULL) {
        return;
    }
    entry_t *next_ptr = bucket_to_destroy->next;
    entry_destroy(bucket_to_destroy);
    bucket_destroy_rec(next_ptr);
}

void ioopm_hash_table_destroy(ioopm_hash_table_t *ht) 
{   
    for (int i = 0; i < no_buckets; i++)
    {
        bucket_destroy(ht->buckets[i]);

    }
    free(ht);
    return;
}

/// @brief Finds if a key has a corresponding value and returns a pointer to said value
/// @param bucket {entry_t *} - bucket that key mapped to
/// @param key {int} - key to be checked for in the bucket
/// @return {entry_t *} - pointer to value with the key: key if it exist otherwise NULL
static entry_t *find_previous_entry_for_key(entry_t *bucket, int key)
{

    while (bucket->next != NULL) 
    {
        if (bucket->next->key == key || bucket->next->key > key) 
        {
            break;
        }
        bucket = bucket->next;
    }

    return bucket;
}
/// @brief calculates the bucket in hashtable 
/// @param key {int}. this is used to calculate the bucket.
/// @return {int} bucket in hashtable.
static int bucket_calc(int key) 
{
    /// since we don't have buckets under 0
   return key >= 0 ? (key % no_buckets) : ((key % no_buckets) + no_buckets); // korrekt
   //return key > 0 ? (key % 17) : ((key % 17) + 17); // fel 2
   //return key % 17; // fel 1
}

void ioopm_hash_table_insert(ioopm_hash_table_t *ht, int key, char *value)
{
  /// Calculate the bucket for this entry
    int bucket = bucket_calc(key);
  /// Search for an existing entry for a key
    entry_t *entry = find_previous_entry_for_key(ht->buckets[bucket], key);
    entry_t *next = entry->next;

  /// Check if the next entry should be updated or not
    if (next != NULL && next->key == key) 
    {
        next->value = value;
    } else 
    {
        entry->next = entry_create(key, value, next);
    }
}

char *ioopm_hash_table_remove(ioopm_hash_table_t *ht, int key) 
{
    int bucket = bucket_calc(key);
    entry_t *prev_entry = find_previous_entry_for_key(ht->buckets[bucket], key);

    if (prev_entry->next == NULL) {
        
        return NULL;
    }
    entry_t *to_remove = prev_entry->next;
    char *val = to_remove->value;
    prev_entry->next = to_remove->next;

    free(to_remove);
    return val;
}

option_t ioopm_hash_table_lookup(ioopm_hash_table_t *ht, int key)
{
    int bucket = bucket_calc(key); 

    /// Find the previous entry for key
    entry_t *tmp = find_previous_entry_for_key(ht->buckets[bucket], key);
    entry_t *next = tmp->next;
    
    if (next && next->key == key)
    {
        /// If entry was found, return its value...
        return Success(next->value);
    } else
    {
        //option_t option = {.success = false};
        return Failure();
    }
    
}