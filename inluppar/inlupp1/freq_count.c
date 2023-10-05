#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "hashtable.h"
#include "linked_list.h"
#include "common.h"
#include "iterator.h"
#define Delimiters "+-#@()[]{}.,:;!? \t\n\r"

void free_all_elements(elem_t key, elem_t *value, void *xtra) {
    if (key.type == ELEM_V_PTR || key.type == ELEM_STR) {
        free(key.data.str);
    }
}

static int cmpstringp(const void *p1, const void *p2)
{
    return strcmp(*(char *const *)p1, *(char *const *)p2);
}

void sort_keys(char *keys[], size_t no_keys)
{
    qsort(keys, no_keys, sizeof(char *), cmpstringp);
}

void process_word(char *word, ioopm_hash_table_t *ht)
{
    int freq =
        ioopm_hash_table_has_key(ht, ptr_elem(word)) ?
        (ioopm_hash_table_lookup(ht, ptr_elem(word))).value.data.val :
        0 ;

    if (freq != 0)
    {
        ioopm_hash_table_insert(ht, ptr_elem(word), int_elem(freq + 1));
    } else
    {
    ioopm_hash_table_insert(ht, ptr_elem(strdup(word)), int_elem(freq + 1));
    }
}

void process_file(char *filename, ioopm_hash_table_t *ht)
{
    FILE *f = fopen(filename, "r");
    while (true)
    {
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

int string_sum_hash(elem_t e)
{
    char *str = e.data.str;
    int result = 0;
    do
    {
        result += *str;
    }
    while (*++str != '\0');
    return abs(result);
}

bool string_eq(elem_t e1, elem_t e2)
{
    return (strcmp(e1.data.str, e2.data.str) == 0);
}


int main(int argc, char *argv[])
{
    ioopm_hash_table_t *ht = ioopm_hash_table_create((ioopm_hash_function) string_sum_hash, (ioopm_eq_function) str_eq, (ioopm_eq_function) int_eq);

    if (argc > 1)
    {
        for (int i = 1; i < argc; ++i)
        {
            process_file(argv[i], ht);
        }
        // FIXME: If the keys are returned as a list, transfer them into
        // an array to use `sort_keys` (perhaps using an iterator?)

        ioopm_list_t *key_list = ioopm_hash_table_keys(ht);
        ioopm_list_iterator_t *key_iter = ioopm_list_iterator(key_list);

        int size = ioopm_hash_table_size(ht);
        char *keys[size];
        //char **keys = calloc(size, sizeof(char *));
        //char **keys = ioopm_hash_table_keys(ht);
        keys[0] = ioopm_iterator_current(key_iter).data.str;

        for (size_t i = 1; i < size-1; i++)
        {
            keys[i] = ioopm_iterator_next(key_iter).data.str;
        }
    
        sort_keys(keys, size);
        for (int i = 1; i < size; ++i)
        {
            // FIXME: Update to match your own interface, error handling, etc.
            //int freq = (ioopm_hash_table_lookup(ht, (elem_t) {.data.str = keys[i]})).value.data.val;
            int freq = ioopm_hash_table_lookup(ht, ptr_elem(keys[i])).value.data.val;
            printf("%s: %d\n", keys[i], freq);
        }
        ioopm_iterator_destroy(key_iter);
        //ioopm_apply_function
        ioopm_linked_list_destroy(key_list);
        
    }
    else
    {
        puts("Usage: freq-count file1 ... filen");
    }
    // FIXME: Leaks memory! Use valgrind to find out where that memory is
    // being allocated, and then insert code here to free it.
    
    ioopm_hash_table_apply_to_all(ht, free_all_elements, NULL);
    ioopm_hash_table_destroy(ht);
    return 0;
}