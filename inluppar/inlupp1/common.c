#include "common.h"
#include <stdbool.h>
#include <string.h>


bool int_eq(elem_t a, elem_t b)
{
    return a.data.val == b.data.val;
}

bool str_eq(elem_t a, elem_t b)
{
    return strcmp(a.data.str, b.data.str) == 0;
}

