#include "common.h"
#include <stdbool.h>


bool int_eq(elem_t a, elem_t b)
{
    return a.val == b.val;
}

bool str_eq(elem_t a, elem_t b)
{
    return strcmp(a.str, b.str) == 0;
}

