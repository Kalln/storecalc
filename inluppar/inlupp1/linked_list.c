#include "linked_list.h"
#include <stdlib.h>

typedef struct elem {
    elem_t *next;
    elem_t *prev;
    int value;
} elem_t;

typedef struct list {
    elem_t *first;
    elem_t *last;
    int size;
} list_t;