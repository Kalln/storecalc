/**
 * @file merch.h
 * @author Karl Widborg Kortelainen and Mauritz Hamrin Sverredal
 * @date 2023-11-06
 * @brief Operations related to shopping merch and shelves
 */

#include "../datastructures/linked_list.h"
#include "../datastructures/hash_table.h"

/// @brief A struct representing a merch item in the database
typedef struct merch merch_t;
typedef struct shelf shelf_t;

/// @brief Create a merch item with the given attributes.
/// The item should be destroyed with destroy_merch()
/// when it is no longer in use.
/// @param title The title of the item
/// @param description The description of the item
/// @param price The price of the item (must be positive)
/// @return A pointer to the created item. Ownership of
/// the item is transferred to the caller.
/// If the merch could not be created, returns NULL
merch_t *create_merch(char *title, char *description, int price);

/// @brief Destroys a merch_t item
/// @param merch The merch to destroy.
void destroy_merch(merch_t *merch);

/// @brief Change the title of a given merch item. This will destroy the memory old merch, and create a new one.
/// @param old_merch the merch_t to change the title of.
/// @param new_title new title that the old_merch should change to.
/// @return the new merch_t ptr with the new title.
merch_t *merch_change_title(merch_t *old_merch, char* new_title);

/// @brief Gets the title of a merch item
/// @param merch The item to get the title of
/// @return The title of the item
char *merch_title(merch_t *merch);

/// @brief Gets the description of a merch item
/// @param merch The item to get the description of
/// @return The description of the item
char *merch_description(merch_t *merch);

/// @brief Gets the price of a merch item
/// @param merch The item to get the price of
/// @return The price of the item
int merch_price(merch_t *merch);

/// @brief Set the price of a merch item
/// @param merch_to_edit The merch item to edit
/// @param new_price The value to set the price to
/// @return true/false (success/failure)
bool merch_edit_price(merch_t *merch_to_edit, int new_price);

/// @brief Set the description of a merch item
/// @param merch_to_edit The merch item to edit
/// @param new_description The new description to set
/// @return true/false (success/failure)
bool merch_edit_description(merch_t *merch_to_edit, char* new_description);

/// @brief gets the stock of a given merch.
/// @param merch merch to get the stock from.
/// @return integer of the stock.
int merch_get_stock(merch_t *merch);

/// @brief Gets the location of a shelf
/// @param shelf the shelf to get the location of
/// @return location ex. 'A33', K43'.
char *shelf_get_loc(shelf_t *shelf);

/// @brief gets the stock of a shelf.
/// @param shelf the shelf to check the stock of.
/// @return integer of the stock.
int shelf_get_stock(shelf_t *shelf);

/// @brief Add location to a merch item. ex. 'A33', K44'. NOTE! this does not to error checking, and has to be done before.
/// @param merch the merch to add the location to.
/// @param shelf_location a string in format 'A33', 'K44'.
void merch_add_stock_location(merch_t *merch, char *shelf_location);

/// @brief gets the linked list of shelfs of a merch item.
/// @param merch the merch to get the shelfs of.
/// @return a linked list of shelfs that contains both location and stock of that shelf.
ioopm_list_t *shelf_merch_list(merch_t *merch);

/// @brief adds stock to a specific shelf.
/// @param merch the merch to add stock to, where the shelf is located.
/// @param shelf_location  a string in format 'A33', 'K44'.
/// @param amount_add a int that is the amount to add to the shelf.
/// @return true if the stock was added, false otherwise.
bool shelf_add_stock(merch_t *merch, char *shelf_location, int amount_add);

/// @brief removes stock from the shelf. This will always remove the stock from the first non-empty
/// shelf in the list.
/// @param merch merch to remove stock from.
/// @param amount_remove quanitty to remove from stock.
/// @return true if the stock was removed, false otherwise.
bool merch_remove_stock(ioopm_hash_table_t *shelf_ht, merch_t *merch, int amount_remove);

/// @brief Try to reserve a specified amount of a merch.
/// @param merch The merch to reserve
/// @param quantity The quantity to reserve
/// @return true if the amount could be reserved, false otherwise
bool merch_reserve_stock(merch_t *merch, int quantity);

/// @brief gets the given reserved stock of a merch.
/// @param merch
/// @return int
int merch_get_reserved(merch_t *merch);

/// @brief Try to unreserve a specified amount of a merch.
/// @param merch The merch to unreserve
/// @param quantity The quantity to unreserve
/// @return true if the amount could be unreserved, false otherwise
bool merch_unreserve(merch_t *merch, int quantity);

/// @brief Compare two shelves for equality
/// @param sh1 The first shelf
/// @param sh2 The second shelf
/// @return true if the shelves are equal, false otherwise
bool shelf_cmp(elem_t sh1, elem_t sh2);

/// @brief gets the shelf list of a given merch.
/// @param merch merch to get the list from
/// @return lineked list of the shelf list.
ioopm_list_t *shelf_list(merch_t *merch);
