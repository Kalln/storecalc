#include "linked_list.h"
#include "iterator.h"
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include "common.h"

typedef struct link link_t;

struct ioopm_list_iterator
{
    ioopm_list_t *list;
    link_t **current;
};

struct link 
{
    elem_t val;
    link_t *next;
};

struct list 
{
    link_t *first;
    link_t *last;
    size_t size;
    ioopm_eq_function eq_function;
};

/// @brief Creates an element
/// @param val value of the element
/// @param next pointer to next element
/// @return an element with value val and a pointer to the next element;
static link_t *element_create(elem_t val, link_t *next)
{
    link_t *element = calloc(1, sizeof(link_t));
    element->val = val;
    element->next = next;
    return element;
}

/// @brief Checks if a value is in the range of [0 - size], if value is not inside exits
/// @param index Value to check if it is inside the range
/// @param list_size size of the range
static void check_index_exit(int index, int list_size)
{
    if (!(index >= 0 && index <= list_size)) //We don't allow indexing outside rangde [0, size]
    {
        printf("Index: %d Index out of range, valid indexes are [0 - %d]\n", index, list_size);
        exit(1); // Exitcode 1 usually used to signify failure.
    }
}

/// @brief Free the memory of an element
/// @param elem element to be freed
static void element_destroy(link_t *elem)
{
    free(elem);
}

void ioopm_linked_list_destroy(ioopm_list_t *lt)
{

    link_t *cursor = lt->first;
    while (cursor != NULL)
    {
        link_t *next = cursor->next; // save ptr to next, before free current elem
        element_destroy(cursor); // free current elem
        cursor = next; // make next elem, current elem
    }
    free(lt);
}

ioopm_list_t *ioopm_linked_list_create(ioopm_eq_function fun)
{
    ioopm_list_t *lt = calloc(1, sizeof(ioopm_list_t));
    link_t *dummy = element_create(int_elem(0), NULL);

    // maybe do lt->first = lt->last = NULL;
    // then we can check if list is empty by checking if first is NULL
    
    lt->first = dummy;
    lt->last = dummy;
    lt->size = 0;
    lt->eq_function = fun;

    return lt;
}

void ioopm_linked_list_append(ioopm_list_t *list, elem_t value)
{
    // creates new last element with specified value
    link_t *new_last = element_create(value, NULL);
    
    list->last->next = new_last;
    list->last = new_last;

    list->size += 1;
}

void ioopm_linked_list_prepend(ioopm_list_t *list, elem_t value)
{
    // Creates new first element with specified value
    link_t *new_first = element_create(value, list->first->next);

    if (ioopm_linked_list_size(list) == 0)
    {
        list->last = new_first;
    }

    // Update first and increment size
    list->first->next = new_first;
    list->size += 1;
}

int ioopm_linked_list_size(ioopm_list_t *list)
{
    return list->size;
}

void ioopm_linked_list_insert(ioopm_list_t *list, int index, elem_t value)
{
    const size_t size = ioopm_linked_list_size(list);
    check_index_exit(index, size); //Does nothing if index is in the correct range
    

    if (index == 0)
    {
        ioopm_linked_list_prepend(list, value);
    } 
    else if (index == size)
    {
        ioopm_linked_list_append(list, value);
    } 
    else 
    {
        index++;
        link_t *cursor = list->first->next;
        link_t *prev_elem = list->first;

        for (size_t i = 0; i < index - 1; i++) //Step through the list until desired index reached
        {
            cursor = cursor->next;
            prev_elem = prev_elem->next;
        }

        link_t *indexed_element = element_create(value, cursor);
        prev_elem->next = indexed_element;
        list->size += 1;
    }
}

elem_t ioopm_linked_list_remove(ioopm_list_t *list, int index)
{
    int size = ioopm_linked_list_size(list);
    check_index_exit(index, size-1); //Does nothing if index is in the correct range

    link_t *value_to_remove = list->first->next;
    link_t *prev_value = list->first;

    for (size_t i = 0; i < index; i++) //Step through the list until desired index reached
    {
        
        value_to_remove = value_to_remove->next;
        prev_value = prev_value->next;
    }
    
    if (list->size < 1) prev_value->next = value_to_remove->next;
    if (size - 1 == index) list->last = prev_value;

    // remove element from list
    prev_value->next = value_to_remove->next;
    list->size -= 1;
    elem_t val = value_to_remove->val;
    free(value_to_remove);
    return val;
}

