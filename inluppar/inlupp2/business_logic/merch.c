#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "merch.h"
#include "../datastructures/linked_list.h"
#include "../datastructures/iterator.h"
#include "../datastructures/hash_table.h"
#include "../utils/utils.h"



struct shelf {
    char *location;
    int stock;
};

struct merch {
    char *title;
    char *description;
    ioopm_list_t *shelf_list;
    int price;
    int reserved;
    int stock;
};

bool shelf_cmp(elem_t sh1, elem_t sh2) {
    char *sh1p = sh1.p;
    char *sh2p = sh2.p;

    if (sh1p[0] == sh2p[0] &&
        sh1p[1] == sh2p[1] &&
        sh1p[2] == sh2p[2]
    ) {
        return true;
    } else {
        return false;
    }
}


shelf_t *create_shelf(char *s) {
    shelf_t *new_shelf = calloc(1, sizeof(shelf_t));
    new_shelf->location = strdup(s);
    new_shelf->stock = 0;
    return new_shelf;
}


merch_t *create_merch(char *title, char *description, int price) {
    if (price < 0) {
        return NULL;
    }
    merch_t *new_merch = calloc(1, sizeof(merch_t));
    ioopm_list_t *new_linked_list = ioopm_linked_list_create(shelf_cmp);
    new_merch->title        = strdup(title);
    new_merch->description  = strdup(description);
    new_merch->price        = price;
    new_merch->stock        = 0;
    new_merch->shelf_list   = new_linked_list;
    return new_merch;
}

// func for linked list to destroy shelfs
static void destroy_shelf(size_t key, elem_t *value, void *extra) {
    shelf_t *d = (shelf_t *) value->p;
    free(d->location);
    free(d);
}

void destroy_merch(merch_t *merch) {
    free(merch->title);
    free(merch->description);
    ioopm_linked_list_apply_to_all(merch->shelf_list, destroy_shelf, NULL);
    ioopm_linked_list_destroy(merch->shelf_list);
    free(merch);
}

char *merch_title(merch_t *merch) {
    return merch->title;
}

char *merch_description(merch_t *merch) {
    return merch->description;
}

int merch_price(merch_t *merch) {
    return merch->price;
}

bool merch_edit_description(merch_t *merch_to_edit, char* new_description) {
    if (!merch_to_edit) {
        return false;
    }
    char *old_description = merch_to_edit->description;
    merch_to_edit->description = strdup(new_description);
    free(old_description);
    return true;
}

bool merch_edit_price(merch_t *merch_to_edit, int new_price) {
    if (!merch_to_edit) {
        return false;
    }
    merch_to_edit->price = new_price;
    return true;
}

ioopm_list_t *shelf_list(merch_t *merch) {
    return merch->shelf_list;
}

// The idea of this function is to "rehash" the merch into a new slot.
// That does already exist in the database. This has to be done since
// the title is used as a key in our database.
merch_t *merch_change_title(merch_t *merch, char* new_title) {
    free(merch->title);
    merch->title = strdup(new_title);
    return merch;
}


/**
 *
 * MERCH STOCK + LOCATIONS
 *
 */

int merch_get_stock(merch_t *merch) {
    return merch->stock;
}

int merch_get_reserved(merch_t *merch) {
    return merch->reserved;
}

bool merch_unreserve(merch_t *merch, int quantity) {
    if (merch->reserved < quantity) {
        return false;
    } else {
        merch->reserved -= quantity;
        return true;
    }
}

void merch_add_stock_location(merch_t *merch, char *shelf_location) {
    shelf_t *new_shelf = create_shelf(shelf_location);
    ioopm_linked_list_append(merch->shelf_list, (elem_t) { .p = new_shelf });
}

int shelf_get_stock(shelf_t *shelf) {
    return shelf->stock;
}

char *shelf_get_loc(shelf_t *shelf) {
    return shelf->location;
}

ioopm_list_t *shelf_merch_list(merch_t *merch) {
    return merch->shelf_list;
}

/// @brief Add a specified amount of stock to a merch
/// @param merch The merch to restock
/// @param amount_add The amount of stock to add
static void merch_add_stock(merch_t *merch, int amount_add){
    merch->stock += amount_add;
}

/// @brief Find a shelf if it exists in the shelf list
/// @param list_of_shelves The list of shelves
/// @param shelf_loc The location to find the shelf_t of
/// @return The shelf if found, otherwise NULL
static shelf_t *find_shelf(ioopm_list_t *list_of_shelves, char *shelf_loc) {
    ioopm_list_iterator_t *it = ioopm_list_iterator(list_of_shelves);

    while(ioopm_iterator_has_next(it)) {
        shelf_t *cursor = (shelf_t *) ioopm_iterator_next(it).p;
        if (strcmp(cursor->location, shelf_loc) == 0) {
            ioopm_iterator_destroy(it);
            return cursor;
        }

    }

    ioopm_iterator_destroy(it);
    return NULL;
}

bool shelf_add_stock(merch_t *merch, char *shelf_location, int amount_add) {
    shelf_t *found_shelf = find_shelf(merch->shelf_list, shelf_location);
    if (!found_shelf) {
        return false;
    }

    found_shelf->stock += amount_add;
    merch_add_stock(merch, amount_add);
    return true;
}

bool merch_remove_stock(ioopm_hash_table_t *shelf_ht, merch_t *merch, int amount_remove) {
    // is there enough stock to remove?
    if (merch->stock < amount_remove) {
        return false;
    }
    // we have enough stock, start removing from the first shelf until empty.
    merch->reserved -= amount_remove;
    ioopm_list_iterator_t *it = ioopm_list_iterator(merch->shelf_list);
    while (ioopm_iterator_has_next(it)) {
        shelf_t *cursor = (shelf_t *) ioopm_iterator_next(it).p;

        // we have enough items in the first shelf. Remove the stock from the shelf and merch.
        if (cursor->stock > amount_remove) {
            cursor->stock -= amount_remove;
            merch->stock -= amount_remove;
            break;
        } else {
            // we don't have enough items in the first shelf. Remove the stock we have and goto next shelf.
            amount_remove -= cursor->stock;
            merch->stock -= cursor->stock;
            cursor->stock = 0;
            bool res = false;
            ioopm_hash_table_remove(shelf_ht, (elem_t) {.p = cursor->location}, &res);
            ioopm_iterator_remove(it);
            free(cursor->location);
            free(cursor);

        }
    }
    ioopm_iterator_destroy(it);
    return true;
}

bool merch_reserve_stock(merch_t *merch, int quantity) {
    if (merch->stock < merch->reserved + quantity) {
        return false;
    } else {
        merch->reserved += quantity;
        return true;
    }
}
