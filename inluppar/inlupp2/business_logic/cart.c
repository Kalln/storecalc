#include <stdlib.h>

#include "../datastructures/linked_list.h"
#include "../datastructures/hash_table.h"
#include "../datastructures/iterator.h"
#include "cart.h"
#include "merch.h"

struct cart {
    int id;
    ioopm_list_t *contents;
};

struct cart_item {
    int quantity;
    merch_t *merch;
};

struct cart_db {
    int next_id;
    ioopm_hash_table_t *carts;
};

cart_t *cart_create(int id) {
    cart_t *cart = calloc(1, sizeof(cart_t));
    cart->id = id;
    cart->contents = ioopm_linked_list_create(NULL);
    return cart;
}

void destroy_cart_item(cart_item_t *item) {
    free(item);
}

/// @brief Destroy a cart item
/// @param index Unused
/// @param value The elem_t of the cart item to destroy
/// @param extra Unused
static void destroy_cart_item_apply(size_t index, elem_t *value, void* extra) {
    destroy_cart_item(value->p);
}

void cart_destroy(cart_t *cart) {
    ioopm_linked_list_apply_to_all(cart->contents, destroy_cart_item_apply, NULL);
    ioopm_linked_list_destroy(cart->contents);
    free(cart);
}

merch_t *cart_item_get_merch(cart_item_t *cart_item) {
    return cart_item->merch;
}

int cart_item_get_amount(cart_item_t *cart_item) {
    return cart_item->quantity;
}

bool cart_add_item(cart_t *cart, int quantity, merch_t *merch) {
    bool success = false;
    if (!merch_reserve_stock(merch, quantity)) {
        success = false;
        return success;
    }
    ioopm_list_iterator_t *iter = ioopm_list_iterator(cart->contents);
    while (ioopm_iterator_has_next(iter)) {
        cart_item_t *item = ioopm_iterator_next(iter).p;
        if (item->merch == merch) {
            item->quantity += quantity;
            goto cleanup;
        }
    }
    cart_item_t *new_item = calloc(1, sizeof(cart_item_t));
    new_item->merch = merch;
    new_item->quantity = quantity;
    ioopm_linked_list_append(cart->contents, (elem_t) { .p = new_item });

cleanup:
    ioopm_iterator_destroy(iter);
    return true;
}

bool cart_remove_item(cart_t *cart, int quantity, merch_t *merch) {
    bool success = false;
    ioopm_list_iterator_t *iter = ioopm_list_iterator(cart->contents);
    while (ioopm_iterator_has_next(iter)) {
        cart_item_t *item = ioopm_iterator_next(iter).p;
        if (item->merch == merch) {
            if (merch_unreserve(merch, quantity)) {
                item->quantity -= quantity;
                if (item->quantity == 0) {
                    ioopm_iterator_remove(iter);
                    destroy_cart_item(item);
                }
                success = true;
            }
            break;
        }
    }

    ioopm_iterator_destroy(iter);
    return success;
}

long long cart_calculate_cost(cart_t *cart) {
    long long cost = 0;
    ioopm_list_iterator_t *iter = ioopm_list_iterator(cart->contents);
    while (ioopm_iterator_has_next(iter)) {
        cart_item_t *item = ioopm_iterator_next(iter).p;
        cost += item->quantity * merch_price(item->merch);
    }
    ioopm_iterator_destroy(iter);
    return cost;
}

/// @brief Hash function for db keys
/// @param key the key
/// @return The hash of the key
static int key_hash_fn(elem_t key) {
    return key.i;
}

/// @brief Equality function for db keys
/// @param a The first key
/// @param b The second key
/// @return true if the elements are identical, false otherwise
static bool key_eq_fn(elem_t a, elem_t b) {
    return a.i == b.i;
}

/// @brief Compare two database keys
/// @param a The first key
/// @param b The second key
/// @return true if a >= b, false otherwise
static bool key_gt_eq_fn(elem_t a, elem_t b) {
    return a.i >= b.i;
}

int cart_db_add_cart(cart_db_t *cart_db) {
    int id = cart_db->next_id++;
    cart_t *cart = cart_create(id);
    ioopm_hash_table_insert(cart_db->carts, (elem_t) { .i = id }, (elem_t) { .p = cart });
    return id;
}

bool cart_db_remove_cart(cart_db_t *cart_db, int id) {
    bool success = false;
    elem_t elem = ioopm_hash_table_remove(cart_db->carts, (elem_t) { .i = id }, &success);
    if (success) {
        cart_destroy(elem.p);
    }
    return success;
}

cart_db_t *cart_db_create(void) {
    cart_db_t *cart_db = calloc(1, sizeof(cart_db_t));
    cart_db->next_id = 1;
    cart_db->carts = ioopm_hash_table_create(key_hash_fn, key_gt_eq_fn, key_eq_fn, NULL);
    return cart_db;
}

/// @brief Destroy a cart
/// @param key Unused
/// @param value The elem_t of the cart to destroy
/// @param extra Unused
static void destroy_cart_apply(elem_t key, elem_t *value, void* extra) {
    cart_destroy(value->p);
}

void cart_db_destroy(cart_db_t *db) {
    ioopm_hash_table_apply_to_all(db->carts, destroy_cart_apply, NULL);
    ioopm_hash_table_destroy(db->carts);
    free(db);
}

size_t cart_db_size(cart_db_t *db) {
    return ioopm_hash_table_size(db->carts);
}

cart_t *cart_db_get_cart_by_id(cart_db_t *db, int id) {
    elem_t *elem = ioopm_hash_table_lookup(db->carts, (elem_t) { .i = id });
    if (!elem) {
        return NULL;
    } else {
        return elem->p;
    }
}

ioopm_list_t *cart_db_list_items_by_id(cart_db_t *db, int id) {
    cart_t *cart = cart_db_get_cart_by_id(db, id);
    if (!cart) {
        return NULL;
    } else {
        return cart->contents;
    }
}

bool cart_db_cart_id_valid(cart_db_t *db, int id) {
    return ioopm_hash_table_lookup(db->carts, (elem_t) { .i = id }) != NULL;
}