elem_t ioopm_linked_list_get(ioopm_list_t *list, int index)
{
    const size_t size = ioopm_linked_list_size(list);
    check_index_exit(index, size); //Does nothing if index is in the correct range


    if(index == 0) return list->first->next->val; // If we are looking for the first element, we can just return it since we have the ptr.
    if(index == size-1) return list->last->val; // Same argument but for last element.

    // Cursor that will loop until the index we want. And return that value.
    link_t *cursor = list->first->next; 
    for (size_t i = 0; i < index; i++)
    {   
        cursor = cursor->next;
    }

    return cursor->val;
}

bool ioopm_linked_list_contains(ioopm_list_t *list, elem_t element)
{

    link_t *cursor = list->first->next;
    while (cursor != NULL)
    {
        if (list->eq_function(cursor->val, element)) return true;
        cursor = cursor->next;
    }
    
    return false;
}

bool ioopm_linked_list_is_empty(ioopm_list_t *list)
{
    return list->size == 0;
}

void ioopm_linked_list_clear(ioopm_list_t *list)
{
    // Start in the first element.
    link_t *cursor = list->first->next;
    while (cursor != NULL)
    {
        // We save ptr to next element before we free the current one.
        link_t *next = cursor->next;
        free(cursor);
        cursor = next;
    }

    list->first->next = NULL;
    list->last = list->first;
    list->size = 0;
}

bool ioopm_linked_list_all(ioopm_list_t *list, ioopm_int_predicate prop, void *extra)
{
    link_t *cursor = list->first;
    elem_t NOT_USED_KEY = int_elem(0);

    while (cursor != NULL)
    {
        if (!prop(NOT_USED_KEY, cursor->val, extra)) return false;
        cursor = cursor->next;
    }

    return true;
}
bool ioopm_linked_list_any(ioopm_list_t *list, ioopm_int_predicate prop, void *extra)
{
    link_t *cursor = list->first;
    elem_t NOT_USED_KEY = int_elem(0); 

    while (cursor != NULL)
    {
        if (prop(NOT_USED_KEY, cursor->val, extra)) return true;
        cursor = cursor->next;
    }

    return false;
}

void ioopm_linked_list_apply_to_all(ioopm_list_t *list, ioopm_apply_int_function fun, void *extra)
{
    link_t *cursor = list->first;
    elem_t NOT_USED_KEY = int_elem(0); ?

    while (cursor != NULL)
    {
        fun(NOT_USED_KEY, &cursor->val, extra);
        cursor = cursor->next;
    }
}

/**
 * 
 *  ITERATOR
 * 
 */

ioopm_list_iterator_t *ioopm_list_iterator(ioopm_list_t *list)
{
    ioopm_list_iterator_t *iter = calloc(1, sizeof(ioopm_list_iterator_t));
    iter->list = list;
    iter->current = &(list->first->next);
    return iter;
}

bool ioopm_iterator_has_next(ioopm_list_iterator_t *iter)
{
    if (*(iter->current) != NULL && (*(iter->current))->next != NULL) return true;
    return false;

}

elem_t ioopm_iterator_next(ioopm_list_iterator_t *iter)
{
    // elem_t *elem = *(iter->current);
    // elem = elem->next;
    // iter->current = &elem;
    
    // Verkar inte som att den gillade att vi använde elem!
    // Fixade sig så fort vi direkt använder addressen

    iter->current = &((*(iter)->current)->next); // Ansätter current till addressen till pekaren som pekar på next

    return (*(iter)->current)->val;
}

void ioopm_iterator_destroy(ioopm_list_iterator_t *iter)
{
    free(iter);
}

elem_t ioopm_iterator_current(ioopm_list_iterator_t *iter)
{
    link_t *elem = *(iter->current);
    return elem->val;
}

void ioopm_iterator_reset(ioopm_list_iterator_t *iter)
{
    iter->current = &(iter->list->first->next);
}

elem_t ioopm_iterator_remove(ioopm_list_iterator_t *iter)
{
    link_t *elem_to_remove = *(iter->current);
    elem_t value_of_elem_remove = elem_to_remove->val;
    ioopm_iterator_reset(iter);
    
    while((*(iter->current))->next != elem_to_remove) ioopm_iterator_next(iter);
    ((*(iter->current))->next) = (elem_to_remove->next);
    ioopm_iterator_next(iter);
    
    element_destroy(elem_to_remove);

    return value_of_elem_remove;
}

void ioopm_iterator_insert(ioopm_list_iterator_t *iter, elem_t element)
{
    link_t *next_elem = *(iter->current);
    link_t *new_elem = element_create(element, next_elem);

    ioopm_iterator_reset(iter);
    while((*(iter->current))->next != next_elem) ioopm_iterator_next(iter);
    (*iter->current)->next = new_elem;
    ioopm_iterator_next(iter);    
}