#include "linked_list.h"
#include "iterator.h"


typedef elem_t iter_t;


typedef struct
{
    int value;
    elem_t *next;
} elem_t;

struct ioopm_list_iterator
{
    ioopm_list_t *list;
    elem_t *current_elem;
    int current;
};


ioopm_list_iterator_t *ioopm_list_iterator(ioopm_list_t *list)
{
    ioopm_list_iterator_t *iter = calloc(1, sizeof(ioopm_list_iterator_t));
    iter->current = 0;
    iter->current_elem = ioopm_linked_list_get(list, iter->current);
    return iter;
}

bool ioopm_iterator_has_next(ioopm_list_iterator_t *iter)
{
    if (ioopm_linked_list_get(iter, iter->current + 1) != NULL)
}