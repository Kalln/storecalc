#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include "utils.h"

struct item {
    char *name;
    char *description;
    int price;
    char *shelf;
};

typedef struct item item_t;

void print_item(item_t *item) {
    printf("Name: %s\nDesc: %s\nPrice: %d.%d SEK\nShelf: %s\n",
     item->name, item->description, (item->price) / 100, item->price % 100, item->shelf);
}

bool check_valid_shelf(char *shelf) {
    
    if (strlen(shelf) == 3 
        && isdigit(shelf[1]) 
        && isdigit(shelf[2])
        && isupper(shelf[0])) {
        
        return true;
    }
    return false;

}

item_t make_item(char *name, char *desc, int price, char *shelf) {
    item_t it = {.name = name, desc = desc, .price = price, .shelf = shelf};
    return it;
}

char *ask_question_shelf(char *question) {
    return ask_question(question, check_valid_shelf, (convert_func) strdup).string_value;
}

void input_item() {
    char *product = ask_question_string("Name of product: ");
    char *description = ask_question_string("Description of product:");
    int price = ask_question_int("Price of product (IN ÖRE): ");
    char *shelves = ask_question_shelf("Shelf for product: ");

    item_t item1 = make_item(product, description, price, shelves);
    print_item(&item1);

}

char* magick(char *arr1, char *arr2, char *arr3, int len) {
    char buf[255];
    srand(0);
    rand() % len
    for (int i = 0; i < len; i++) {
        srand(i * 20);
        
        /* code */
    }
    
}


int main(int argc, char const *argv[]) {

    //item_t it1 = {.name = "Test", .description = "desc123", .price = 5099, .shelf = "A25"};

    //print_item(&it1);

    input_item();
    return 0;
}
