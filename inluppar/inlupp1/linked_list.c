#include "linked_list.h"
#include "iterator.h"
#include <stdlib.h>
#include <stdio.h>

typedef struct elem elem_t;

struct ioopm_list_iterator
{
    ioopm_list_t *list;
    elem_t **current;
};

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

/// @brief Creates an element
/// @param val value of the element
/// @param next pointer to next element
/// @return an element with value val and a pointer to the next element;
static elem_t *element_create(int val, elem_t *next)
{
    elem_t *element = calloc(1, sizeof(elem_t));
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
static void element_destroy(elem_t *elem)
{
    free(elem);
}

void ioopm_linked_list_destroy(ioopm_list_t *lt)
{

    elem_t *cursor = lt->first;
    while (cursor != NULL)
    {
        elem_t *next = cursor->next; // save ptr to next, before free current elem
        element_destroy(cursor); // free current elem
        cursor = next; // make next elem, current elem
    }
    free(lt);
}
ioopm_list_t *ioopm_linked_list_create()
{
    ioopm_list_t *lt = calloc(1, sizeof(ioopm_list_t));
    elem_t *dummy = element_create(0, NULL);

    // maybe do lt->first = lt->last = NULL;
    // then we can check if list is empty by checking if first is NULL
    
    lt->first = dummy;
    lt->last = dummy;
    lt->size = 0;

    return lt;
}

void ioopm_linked_list_append(ioopm_list_t *list, int value)
{
    // creates new last element with specified value
    elem_t *new_last = element_create(value, NULL);
    
    list->last->next = new_last;
    list->last = new_last;

    list->size += 1;
}

void ioopm_linked_list_prepend(ioopm_list_t *list, int value)
{
    // Creates new first element with specified value
    elem_t *new_first = element_create(value, list->first->next);

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

void ioopm_linked_list_insert(ioopm_list_t *list, int index, int value)
{
    int size = ioopm_linked_list_size(list);
    check_index_exit(index, size); //Does nothing if index is in the correct range
    list->size += 1;

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
        elem_t *cursor = list->first->next;
        elem_t *prev_elem = list->first;

        for (int i = 0; i < index - 1; i++) //Step through the list until desired index reached
        {
            cursor = cursor->next;
            prev_elem = prev_elem->next;
        }

        elem_t *indexed_element = element_create(value, cursor);
        prev_elem->next = indexed_element;
    }
}

int ioopm_linked_list_remove(ioopm_list_t *list, int index)
{
    int size = ioopm_linked_list_size(list);
    check_index_exit(index, size-1); //Does nothing if index is in the correct range

    elem_t *value_to_remove = list->first;
    elem_t *prev_value = list->first;

    if (list->size == 1) // We need a special case for when we only have one element in the list.
    {
        elem_t *dummy = element_create(0, NULL);
        
        list->first = dummy;
        list->last = dummy;
    }

    for (int i = 0; i < index; i++) //Step through the list until desired index reached
    {
        value_to_remove = value_to_remove->next;
        prev_value = prev_value->next;
    }
    
    if (list->size < 1) prev_value->next = value_to_remove->next;

    list->size -= 1;
    int val = value_to_remove->val;
    free(value_to_remove);
    return val;
}

int ioopm_linked_list_get(ioopm_list_t *list, int index)
{
    int size = ioopm_linked_list_size(list);
    check_index_exit(index, size-1); //Does nothing if index is in the correct range


    // elem_t *value_to_check = list->first->next;
    //int i = 0;
    //while ((i++) != index) value_to_check = value_to_check->next;
    //return value_to_check->val;


    if(index == 0) return list->first->next->val; // If we are looking for the first element, we can just return it since we have the ptr.
    if(index == size-1) return list->last->val; // Same argument but for last element.

    // Cursor that will loop until the index we want. And return that value.
    elem_t *cursor = list->first->next; 
    for (int i = 0; i < index; i++)
    {   
        cursor = cursor->next;
    }

    return cursor->val;
}

bool ioopm_linked_list_contains(ioopm_list_t *list, int element)
{
    elem_t *cursor = list->first;
    while (cursor != NULL)
    {
        if (cursor->val == element) return true;
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
    elem_t *cursor = list->first;
    while (cursor != NULL)
    {
        // We save ptr to next element before we free the current one.
        elem_t *next = cursor->next;
        free(cursor);
        cursor = next;
    }
    elem_t *dummy = element_create(0, NULL);
        
    list->first = dummy;
    list->last = dummy;
    list->size = 0;
}

bool ioopm_linked_list_all(ioopm_list_t *list, ioopm_int_predicate prop, void *extra)
{
    elem_t *cursor = list->first;
    int NOT_USED_KEY = 0; // TODO: Maybe it will be more clear what key does later..?

    while (cursor != NULL)
    {
        if (!prop(NOT_USED_KEY, &cursor->val, extra)) return false;
        cursor = cursor->next;
    }

    return true;
}
bool ioopm_linked_list_any(ioopm_list_t *list, ioopm_int_predicate prop, void *extra)
{
    elem_t *cursor = list->first;
    int NOT_USED_KEY = 0; // TODO: Maybe it will be more clear what key does later..?

    while (cursor != NULL)
    {
        if (prop(NOT_USED_KEY, &cursor->val, extra)) return true;
        cursor = cursor->next;
    }

    return false;
}

void ioopm_linked_list_apply_to_all(ioopm_list_t *list, ioopm_apply_int_function fun, void *extra)
{
    elem_t *cursor = list->first;
    int NOT_USED_KEY = 0; // TODO: Maybe it will be more clear what key does later..?

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

int ioopm_iterator_next(ioopm_list_iterator_t *iter)
{
    elem_t *elem = *(iter->current);
    elem = elem->next;
    iter->current = &elem;

    return elem->val;
}

void ioopm_iterator_destroy(ioopm_list_iterator_t *iter)
{
    free(iter);
}

int ioopm_iterator_current(ioopm_list_iterator_t *iter)
{
    elem_t *elem = *(iter->current);
    return elem->val;
}

void ioopm_iterator_reset(ioopm_list_iterator_t *iter)
{
    iter->current = &(iter->list->first->next);
}

int ioopm_iterator_remove(ioopm_list_iterator_t *iter)
{
    elem_t *elem_to_remove = *(iter->current);
    int value_of_elem_remove = elem_to_remove->val;
    ioopm_iterator_reset(iter);
    
    while((*(iter->current))->next != elem_to_remove) ioopm_iterator_next(iter);
    (*(iter->current))->next = &(elem_to_remove->next);
    ioopm_iterator_next(iter);
    
    element_destroy(elem_to_remove);

    return value_of_elem_remove;
}

void ioopm_iterator_insert(ioopm_list_iterator_t *iter, int element)
{
    // TODO: IF TIME
    return;
}