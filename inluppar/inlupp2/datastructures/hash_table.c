#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash_table.h"
#include "linked_list.h"
#include "iterator.h"

#define No_Buckets 17

typedef struct entry entry_t;

typedef struct {
    elem_t key;
    elem_t value;
} key_value_pair;

struct entry {
    key_value_pair *kv;
    entry_t *next; // points to the next entry (possibly NULL)
};

struct hash_table {
    ioopm_list_t *buckets[No_Buckets];
    ioopm_hash_function hash_fn;
    ioopm_gr_or_eq_function gr_or_eq_fn;
    ioopm_eq_function key_eq_fn;
    ioopm_eq_function value_eq_fn;
};

int mod(int a, int b) {
    int r = a % b;
    return r < 0 ? r + b : r;
}

ioopm_hash_table_t *ioopm_hash_table_create(ioopm_hash_function hash_fn, ioopm_gr_or_eq_function gr_or_eq_fn, ioopm_eq_function key_eq_fn, ioopm_eq_function value_eq_fn) {
    ioopm_hash_table_t *result = calloc(1, sizeof(ioopm_hash_table_t));
    result->hash_fn = hash_fn;
    result->gr_or_eq_fn = gr_or_eq_fn;
    result->key_eq_fn = key_eq_fn;
    result->value_eq_fn = value_eq_fn;
    for (size_t i = 0; i < No_Buckets; i++) {
        result->buckets[i] = ioopm_linked_list_create(value_eq_fn);
    }
    return result;
}

static void destroy_key_values(ioopm_list_t *list) {
    ioopm_list_iterator_t *iter = ioopm_list_iterator(list);
    while (ioopm_iterator_has_next(iter)) {
        elem_t value = ioopm_iterator_next(iter);
        free(value.p);
    }
    ioopm_iterator_destroy(iter);
}

void ioopm_hash_table_destroy(ioopm_hash_table_t *ht) {
    for (size_t i = 0; i < No_Buckets; i++) {
        destroy_key_values(ht->buckets[i]);
        ioopm_linked_list_destroy(ht->buckets[i]);
    }
    free(ht);
}

void ioopm_hash_table_insert(ioopm_hash_table_t *ht, elem_t key, elem_t value) {
    int hashed_key = ht->hash_fn(key);

    size_t bucket = mod(hashed_key, No_Buckets);
    ioopm_list_iterator_t *iter = ioopm_list_iterator(ht->buckets[bucket]);
    size_t index = 0;
    bool non_empty_table = ioopm_iterator_has_next(iter);
    while (ioopm_iterator_has_next(iter)) {
        elem_t elem = ioopm_iterator_next(iter);
        if (ht->gr_or_eq_fn(((key_value_pair*) elem.p)->key, key)) {
            break;
        } else {
            ++index;
        }
    }

    elem_t curr = ioopm_iterator_current(iter);
    ioopm_iterator_destroy(iter);

    if (non_empty_table && ht->key_eq_fn(((key_value_pair*) curr.p)->key, key)) {
        ((key_value_pair*) curr.p)->value = value;
        return;
    }

    elem_t new_elem = { .p = (void *) calloc(1, sizeof(key_value_pair))};
    *(key_value_pair*) new_elem.p = (key_value_pair) {
        .key = key,
        .value = value
    };
    ioopm_linked_list_insert(ht->buckets[bucket], index, new_elem);

}

elem_t *ioopm_hash_table_lookup(ioopm_hash_table_t *ht, elem_t key) {
    int hashed_key = ht->hash_fn(key);

    elem_t *res = NULL;
    size_t bucket = mod(hashed_key, No_Buckets);
    ioopm_list_iterator_t *iter = ioopm_list_iterator(ht->buckets[bucket]);
    while (ioopm_iterator_has_next(iter)) {
        elem_t elem = ioopm_iterator_next(iter);
        if (ht->key_eq_fn(((key_value_pair*) elem.p)->key, key)) {
            res = &((key_value_pair*) elem.p)->value;
            break;
        }
    }
    ioopm_iterator_destroy(iter);
    return res;
}

elem_t ioopm_hash_table_remove(ioopm_hash_table_t *ht, elem_t key, bool *success) {
    int hashed_key = ht->hash_fn(key);
    bool local_success = false;

    size_t bucket = mod(hashed_key, No_Buckets);
    ioopm_list_iterator_t *iter = ioopm_list_iterator(ht->buckets[bucket]);
    elem_t res = (elem_t) { .p=NULL };
    while (ioopm_iterator_has_next(iter)) {
        elem_t elem = ioopm_iterator_next(iter);
        if (ht->key_eq_fn(((key_value_pair*) elem.p)->key, key)) {
            res = ((key_value_pair*) elem.p)->value;
            free(elem.p);
            ioopm_iterator_remove(iter);
            local_success = true;
            break;
        }
    }
    ioopm_iterator_destroy(iter);
    *success = local_success;
    return res;
}

