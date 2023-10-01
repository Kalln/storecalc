#pragma once
#include <stdbool.h>

#define int_elem(x) (elem_t) { .data.val=(x),       .type=ELEM_INT }
#define ptr_elem(x) (elem_t) { .data.str=(x),       .type=ELEM_STR }
#define void_elem(x) (elem_t) { .data.void_ptr=(x), .type=ELEM_V_PTR }

typedef struct elem elem_t;

/// @brief INT -> 0, STR -> 1, V_PTR -> 2
enum elem_type 
{
    ELEM_INT,
    ELEM_STR,
    ELEM_V_PTR
};


struct elem {
    enum elem_type type;
    union {
        char *str;
        int val;
        void *void_ptr;
    } data;
};

// typedef union elem
// {
//     char *str;
//     int val;
//     void *void_ptr;
    
// } elem_t;


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