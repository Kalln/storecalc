#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "iterator.h"
#include "hash_table.h"
#include "linked_list.h"
#include "common.h"

#define Delimiters "+-#@()[]{}.,:;!? \t\n\r"

static int cmpstringp(const void *p1, const void *p2) {
    return strcmp(*(char *const *)p1, *(char *const *)p2);
}

void sort_keys(char *keys[], size_t no_keys) {
    qsort(keys, no_keys, sizeof(char *), cmpstringp);
}

void process_word(char *word, ioopm_hash_table_t *ht) {
    // FIXME: Rewrite to match your own interface, error-handling, etc.
    int freq = ioopm_hash_table_has_key(ht, (elem_t) {.p = word})
        ? (ioopm_hash_table_lookup(ht, (elem_t) {.p = word}))->i
        : 0;
    if (freq > 0) {
        ioopm_hash_table_insert(ht, (elem_t) {.p = word }, (elem_t) { .i = freq + 1});
    } else {
        ioopm_hash_table_insert(ht, (elem_t) {.p = strdup(word) }, (elem_t) { .i = freq + 1});
    }
}

void process_file(char *filename, ioopm_hash_table_t *ht) {
    FILE *f = fopen(filename, "r");

    while (true) {
        char *buf = NULL;
        size_t len = 0;
        getline(&buf, &len, f);

        if (feof(f))
        {
            free(buf);
            break;
        }

        for (char *word = strtok(buf, Delimiters);
                word && *word;
                word = strtok(NULL, Delimiters))
        {
            process_word(word, ht);
        }

        free(buf);
    }

    fclose(f);
}

int string_sum_hash(elem_t e) {
    char *str = e.p;
    int result = 0;
    do {
        result += *str;
    } while (*++str != '\0');
    return result;
}

bool string_eq(elem_t a, elem_t b) {
    return strcmp(a.p, b.p) == 0;
}

bool string_gt_eq(elem_t a, elem_t b) {
    return strcmp(a.p, b.p) >= 0;
}

bool int_eq(elem_t a, elem_t b) {
    return a.i == b.i;
}

int main(int argc, char *argv[]) {
    ioopm_hash_table_t *ht = ioopm_hash_table_create((ioopm_hash_function)string_sum_hash, string_gt_eq, string_eq, int_eq);

    if (argc > 1) {
        for (int i = 1; i < argc; ++i) {
            process_file(argv[i], ht);
        }

        ioopm_list_t *keys_list = ioopm_hash_table_keys(ht);
        char **keys = calloc(ioopm_hash_table_size(ht), sizeof(char**));
        size_t index = 0;
        ioopm_list_iterator_t *iter = ioopm_list_iterator(keys_list);
        while (ioopm_iterator_has_next(iter)) {
            keys[index++] = (char *) ioopm_iterator_next(iter).p;
        }
        ioopm_iterator_destroy(iter);

        int size = ioopm_hash_table_size(ht);
        sort_keys(keys, size);

        for (int i = 0; i < size; ++i) {
            int freq = (ioopm_hash_table_lookup(ht, (elem_t) {.p = keys[i]}))->i;
            printf("%s: %d\n", keys[i], freq);
        }

        ioopm_linked_list_destroy(keys_list);
        free(keys);
    } else {
        puts("Usage: freq-count file1 ... filen");
    }

    ioopm_list_t *keys_list = ioopm_hash_table_keys(ht);
    ioopm_list_iterator_t *iter = ioopm_list_iterator(keys_list);

    while (ioopm_iterator_has_next(iter)) {
        free(ioopm_iterator_next(iter).p);
    }

    ioopm_iterator_destroy(iter);
    ioopm_linked_list_destroy(keys_list);
    ioopm_hash_table_destroy(ht);
}
