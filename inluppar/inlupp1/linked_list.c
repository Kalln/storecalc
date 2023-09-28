#include "linked_list.h"
#include <stdlib.h>
#include <stdio.h>

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

    //TODO: free all elem in list.
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

    if (list->size == 0)
    {
        // remove dummy node...
        free(list->first);

        list->first = new_last;
        list->last = new_last;
    } else 
    {
        list->last->next = new_last;
        list->last = new_last;
    }

    list->size += 1;
}

void ioopm_linked_list_prepend(ioopm_list_t *list, int value)
{
    elem_t *new_first = element_create(value, list->first);

    // Creates new first element with specified value
    if (list->size == 0) {
        // remove dummy node...
        free(list->first);
        list->first = new_first;
        list->last = new_first; 
    } else 
    {
        list->first = new_first;
    }

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


    if(index == 0) return list->first->val; // If we are looking for the first element, we can just return it since we have the ptr.
    if(index == size-1) return list->last->val; // Same argument but for last element.

    // Cursor that will loop until the index we want. And return that value.
    elem_t *cursor = list->first->next; 
    for (int i = 0; i < index - 1; i++)
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








