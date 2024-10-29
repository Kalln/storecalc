/**
 * @file cart.h
 * @author Karl Widborg Kortelainen and Mauritz Hamrin Sverredal
 * @date 2023-11-06
 * @brief Operations related to shopping carts
 */

#include <stddef.h>

#include "merch.h"

/// @brief A struct representing a shopping cart
typedef struct cart cart_t;

/// @brief A struct containing different carts
typedef struct cart_db cart_db_t;
/// @brief An item stored in a cart (merch and amount)
typedef struct cart_item cart_item_t;

/// @brief Create a new cart with the given id
/// @param id The id of the cart
/// @return The newly created cart
cart_t *cart_create(int id);

/// @brief Destroy a cart
/// @param cart The cart to destroy
void cart_destroy(cart_t *cart);

/// @brief Create a new, empty, cart database
/// @return The new database
cart_db_t *cart_db_create(void);

/// @brief Destroy a cart database and all carts contained within
/// @param cart_db The database to destroy
void cart_db_destroy(cart_db_t *cart_db);

/// @brief Add a new cart to a cart DB, with a unique id
/// @param cart_db The cart DB
/// @return The id of the new cart
int cart_db_add_cart(cart_db_t *cart_db);

/// @brief Try to remove a cart from a cart db and destroy it
/// @param cart_db The cart DB
/// @param id The id of the cart to remove
/// @return true if the cart was successfully removed, false otherwise
bool cart_db_remove_cart(cart_db_t *cart_db, int id);

/// @brief Get the amount of carts in a cart DB
/// @param db The cart DB
/// @return The amount of carts in the DB
size_t cart_db_size(cart_db_t *db);

/// @brief Try to add a given quantity of a merch to a cart
/// @param cart The cart to add the merch to
/// @param quantity The quantity to add
/// @param merch The merch to add
/// @return true if the item was successfully added, false otherwise
bool cart_add_item(cart_t *cart, int quantity, merch_t *merch);

/// @brief Remove a given quantity of a merch from a cart
/// @param cart The cart to remove merch from
/// @param quantity The quantity to remove
/// @param merch The merch to remove
/// @return true if the item was successfully removed, false otherwise
bool cart_remove_item(cart_t *cart, int quantity, merch_t *merch);

/// @brief Calculate the cost of all items in a cart
/// @param cart The cart to get the cost of
/// @return The combined cost of all items in the cart
long long cart_calculate_cost(cart_t *cart);

/// @brief Get the merch from a cart item
/// @param cart_item The cart item
/// @return The merch associated with the cart item
merch_t *cart_item_get_merch(cart_item_t *cart_item);

/// @brief Get the amount of a cart item
/// @param cart_item The cart item
/// @return The amount of a cart item
int cart_item_get_amount(cart_item_t *cart_item);

/// @brief Try to get a cart from the cart db by id
/// @param db The cart database
/// @param id The id of the requested cart
/// @return The cart with the given id, or NULL if no cart with the given id exists.
cart_t *cart_db_get_cart_by_id(cart_db_t *db, int id);

/// @brief Get a list of the items in the cart of the given id
/// @param db The cart database
/// @param id The id of the requested cart
/// @return A list of items in the cart with the given id, or NULL if no cart with the given id exists.
ioopm_list_t *cart_db_list_items_by_id(cart_db_t *db, int id);

/// @brief Check if there exists a cart with the given id in the database
/// @param db The database
/// @param id The id of the cart
/// @return true if there is a cart, false otherwise
bool cart_db_cart_id_valid(cart_db_t *db, int id);
