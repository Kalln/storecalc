#include "common.h"
#include <stdbool.h>
#include <string.h>


bool int_eq(elem_t a, elem_t b)
{
    if (a.type == ELEM_V_PTR || b.type == ELEM_V_PTR)
    {
        return false;
    }
    return a.data.val == b.data.val;
}

bool str_eq(elem_t a, elem_t b)
{
    if (a.type == ELEM_V_PTR || b.type == ELEM_V_PTR)
    {
        return false;
    }
    return strcmp(a.data.str, b.data.str) == 0;
}

