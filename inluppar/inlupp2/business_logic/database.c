#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>


#include "../utils/utils.h"
#include "../datastructures/hash_table.h"
#include "../datastructures/iterator.h"
#include "database.h"
#include "cart.h"
#include "merch.h"

struct database {
    ioopm_hash_table_t *table;
    ioopm_hash_table_t *shelves;
    cart_db_t *carts;
};

int database_size(database_t *db) {
    return ioopm_hash_table_size(db->table);
}


static int string_hash_function(elem_t key) {
    return strlen(key.p);
}

static bool string_eq(elem_t a, elem_t b) {
    return !strcmp(a.p, b.p);
}

static bool string_gt_eq(elem_t a, elem_t b) {
    return strcmp((char *) a.p, (char *) b.p) >=  0;
}

static bool merch_eq(elem_t a, elem_t b) {
    return a.p == b.p;
}

database_t *create_database(void) {
    database_t *db = calloc(1, sizeof(database_t));
    db->table = ioopm_hash_table_create(string_hash_function, string_gt_eq, string_eq, merch_eq);
    db->shelves = ioopm_hash_table_create(string_hash_function, string_gt_eq, string_eq, NULL);
    db->carts = cart_db_create();
    return db;
}

void destroy_key_and_merch(elem_t key, elem_t *value, void *extra) {
    destroy_merch((merch_t *) value->p);
}

void destroy_database(database_t *db) {
    ioopm_hash_table_apply_to_all(db->table, destroy_key_and_merch, NULL);
    ioopm_hash_table_destroy(db->table);
    ioopm_hash_table_destroy(db->shelves);
    cart_db_destroy(db->carts);
    free(db);
}

int database_add_cart(database_t *db) {
    return cart_db_add_cart(db->carts);
}

bool database_remove_cart(database_t *db, int id) {
    return cart_db_remove_cart(db->carts, id);
}

bool database_add_item(database_t *db, merch_t *merch_to_add) {
    elem_t merch_key = (elem_t) { .p = merch_title(merch_to_add) };

    if (ioopm_hash_table_has_key(db->table, merch_key)) {
        return false;
    } else {
        ioopm_hash_table_insert(db->table, merch_key, (elem_t) { .p=merch_to_add});
        return true;
    }
}

ioopm_hash_table_t *database_get_shelf_ht(database_t *db) {
    return db->shelves;
}

bool database_add_merch(database_t *db, char* title, char* desc, int price) {
    merch_t *merch = create_merch(title, desc, price);
    if (!merch) {
        return false;
    }
    bool response = database_add_item(db, merch);
    // in case item already exists, we have to delete the merch we created.
    if (!response) {
        destroy_merch(merch);
    }
    return response;
}

ioopm_hash_table_t *database_get_items(database_t *db) {
    return db->table;
}

long long database_calculate_cart_cost(database_t *db, int id) {
    cart_t *cart = cart_db_get_cart_by_id(db->carts, id);

    if (!cart) {
        return -1;
    }

    return cart_calculate_cost(cart);
}

merch_t *get_merch_by_key(database_t *db, char* key) {
    elem_t merch_key = (elem_t) { .p=key };
    elem_t *merch = ioopm_hash_table_lookup(db->table, merch_key);
    if (merch) {
        return (merch_t *) merch->p;
    } else {
        return NULL;
    }
}

bool database_remove_merch(database_t *db, merch_t *merch_to_remove) {
    bool response = false;

    if (!merch_to_remove) {
        return response;
    }
    if (merch_get_reserved(merch_to_remove) != 0) {
        return response;
    }

    elem_t merch_key = (elem_t) { .p = merch_title(merch_to_remove) };

    ioopm_hash_table_remove(db->table, merch_key, &response);
    if (response) {
        destroy_merch(merch_to_remove);
    }

    return response;
}

/**
 *
 * EDIT A MERCH
 *
*/

bool database_edit_title(database_t *db, merch_t *merch_to_edit, char* new_title) {
    bool res = false;
    if (!merch_to_edit) {
        return res;
    }
    ioopm_hash_table_remove(db->table, (elem_t) { .p = merch_title(merch_to_edit)}, &res);
    if (res) {
        merch_t *new_merch = merch_change_title(merch_to_edit, new_title);
        database_add_item(db, new_merch);
    }
    return res;
}

bool database_edit_price(merch_t *merch_to_edit, int new_price) {
    return !merch_to_edit ? false : merch_edit_price(merch_to_edit, new_price);
}

bool database_edit_description(merch_t *merch_to_edit, char* new_description) {
    return !merch_to_edit ? false : merch_edit_description(merch_to_edit, new_description);
}

int database_get_price(merch_t *merch) {
    return merch_price(merch);
}

char *database_get_title(merch_t *merch) {
    return merch_title(merch);
}

char *database_get_description(merch_t *merch) {
    return merch_description(merch);
}

ioopm_list_t *database_shelf_list(merch_t *merch) {
    return shelf_list(merch);
}
/**
 *
 * ADD SHELF, ADD STOCK TO SHELF, AVAILABLE STOCK
 *
*/

bool shelf_exists(database_t *db, char *shelf) {
    elem_t shelf_key = (elem_t) { .p = shelf };
    return ioopm_hash_table_has_key(db->shelves, shelf_key);
}

