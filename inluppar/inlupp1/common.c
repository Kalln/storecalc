#include "common.h"
#include <stdbool.h>
#include <string.h>


bool int_eq(elem_t a, elem_t b)
{
    return a.data.val == NULL && b.data.val == NULL ? true
         : a.data.val == NULL || b.data.val == NULL ? false
         : a.data.val == b.data.val;
}

bool str_eq(elem_t a, elem_t b)
{
    return a.data.str == NULL && b.data.str == NULL ? true
         : a.data.str == NULL || b.data.str == NULL ? false
         : strcmp(a.data.str, b.data.str) == 0;
}

