#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "../datastructures/hash_table.h"
#include "../datastructures/linked_list.h"
#include "../datastructures/iterator.h"

#include "../business_logic/database.h"
#include "../utils/utils.h"


static bool ask_for_confirmation(char* question) {
    return ask_question_menu(question) == 'Y';
}

static bool ask_for_non_confirmation(char* question) {
    return ask_question_menu(question) == 'N';
}

static void merch_add_user(database_t *db) {
    char *new_merch_title = ask_question_string("Enter title of product:");
    char *new_merch_desc = ask_question_string("Enter description:");
    int new_merch_price = ask_question_int("Enter price:");
    if (!database_add_merch(db, new_merch_title, new_merch_desc, new_merch_price)) {
        printf("Unable to create merch\n");
    }
    free(new_merch_title);
    free(new_merch_desc);
}

static void create_cart(database_t *db) {
    int id = database_add_cart(db);
    printf("New cart id: %d\n", id);
}

static void remove_cart(database_t *db) {
    int id = ask_question_int("Enter cart id:");
    if (database_cart_id_valid(db, id)) {
        printf("Do you want to remove cart %d?", id);
        if (ask_for_confirmation(" (Y/N)\n")) {
            printf("Removing cart %d\n", id);
            if (!database_remove_cart(db, id)) {
                printf("Unable to remove cart %d\n", id);
            }
        }
    } else {
        printf("Invalid cart id %d\n", id);
    }

}

static void calculate_cost(database_t *db) {
    int id = ask_question_int("Enter cart id:");
    long long cost = database_calculate_cart_cost(db, id);
    if (cost != -1) {
        printf("Cost: %lld,%lld\n", cost / 100, cost % 100);
    } else {
        printf("Invalid cart id %d\n", id);
    }

}


static void checkout_cart(database_t *db) {
    int id = ask_question_int("Enter cart id:");
    if(!database_cart_id_valid(db, id)) {
        printf("Invalid cart ID: %d\n", id);
        return;
    }
    long long cost = database_calculate_cart_cost(db, id);
    bool success = database_checkout_cart(db, id);
    if (!success) {
        printf("Unable to checkout cart %d\n", id);
    }

    printf("Checkout success, total price: %lld,%lld\n", cost / 100, cost % 100);
}

static void add_to_cart(database_t *db) {
    int id = ask_question_int("Enter cart id:");
    char *merch_title = ask_question_string("Enter title of product:");
    int quantity =  ask_question_int("Enter quantity:");
    if (!database_add_item_to_cart(db, id, merch_title, quantity)) {
        printf("Unable to add item %s to cart %d\n", merch_title, id);
    }
    free(merch_title);
}

static void remove_from_cart(database_t *db) {
    int id = ask_question_int("Enter cart id:");
    char *merch_title = ask_question_string("Enter title of product:");
    int quantity =  ask_question_int("Enter quantity:");
    database_remove_item_from_cart(db, id, merch_title, quantity);
    free(merch_title);
}

static void print_menu(void) {
    printf("Choose option:\n");
    printf("[A]dd merch\n");
    printf("[L]ist merch\n");
    printf("[D]elete merch\n");
    printf("[E]dit merch\n");
    printf("[S]how stock\n");
    printf("Re[P]lenish stock\n");
    printf("[C]reate cart\n");
    printf("[R]emove cart\n");
    printf("[+]Add to cart\n");
    printf("[-]Remove from cart\n");
    printf("[=]Calculate cart cost\n");
    printf("Check[O]ut\n");
    printf("[U]ndo\n");
    printf("[-]Remove from cart\n");
}

static void shelf_print(ioopm_list_t *ls) {
    ioopm_list_iterator_t *it = ioopm_list_iterator(ls);
    while (ioopm_iterator_has_next(it)) {
        shelf_t *shelf = ioopm_iterator_next(it).p;
        printf("%s -> %d\n", shelf_get_loc(shelf), shelf_get_stock(shelf));
    }
    ioopm_iterator_destroy(it);

}

static void merch_print(merch_t *merch) {
    printf("------------------------------\n");
    printf("Title: %s\n", (merch_title(merch)));
    printf("Description: %s\n", (merch_description(merch)));
    printf("Shelves (total: %d):\n", merch_get_stock(merch));
    shelf_print(shelf_list(merch));
    int price = merch_price(merch);
    printf("Price: %d,%d\n", price / 100, price % 100);
    printf("------------------------------\n");
}

static void print_edit_menu(void) {
    printf("Choose option:\n");
    printf("Edit [T]itle\n");
    printf("Edit [D]escription\n");
    printf("Edit [P]rice\n");
    printf("[B]ack\n");
}

static merch_t *get_merch(database_t *db) {
    char *merch_to_get = ask_question_string("Current title of the product:");
    merch_t *found_merch = get_merch_by_key(db, merch_to_get);
    free(merch_to_get);
    return found_merch;
}

static void interface_edit_title(database_t *db) {
    merch_t *found_merch = get_merch(db);
    if (found_merch) {
        char *new_title = ask_question_string("Enter the new title of chosen title.");
        // change title
        if (database_edit_title(db, found_merch, new_title)) {
            printf("New name: %s\n", new_title);
        }
        free(new_title);
    }
    else {
        printf("That product does not exist in the database. Please add it before trying to edit it.\n");
    }
}

