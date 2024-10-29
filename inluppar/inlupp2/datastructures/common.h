#pragma once

typedef union {
    int i;
    unsigned int u;
    bool b;
    float f;
    void *p;
} elem_t;

/// Compares two elements and returns true if they are equal
typedef bool(*ioopm_eq_function)(elem_t a, elem_t b);

/// Returns true if a is greater than or equal to b
typedef bool(*ioopm_gr_or_eq_function)(elem_t a, elem_t b);
