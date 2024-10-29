/**
 * @file database.h
 * @author Karl Widborg Kortelainen and Mauritz Hamrin Sverredal
 * @date 2023-11-06
 * @brief An interface for the database
 */

#include "../datastructures/hash_table.h"
#include "merch.h"

/// @brief A database containing merch items and carts.
typedef struct database database_t;

/// @brief Create a new database
/// @return The newly created database
database_t *create_database(void);

/// @brief Destroy a database and free the associated memory
/// @param db The database to destroy
void destroy_database(database_t *db);

/// @brief Create and add a merch item to the database
/// @param db The database
/// @param title The title of the merch
/// @param desc The description of the merch
/// @param price The price of the merch
/// @return true if successful, false otherwise
bool database_add_merch(database_t *db, char* title, char* desc, int price);

/// @brief Add an item to the database with quantity 0
/// @param db The database
/// @param merch_to_add The merch to add
/// @return true if successful, false if an item with the same title is already in the DB
bool database_add_item(database_t *db, merch_t *merch_to_add);

/// @brief Will remove merch, this function is called after the user has confirmed the removal of the merch.
/// @param db merch database.
/// @param merch_to_remove the merch to remove.
/// @return true if merch was succesfully removed, false otherwise.
bool database_remove_merch(database_t *db, merch_t *merch_to_remove);

/// @brief Gets the amount of merch items in the database
/// @param db The database
/// @return The number of merch items in the DB
int database_size(database_t *db);

/// @brief given a db and a key (title of merch) it will return the merch item
/// @param db database_t
/// @param key char* title of merch
/// @return merch_t* the merch item or NULL if it does not exist.
merch_t *get_merch_by_key(database_t *db, char* key);

/// @brief Add an empty cart to the database
/// @param db The database
/// @return The id of the newly added cart
int database_add_cart(database_t *db);

/// @brief Try to remove a cart from the database
/// @param db The database
/// @param id The id of the cart to remove
/// @return true if the cart was removed successfully, false otherwise
bool database_remove_cart(database_t *db, int id);

/// @brief Perform a checkout with a cart, removing the items from stock and then deleting the cart.
/// @param db The database
/// @param id The id of the cart to checkout
/// @return true if successful, false if unable to check out
bool database_checkout_cart(database_t *db, int id);

/// @brief Add a specified quantity of an item to a cart. The specified quantity needs to be available to be reserved.
/// @param db The database
/// @param cart_id The id of the cart
/// @param product_title The title of the product to add
/// @param quantity The quantity to add to the cart
/// @return true if successful, false otherwise
bool database_add_item_to_cart(database_t *db, int cart_id, char *product_title, int quantity);

/// @brief Remove a specified quantity of an item from a cart.
/// @param db The database
/// @param cart_id The id of the cart
/// @param product_title The title of the product to remove
/// @param quantity The quantity to remove from the cart
/// @return true if successful, false otherwise
bool database_remove_item_from_cart(database_t *db, int cart_id, char *product_title, int quantity);

/// @brief Calculate the total cost of all items in a cart
/// @param db The database
/// @param id The id of the cart
/// @return The total cost of the items in the cart or -1 if the cart does not exist
long long database_calculate_cart_cost(database_t *db, int id);

/// @brief Change the description of an item
/// @param merch_to_edit The merch to edit
/// @param new_description The new description
/// @return true if successful, false otherwise
bool database_edit_description(merch_t *merch_to_edit, char* new_description);

/// @brief Change the price of an item
/// @param merch_to_edit The merch to edit
/// @param new_price The new price
/// @return true if successful, false otherwise
bool database_edit_price(merch_t *merch_to_edit, int new_price);

/// @brief Change the title of an item
/// @param db The database containing the item
/// @param merch_to_edit The merch to edit
/// @param new_title The new title
/// @return true if successful, false otherwise
bool database_edit_title(database_t *db, merch_t *merch_to_edit, char* new_title);

/// @brief gets the price of a given merch
/// @param merch merch to get price of
/// @return int of the price of merch
int database_get_price(merch_t *merch);

/// @brief gets the title of a merch.
/// @param merch merch to get the price of
/// @return string of the title of merch.
char *database_get_title(merch_t *merch);

/// @brief gets the description of a merch.
/// @param merch merch to get the description of
/// @return string of the description of merch.
char *database_get_description(merch_t *merch);

/// @brief Reserve stock in the database. It is only possible to reserve stock that is available.
/// It's not possible to reserve more stock than available.
/// @param db database_t
/// @param merch_title title of the merch that reservation is to be done.
/// @param reserve_amount {int} how many items should be reserved. If the amount is greater than
/// available stock then it will return false.
/// @return true if reservation was successful, otherwise false.
bool database_reserve_item_stock(database_t *db, char *merch_title, int reserve_amount);

/// @brief Checkout item in database. THIS IS ONLY SUPPOSED TO BE USED WHEN RESERVATION HAS BEEN DONE.
/// @param db database_t
/// @param merch_title title of the merch that checkout is executed on.
/// @param amount amount that will be bought.
/// @return This should always return true, it should not be possible to checkout unreserved items.
bool database_checkout_item(database_t *db, char *merch_title, int amount);

/// @brief Add a shelf to a merch item
/// @param db database_t
/// @param merch the merch item to add a shelf to
/// @param shelf_to_add the shelf to add
/// @return true if successful, false otherwise
bool database_add_shelf(database_t *db, merch_t *merch, char *shelf_to_add);

/// @brief adds stock to a given shelf
/// @param db database_t
/// @param merch merch to add stock to
/// @param shelf the shelf to add stock to
/// @param restock_amount amount to add stock to shelf to.
/// @return true if successful, or false if unsuccessful.
bool database_add_stock_to_shelf(database_t *db, merch_t *merch, char *shelf, int restock_amount);

/// @brief get the available stock. total stock - reserved stock = available stock
/// @param merch merch to get available stock from
/// @return a int with the available stock
int database_get_available_stock(merch_t *merch);

/// @brief check if a shelf exists in the database.
/// @param db database to check if shelf exists in.
/// @param shelf shelf to check if exists in db.
/// @return true if exists, or false if not.
bool shelf_exists(database_t *db, char *shelf);

/// @brief gets the hash table of the database.
/// @param db database to get hashtable of. This will get the shelf hashtable.
/// @return shelf hashtable of the database.
ioopm_hash_table_t *database_get_shelf_ht(database_t *db);

/// @brief replenish stock of a merch item. Works even if the shelf does not exist.
/// @param db database that contains the merch item.
/// @param merch merch item to replenish stock of.
/// @param shelf what shelf to add stock to.
/// @param quantity amount to stock up the shelf with.
/// @return true if success, or false if operation failed.
bool replenish_stock(database_t *db, merch_t *merch, char* shelf, int quantity);

/// @brief get the merch hashtable of the database.
/// @param db database to get the merch hashtable from
/// @return hash_table of the merch items.
ioopm_hash_table_t *database_get_items(database_t *db);

/// @brief will get the shelf list of a merch item.
/// @param merch merch to get the shelfs from
/// @return linked list with all the shelfs that the merch item has.
ioopm_list_t *database_shelf_list(merch_t *merch);

/// @brief Check if a cart with the given id exists in the database
/// @param db The database
/// @param id The id of the cart
/// @return true if the cart exists, false otherwise
bool database_cart_id_valid(database_t *db, int id);
