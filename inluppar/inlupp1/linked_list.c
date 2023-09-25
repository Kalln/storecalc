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


void ioopm_linked_list_destroy(ioopm_list_t *lt)
{

    //TODO: free all elem in list.
    free(lt);
}
ioopm_list_t *ioopm_linked_list_create()
{
    ioopm_list_t *lt = calloc(1, sizeof(ioopm_list_t));
    lt->size = 0;

    return lt;
}








