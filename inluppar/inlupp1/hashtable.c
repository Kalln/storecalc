#include "hashtable.h"
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include "linked_list.h"
#include <stdbool.h>
#include "common.h"



// Karl Widborg Kortelainen & William Paradell

typedef struct entry entry_t;
#define Success(v)      (option_t){.success = true, .value = v};
#define Failure()       (option_t){.success = false};


#define no_buckets 17
#define ioopm_int_str_ht_insert(ht, i, s) \
   ioopm_hash_table_insert(ht, int_elem(i), ptr_elem(s))

struct entry
{
    elem_t key;
    elem_t value;
    entry_t *next;
};

struct hash_table
{
    entry_t *buckets[no_buckets];
    int size;
    ioopm_hash_function ht_function;
    ioopm_eq_function eq_key;
    ioopm_eq_function eq_val;
};

/// @brief Creates an entry for hashtable.
/// @param key key for new entry, (expects that there is no previous entry of this key).
/// @param val value for the entry.
/// @param first_entry first entry in the bucket.
/// @return ptr to new entry.
static entry_t *entry_create(elem_t key, elem_t val, entry_t *first_entry)
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

ioopm_hash_table_t *ioopm_hash_table_create(ioopm_hash_function hash_function, ioopm_eq_function eq_key, ioopm_eq_function eq_val)
{
    ioopm_hash_table_t *result = calloc(1, sizeof(ioopm_hash_table_t));
    for (size_t i = 0; i < no_buckets; i++)
    {
        entry_t *ent = entry_create(int_elem(0), ptr_elem(NULL), NULL);
        result->buckets[i] = ent;
    }
    result->size = 0;
    result->ht_function = hash_function;
    result->eq_key = eq_key;
    result->eq_val = eq_val;


    return result;
}

static void bucket_destroy(entry_t *bucket_to_destroy)
{
    while (bucket_to_destroy != NULL)
    {
        entry_t *next_ptr = bucket_to_destroy->next;
        entry_destroy(bucket_to_destroy);
        bucket_to_destroy = next_ptr;
    }
}

