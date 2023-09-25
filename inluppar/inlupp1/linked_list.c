#include "linked_list.h"
#include <stdlib.h>

typedef struct elem elem_t;


struct elem 
{
    int val;
    elem_t *next;
};

struct list 
{
    elem_t *first;
    elem_t *last;
    int size;
};