static void interface_edit_description(database_t *db) {
    merch_t *found_merch = get_merch(db);
    if(found_merch) {
        char *new_desc = ask_question_string("Enter the new description");

        if (database_edit_description(found_merch, new_desc)) {
            printf("New desc: %s\n", new_desc);
        }
        free(new_desc);
    }
    else {
        printf("There are no such product. Please add it before trying to change description.");
    }
}

static void interface_edit_price(database_t *db) {
    merch_t *found_merch = get_merch(db);
    if (found_merch) {
        int new_price = ask_question_int("Enter the new price:");

        if (database_edit_price(found_merch, new_price)) {
            printf("New price is: %d", new_price);
        }
        printf("failed\n");
    }
    else {
        printf("There are no such product. Please add it before trying to change price.");
    }
    return;

}

static void edit_merch(database_t *db) {
    print_edit_menu();
    char val = ask_question_menu("");
    switch (val)
    {
    case 'T':
        interface_edit_title(db);
        break;
    case 'D':
        interface_edit_description(db);
        break;
    case 'P':
        interface_edit_price(db);
        break;
    case 'B':
        break;

    default:
       edit_merch(db);
    }
}



static void show_stock(database_t *db) {
    merch_t *merch = get_merch(db);
    if (!merch) {
        printf("Couldn't find the specified merch\n");
        return;
    }
    printf("Total: %d\n", merch_get_stock(merch));
    ioopm_list_t *list_shelf = database_shelf_list(merch);
    ioopm_list_iterator_t *it =  ioopm_list_iterator(list_shelf);
    while (ioopm_iterator_has_next(it)) {
        shelf_t *shelf = ioopm_iterator_next(it).p;
        printf("Shelf: %s -> %d stock\n", shelf_get_loc(shelf), shelf_get_stock(shelf));
    }
}

static void remove_merch(database_t *db) {
    merch_t *merch = get_merch(db);
    if (!merch) {
        printf("Couldn't find the specified merch\n");
        return;
    }
    if (database_remove_merch(db, merch)) {
        printf("Successfully removed merch\n");
    }
    else {
        printf("Failed to remove merch\n");
    }
}

static void user_replenish_stock(database_t *db) {
    merch_t *merch = get_merch(db);

    if (!merch) {
        printf("Couldn't find the specified merch\n");
        return;
    }

    char *shelf = ask_question_shelf("Enter shelf");
    int quantity = ask_question_int("Enter amount");

    if (!replenish_stock(db, merch, shelf, quantity)) {
        printf("Unable to replenish quantity %d of %s\n", quantity, merch_title(merch));
    }
    free(shelf);
}

static int sort_fn(const void *a, const void *b) {
    return strcmp(merch_title(*(merch_t **) a), merch_title(*(merch_t **) b));
}

static void list_merch(database_t *db) {
    ioopm_list_t *items = ioopm_hash_table_values(database_get_items(db));
    size_t len = ioopm_linked_list_size(items);
    merch_t **arr = calloc(len, sizeof(merch_t *));

    ioopm_list_iterator_t *iter = ioopm_list_iterator(items);
    for (size_t i = 0; i < len; ++i) {
        arr[i] = ioopm_iterator_next(iter).p;
    }
    ioopm_iterator_destroy(iter);
    ioopm_linked_list_destroy(items);

    qsort(arr, len, sizeof(merch_t *), sort_fn);

    int cnt = 1; // Counts to 20, then resets
    for (size_t i = 0; i < len; ++i, ++cnt) {
        merch_print(arr[i]);
        if (cnt == 20 && i + 1 < len) {
            if (ask_for_non_confirmation("Continue? (Y/N)\n")){
                goto cleanup;
            }
            cnt = 0; // Incremented to 1 immediately
        }
    }

cleanup:
    free(arr);
}

void event_loop(database_t *db) {
    while (true) {
        print_menu();
        char action = ask_question_menu("");
        switch (action) {
            case 'A': { // Add merch
                merch_add_user(db);
                break;
            }
            case 'L': { // List Merchandise
                list_merch(db);
                break;
            }
            case 'D': { // Remove Merchandise
                remove_merch(db);
                break;
            }
            case 'E': { // Edit Merchandise
                edit_merch(db);
                break;
            }
            case 'S': { // Show Stock
                show_stock(db);
                break;
            }
            case 'P': { // Replenish
                user_replenish_stock(db);
                break;
            }
            case 'C': { // Create Cart
                create_cart(db);
                break;
            }
            case 'R': { // Remove Cart
                remove_cart(db);
                break;
            }
            case '+': { // Add To Cart
                add_to_cart(db);
                break;
            }
            case '-': { // Remove from Cart
                remove_from_cart(db);
                break;
            }
            case '=': { // Calculate Cost
                calculate_cost(db);
                break;
            }
            case 'O': { // Checkout
                checkout_cart(db);
                break;
            }
            case 'U': { // Undo

                break;
            }
            case 'Q': { // Quit
                if (ask_for_confirmation("Exit? (Y/N)")) {
                    return;
                }
                break;
            }
            default: {
                break;
            }
        }
    }
}