void ioopm_hash_table_destroy(ioopm_hash_table_t *ht)
{
    for (size_t i = 0; i < no_buckets; i++)
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
static entry_t *find_previous_entry_for_key(entry_t *bucket, elem_t key, ioopm_eq_function eq)
{

    while (bucket->next != NULL)
    {
        if (eq(bucket->next->key, key))
        {
            break;
        }
        bucket = bucket->next;
    }

    return bucket;
}
/// @brief Calculates the bucket for a key
/// @param hash hash function to calculate bucket. IT MUST RETURN POSITIVE VALUE.
/// @param key key to hash.
/// @return will return a bucket between 0 and no_buckets.
static size_t bucket_calc(ioopm_hash_function hash, elem_t key)
{
    /// since we don't have buckets under 0
    return hash(key) % no_buckets;

}

void ioopm_hash_table_insert(ioopm_hash_table_t *ht, elem_t key, elem_t value)
{
    /// Calculate the bucket for this entry
    const size_t bucket = bucket_calc(ht->ht_function, key);

    /// Search for an existing entry for a key
    entry_t *entry = find_previous_entry_for_key(ht->buckets[bucket], key, ht->eq_key);
    entry_t *next = entry->next;

    /// Check if the next entry should be updated or not

    if (next != NULL && ht->eq_key(next->key, key))
    {
        next->value = value;
    }
    else
    {
        // Allocate memory for value?
        // that would mean that remove would have to free the value.
        entry->next = entry_create(key, value, next);
        ht->size += 1;
    }
}

elem_t ioopm_hash_table_remove(ioopm_hash_table_t *ht, elem_t key)
{
    const size_t bucket = bucket_calc(ht->ht_function, key);
    entry_t *prev_entry = find_previous_entry_for_key(ht->buckets[bucket], key, ht->eq_key);

    /// If there is no entry for this key, return failure
    if (prev_entry == NULL || prev_entry->next == NULL)
    {
        return void_elem(NULL);
    }
    // If there is an entry for this key, remove it and return its value
    entry_t *to_remove = prev_entry->next;
    enum elem_type type = to_remove->key.type;

    // De-refrence pointer and set new entry
    prev_entry->next = to_remove->next;

    ht->size -= 1;

    if (type == ELEM_STR)
    {
        elem_t val = ptr_elem(to_remove->value.data.str);
        free(to_remove);
        return val;

    } else
    {
        elem_t val;
        val = int_elem(to_remove->value.data.val);
        free(to_remove);
        return val;
    }
}

option_t ioopm_hash_table_lookup(ioopm_hash_table_t *ht, elem_t key)
{
    const size_t bucket = bucket_calc(ht->ht_function, key);

    /// Find the previous entry for key
    entry_t *tmp = find_previous_entry_for_key(ht->buckets[bucket], key, ht->eq_key);
    entry_t *next = tmp->next;

    if (next && ht->eq_key(next->key, key))
    {
        if (next->value.type == ELEM_STR)
        {
            return Success(next->value);
        }

        return Success(next->value);
    }
    else
    {
        return Failure();
    }
}

size_t ioopm_hash_table_size(ioopm_hash_table_t *ht)
{
    return ht->size;
}

bool ioopm_hash_table_is_empty(ioopm_hash_table_t *ht)
{
    return ioopm_hash_table_size(ht) == 0;
}

void ioopm_hash_table_clear(ioopm_hash_table_t *ht)
{
    for (size_t i = 0; i < no_buckets; i++)
    {
        bucket_destroy(ht->buckets[i]->next);
        ht->buckets[i]->next = NULL;
    }

    ht->size = 0;
    return;
}

ioopm_list_t *ioopm_hash_table_keys(ioopm_hash_table_t *ht)
{
    ioopm_list_t *lt = ioopm_linked_list_create(int_eq);


    for (size_t i = 0; i < no_buckets; i++)
    {
        entry_t *cursor = ht->buckets[i]->next;

        while (cursor != NULL)
        {
            ioopm_linked_list_append(lt, cursor->key); // Puts the value last in the linked list
            cursor = cursor->next;
        }
    }

    return lt;
}

ioopm_list_t *ioopm_hash_table_values(ioopm_hash_table_t *ht)
{
    ioopm_list_t *values = ioopm_linked_list_create(ht->eq_val);

    for (int i = 0; i < no_buckets; i++)
    {
        entry_t *cursor = ht->buckets[i]->next; // Dummy bucket is always first, so next is first entry.

        while (cursor != NULL)
        {
            elem_t elem_ptr = cursor->value;
            ioopm_linked_list_append(values, elem_ptr);
            cursor = cursor->next;
        }
    }

    return values;
}

bool ioopm_hash_table_has_key(ioopm_hash_table_t *ht, elem_t key)
{
    const size_t bucket_key = bucket_calc(ht->ht_function, key);
    entry_t *cursor = ht->buckets[bucket_key]->next;

    while (cursor != NULL)
    {
        elem_t current_key = cursor->key;

        if (ht->eq_key(current_key, key))
        {
            return true;
        }

        cursor = cursor->next;
    }

    return false;
}

bool ioopm_hash_table_has_value(ioopm_hash_table_t *ht, elem_t value)
{
    ioopm_list_t *ht_val = ioopm_hash_table_values(ht);
    bool booelan = ioopm_linked_list_contains(ht_val, value);
    ioopm_linked_list_destroy(ht_val);
    return booelan;
}

bool ioopm_hash_table_all(ioopm_hash_table_t *ht, ioopm_predicate pred, void *arg)
{
    for (size_t i = 0; i < no_buckets; i++)
    {
        entry_t *cursor = ht->buckets[i]->next;
        while (cursor != NULL)
        {
            if (!pred(cursor->key, cursor->value, arg))
            {
                return false;
            }
            cursor = cursor->next;
        }
    }
    return true;
}

bool ioopm_hash_table_any(ioopm_hash_table_t *ht, ioopm_predicate pred, void *arg)
{
    for (size_t i = 0; i < no_buckets; i++)
    {

        entry_t *cursor = ht->buckets[i]->next;

        while (cursor != NULL)
        {
            if (pred(cursor->key, cursor->value, arg))
            {
                return true;
            }
            cursor = cursor->next;
        }
    }
    return false;
}

void ioopm_hash_table_apply_to_all(ioopm_hash_table_t *ht, ioopm_apply_function apply_fun, void *arg)
{

    for (size_t i = 0; i < no_buckets; i++)
    {
        entry_t *cursor = ht->buckets[i]->next;
        while (cursor != NULL)
        {
            apply_fun(cursor->key, &cursor->value, arg);
            cursor = cursor->next;
        }
    }
}