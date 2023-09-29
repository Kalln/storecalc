#pragma once

#define int_elem(x) (elem_t) { .val=(x) }
#define ptr_elem(x) (elem_t) { .str=(x) }

typedef union elem
{
    char *str;
    int val;
    void *void_ptr;
} elem_t;

typedef int (*ioopm_hash_function)(elem_t key);


//// @brief Compares two integer values.
/// @param a Value1 to be compared.
/// @param b Value2 to be compared.
/// @return true if equal.
bool int_eq(elem_t a, elem_t b);

/// @brief Compares two strings.
/// @param a str1 to be compared.
/// @param b str2 to be compared.
/// @return true if equal.
bool str_eq(elem_t a, elem_t b);