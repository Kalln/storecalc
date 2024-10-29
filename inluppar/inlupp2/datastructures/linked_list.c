
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "linked_list.h"
#include "iterator.h"

/**
 * @file linked_list.c
 * @author Elias Swanberg and Mauritz Hamrin Sverredal
 * @date 2023-09-18
 * @brief A simple linked list
 */

// The links of the linked list
struct link {
    elem_t value;
    struct link *next;
};

typedef struct link link_t;

link_t *link_create(elem_t value, link_t *next) {
    link_t *link = calloc(1, sizeof(link_t));
    assert(link);
    link->value = value;
    link->next = next;
    return link;
}

// The list contains pointers to its first and last link, its size, and a pointer to an equality function
struct list {
    link_t *head;
    link_t *last;
    size_t size;
    ioopm_eq_function eq_fn;
};

struct iter {
  link_t *current;
  ioopm_list_t *list;
};

ioopm_list_iterator_t *ioopm_list_iterator(ioopm_list_t *list) {
    ioopm_list_iterator_t *result = calloc(1, sizeof(ioopm_list_iterator_t));

    result->current = list->head;
    result->list = list;

    return result;
}

void ioopm_iterator_destroy(ioopm_list_iterator_t *iter) {
    free(iter);
}

bool ioopm_iterator_has_next(ioopm_list_iterator_t *iter) {
    return iter->current->next;
}

elem_t ioopm_iterator_next(ioopm_list_iterator_t *iter) {
    iter->current = iter->current->next;
    return iter->current->value;
}

static link_t *iter_get_previous(ioopm_list_iterator_t *iter) {
    assert(iter->current != iter->list->head);
    link_t *prev = iter->list->head;
    while (prev->next != iter->current) {
        prev = prev->next;
    }
    return prev;
}

elem_t ioopm_iterator_remove(ioopm_list_iterator_t *iter) {
    link_t *to_remove = iter->current;
    iter->current = iter_get_previous(iter);
    iter->current->next = to_remove->next;
    if (to_remove == iter->list->last) {
        iter->list->last = iter->current;
    }
    elem_t res = to_remove->value;
    free(to_remove);
    --iter->list->size;
    return res;
}

void ioopm_iterator_insert(ioopm_list_iterator_t *iter, elem_t element) {
    link_t *new = link_create(element, iter->current->next);
    iter->current->next = new;
    if (iter->current == iter->list->last) {
        iter->list->last = new;
    }
    ++iter->list->size;
}

void ioopm_iterator_reset(ioopm_list_iterator_t *iter) {
    iter->current = iter->list->head;
}

elem_t ioopm_iterator_current(ioopm_list_iterator_t *iter) {
    return iter->current->value;
}


ioopm_list_t *ioopm_linked_list_create(ioopm_eq_function eq_fn) {
    ioopm_list_t *list = calloc(1, sizeof(struct list));
    assert(list);
    list->head = link_create((elem_t) { .i=(0) }, NULL); // Sentinel element
    list->last = list->head;
    list->size = 0;
    list->eq_fn = eq_fn;
    return list;
}

void ioopm_linked_list_destroy(ioopm_list_t *list) {
    assert(list);
    link_t *current = list->head;
    while (current) {
        link_t *tmp = current;
        current = current->next;
        free(tmp);
    }
    free(list);
}

void ioopm_linked_list_append(ioopm_list_t *list, elem_t value) {
    list->last->next = link_create(value, NULL);
    list->last = list->last->next;
    ++list->size;
}

void ioopm_linked_list_prepend(ioopm_list_t *list, elem_t value) {
    ioopm_linked_list_insert(list, 0, value);
}

static void insert_helper(ioopm_list_t *list, link_t *link, size_t index, elem_t value) {
    if (index == 0) {
        link->next = link_create(value, link->next);
        if (link == list->last) {
            list->last = link->next;
        }
    } else {
        insert_helper(list, link->next, index - 1, value);
    }
}

void ioopm_linked_list_insert(ioopm_list_t *list, size_t index, elem_t value) {
    insert_helper(list, list->head, index, value);
    ++list->size;
}

elem_t ioopm_linked_list_remove(ioopm_list_t *list, size_t index) {
    assert(list);
    assert(list->head);

    link_t *prev = list->head;
    link_t *current = prev->next;
    for (size_t i = 0; i < index; i++) {
        assert(current);
        prev = current;
        current = current->next;
    }

    prev->next = current->next;
    if (current == list->last) {
        list->last = prev;
    }
    elem_t value = current->value;
    free(current);
    list->size--;
    return value;
}

elem_t ioopm_linked_list_get(ioopm_list_t *list, size_t index) {
    assert(list);
    assert(list->head);
    assert(list->head->next);
    link_t *current = list->head->next;
    for (size_t i = 0; i < index; i++) {
        assert(current);
        current = current->next;
    }
    return current->value;
}

bool ioopm_linked_list_contains(ioopm_list_t *list, elem_t element) {
    link_t *elem = list->head->next; // step past sentinel element

    while (elem) {
        if (list->eq_fn(elem->value, element)) {
            return true;
        }

        elem = elem->next;
    }

    return false;
}

size_t ioopm_linked_list_size(ioopm_list_t *list) {
    assert(list);
    return list->size;
}

bool ioopm_linked_list_is_empty(ioopm_list_t *list) {
    return !list->size;
}

void ioopm_linked_list_clear(ioopm_list_t *list) {
    while (!ioopm_linked_list_is_empty(list)) {
        ioopm_linked_list_remove(list, 0);
    }
}

static bool all_helper(link_t *elem, ioopm_elem_predicate prop, void *extra, size_t key) {
    return !elem
        || (
            prop(key, elem->value, extra)
            && all_helper(elem->next, prop, extra, key + 1)
        );
}

bool ioopm_linked_list_all(ioopm_list_t *list, ioopm_elem_predicate prop, void *extra) {
    return all_helper(list->head->next, prop, extra, 0);
}


static bool any_helper(link_t *elem, ioopm_elem_predicate prop, void *extra, size_t key) {
    return elem
        && (
            prop(key, elem->value, extra)
            || any_helper(elem->next, prop, extra, key + 1)
        );
}

bool ioopm_linked_list_any(ioopm_list_t *list, ioopm_elem_predicate prop, void *extra) {
    return any_helper(list->head->next, prop, extra, 0);
}

static void apply_helper(link_t *elem, ioopm_apply_elem_function fun, void *extra, size_t key) {
    if (elem) {
        fun(key, &elem->value, extra);
        apply_helper(elem->next, fun, extra, key + 1);
    }
}

void ioopm_linked_list_apply_to_all(ioopm_list_t *list, ioopm_apply_elem_function fun, void *extra) {
    apply_helper(list->head->next, fun, extra, 0);
}
