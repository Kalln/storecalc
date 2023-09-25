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

void ioopm_linked_list_append(ioopm_list_t *list, int value)
{
    // creates new last entry with specified value
    elem_t *new_last;
    new_last->next = NULL;
    new_last->val = value;

    // Update last and increment size
    list->last->next = new_last;
    list->last = new_last;
    list->size += 1;
}