bool database_add_shelf(database_t *db, merch_t *merch, char *shelf_to_add) {
    // If the merch doesn't exist, return false.
    if (!merch) {
        return false;
    } else if (shelf_exists(db, shelf_to_add)) {
        // If the shelf already exists, return false.
        return false;
    } else if (  // Checks that the shelf has the correct format ex. "A33".
        strlen(shelf_to_add) == 3
        && isupper(shelf_to_add[0])
        && isdigit(shelf_to_add[1])
        && isdigit(shelf_to_add[2])
    ) {
        // If the shelf doesn't exist, add it to the merch.
        ioopm_hash_table_insert(db->shelves, (elem_t) { .p = shelf_to_add }, (elem_t) { .p = merch_title(merch) });
        merch_add_stock_location(merch, shelf_to_add);
        return true;
    } else {
        // format is wrong, return false.
        return false;
    }
}

bool database_add_stock_to_shelf(database_t *db, merch_t *merch, char *shelf, int restock_amount) {
    // If the merch doesn't exist, return false.
    if (!merch) {
        return false;
    }

    // if we try to add a negative amount of stock. It should not be possible to remove stock in add stock func.
    else if (restock_amount < 0) {
        return false;
    }

    // Try to add stock, if it succeeds return true.
    return shelf_add_stock(merch, shelf, restock_amount);
}

int database_get_available_stock(merch_t *merch) {
    return (merch_get_stock(merch) - merch_get_reserved(merch));
}

/**
 *
 * RESERVE & STOCK MERCH
 *
*/

/// @brief Find an item in the database by title
/// @param db The database
/// @param product_title The title of the product
/// @return A pointer to the merch item if found, otherwise NULL
static merch_t *find_item(database_t *db, char *product_title) {
    if (!product_title) {
        return NULL;
    }
    bool item_exists = ioopm_hash_table_has_key(db->table, (elem_t) { .p = product_title });
    return item_exists ? get_merch_by_key(db, product_title) : NULL;
}

bool database_add_item_to_cart(database_t *db, int cart_id, char *product_title, int quantity) {
    merch_t *merch = find_item(db, product_title);
    if (!merch) {
        return false;
    } else {
        cart_t *cart = cart_db_get_cart_by_id(db->carts, cart_id);
        if (cart && cart_add_item(cart, quantity, merch)) {
            return true;
        } else {
            return false;
        }
    }
}

bool database_remove_item_from_cart(database_t *db, int cart_id, char *product_title, int quantity) {
    merch_t *merch = find_item(db, product_title);
    if (!merch) {
        return false;
    } else {
        cart_t *cart = cart_db_get_cart_by_id(db->carts, cart_id);
        // check if cart exists
        if (!cart) {
            return false;
        }
        else if (cart_remove_item(cart, quantity, merch)) {
            return true;
        } else {
            return false;
        }
        return true;
    }
}

bool database_reserve_item_stock(database_t *db, char *merch_title, int reserve_amount) {
    merch_t *merch_exists = find_item(db, merch_title);
    return merch_exists ? merch_reserve_stock(merch_exists, reserve_amount) : false;
}

// TA BORT RESERVARTION

bool database_checkout_item(database_t *db, char *merch_title, int amount) {
    merch_t *merch_exists = find_item(db, merch_title);
    return merch_exists ? merch_remove_stock(db->shelves, merch_exists, amount) : false;
}

bool database_checkout_cart(database_t *db, int id) {

    if (!database_cart_id_valid(db, id)) { return false; }
    bool success = true;

    ioopm_list_t *list = cart_db_list_items_by_id(db->carts, id);
    if (!list) {
        return false;
    } else {
        ioopm_list_iterator_t *iter = ioopm_list_iterator(list);
        while (ioopm_iterator_has_next(iter)) {
            cart_item_t *item = ioopm_iterator_next(iter).p;
            merch_t *merch = cart_item_get_merch(item);
            int quantity = cart_item_get_amount(item);
            if (!database_checkout_item(db, merch_title(merch), quantity)) {
                success = false;
            }
        }
        ioopm_iterator_destroy(iter);
        database_remove_cart(db, id);
        return success;
    }
}


bool replenish_stock(database_t *db, merch_t *merch, char* shelf, int quantity) {
    if (!merch || quantity <= 0) { return false; }

    // if shelf exists, we want to check if the merch is the same as the merch we want to add to.
    if (shelf_exists(db, shelf)) {
        // check if the current merch is equal to the merch we want to add to.
        elem_t *item_on_shelf = ioopm_hash_table_lookup(database_get_shelf_ht(db), (elem_t) { .p = shelf });
        merch_t *merch_on_shelf = (merch_t *) item_on_shelf;

        if (strcmp(merch_title(merch_on_shelf), merch_title(merch)) == 0) {
            // shelves are equal and merch is the same. We can add stock to the shelf.
            return database_add_stock_to_shelf(db, merch, shelf, quantity);
        } else {
            // we are trying to add stock to a shelf that already has another merch on it.
            return false;
        }
    }
    // shelf does not exist. We can add the shelf and add stock to it.
    else {
        return database_add_shelf(db, merch, shelf)
        ? database_add_stock_to_shelf(db, merch, shelf, quantity)
        : false;
    }
}

bool database_cart_id_valid(database_t *db, int id) {
    return cart_db_cart_id_valid(db->carts, id);
}
