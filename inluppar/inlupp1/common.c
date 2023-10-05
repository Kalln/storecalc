#include "common.h"
#include <stdbool.h>
#include <string.h>


bool int_eq(elem_t a, elem_t b)
{
    return (a.data.void_ptr) == NULL && (b.data.void_ptr) == NULL ? true
         : (a.data.void_ptr) == NULL || (b.data.void_ptr) == NULL ? false
         : a.data.val == b.data.val;
}

bool str_eq(elem_t a, elem_t b)
{
    return a.data.str == NULL && b.data.str == NULL ? true
         : a.data.str == NULL || b.data.str == NULL ? false
         : strcmp(a.data.str, b.data.str) == 0;
}