size_t ioopm_hash_table_size(ioopm_hash_table_t *ht) {
    size_t size = 0;

    for (size_t i = 0; i < No_Buckets; i++) {
        size += ioopm_linked_list_size(ht->buckets[i]);
    }

    return size;
}

bool ioopm_hash_table_is_empty(ioopm_hash_table_t *ht) {
    return !ioopm_hash_table_size(ht);
}

void ioopm_hash_table_clear(ioopm_hash_table_t *ht) {
    for (size_t i = 0; i < No_Buckets; i++) {
        destroy_key_values(ht->buckets[i]);
        ioopm_linked_list_clear(ht->buckets[i]);

    }
}

ioopm_list_t *ioopm_hash_table_keys(ioopm_hash_table_t *ht) {
    ioopm_list_t *list = ioopm_linked_list_create(ht->key_eq_fn);

    for (size_t i = 0; i < No_Buckets; i++) {
        ioopm_list_iterator_t *iter = ioopm_list_iterator(ht->buckets[i]);
        while (ioopm_iterator_has_next(iter)) {
            elem_t entry = ioopm_iterator_next(iter);
            ioopm_linked_list_append(list, ((key_value_pair*) entry.p)->key);
        }
        ioopm_iterator_destroy(iter);
    }

    return list;
}

ioopm_list_t *ioopm_hash_table_values(ioopm_hash_table_t *ht) {
    ioopm_list_t *list = ioopm_linked_list_create(ht->value_eq_fn);

    for (size_t i = 0; i < No_Buckets; i++) {
        ioopm_list_iterator_t *iter = ioopm_list_iterator(ht->buckets[i]);
        while (ioopm_iterator_has_next(iter)) {
            elem_t entry = ioopm_iterator_next(iter);
            ioopm_linked_list_append(list, ((key_value_pair*) entry.p)->value);
        }
        ioopm_iterator_destroy(iter);
    }

    return list;
}

bool ioopm_hash_table_has_key(ioopm_hash_table_t *ht, elem_t key) {
    bool res = false;
    for (size_t i = 0; i < No_Buckets; i++) {
        ioopm_list_iterator_t *iter = ioopm_list_iterator(ht->buckets[i]);
        while (ioopm_iterator_has_next(iter)) {
            elem_t entry = ioopm_iterator_next(iter);
            if (ht->key_eq_fn(((key_value_pair*) entry.p)->key, key)) {
                res = true;
                break;
            }
        }
        ioopm_iterator_destroy(iter);
    }
    return res;
}

bool ioopm_hash_table_has_value(ioopm_hash_table_t *ht, elem_t value) {
    bool res = false;
    for (size_t i = 0; i < No_Buckets; i++) {
        ioopm_list_iterator_t *iter = ioopm_list_iterator(ht->buckets[i]);
        while (ioopm_iterator_has_next(iter)) {
            elem_t entry = ioopm_iterator_next(iter);
            if (ht->value_eq_fn(((key_value_pair*) entry.p)->value, value)) {
                res = true;
                break;
            }
        }
        ioopm_iterator_destroy(iter);
    }
    return res;
}

bool ioopm_hash_table_all(ioopm_hash_table_t *ht, ioopm_predicate pred, void *arg) {
    bool res = true;
    for (size_t i = 0; i < No_Buckets; i++) {
        ioopm_list_iterator_t *iter = ioopm_list_iterator(ht->buckets[i]);
        while (ioopm_iterator_has_next(iter)) {
            elem_t entry = ioopm_iterator_next(iter);
            if (!pred(((key_value_pair*) entry.p)->key, ((key_value_pair*) entry.p)->value, arg)) {
                res = false;
                break;
            }
        }
        ioopm_iterator_destroy(iter);
    }
    return res;
}

bool ioopm_hash_table_any(ioopm_hash_table_t *ht, ioopm_predicate pred, void *arg) {
    bool res = false;
    for (size_t i = 0; i < No_Buckets; i++) {
        ioopm_list_iterator_t *iter = ioopm_list_iterator(ht->buckets[i]);
        while (ioopm_iterator_has_next(iter)) {
            elem_t entry = ioopm_iterator_next(iter);
            if (pred(((key_value_pair*) entry.p)->key, ((key_value_pair*) entry.p)->value, arg)) {
                res = true;
                break;
            }
        }
        ioopm_iterator_destroy(iter);
    }
    return res;
}

void ioopm_hash_table_apply_to_all(ioopm_hash_table_t *ht, ioopm_apply_function apply_fun, void *arg) {
    for (size_t i = 0; i < No_Buckets; i++) {
        ioopm_list_iterator_t *iter = ioopm_list_iterator(ht->buckets[i]);
        while (ioopm_iterator_has_next(iter)) {
            elem_t entry = ioopm_iterator_next(iter);
            apply_fun(((key_value_pair*) entry.p)->key, &((key_value_pair*) entry.p)->value, arg);
        }
        ioopm_iterator_destroy(iter);
    }
}
