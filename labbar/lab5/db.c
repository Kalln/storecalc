#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "utils.h"
#include <time.h>

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

item_t make_item(char *name, char *desc, int price, char *shelf) {
    item_t it = {.name = name, desc = desc, .price = price, .shelf = shelf};
    return it;
}

item_t input_item() {
    char *product = ask_question_string("Name of product: ");
    char *description = ask_question_string("Description of product:");
    int price = ask_question_int("Price of product (IN ÖRE): ");
    char *shelves = ask_question_shelf("Shelf for product: ");

    item_t item1 = make_item(product, description, price, shelves);
    return item1;

}

char *magick(char **arr1, char **arr2, char **arr3, int len) {
    char buf[255];
    int pick[3];
    char *p = &buf[0]; // points at current writing pos

    // seed random number
    for (int i = 0; i < 3; i++) {
        srand(i * time(NULL));
        pick[i] = random() % len;
    }
    char *str1 = arr1[pick[0]];
    char *str2 = arr2[pick[1]];
    char *str3 = arr3[pick[2]];
    
    //Generate first word of the product name
    int i = 0; 
    while (str1[i] != '\0') {
        *p = str1[i];
        i++;
        p++;
    }
    *p = '-';
    p++;

    // generate second word of the product name.
    i = 0; 
    while (str2[i] != '\0') {
        *p = str2[i];
        i++;
        p++;
    }
    *p = '-';
    p++;

    // generate thrid word of the product name.
    i = 0; 
    while (str3[i] != '\0') {
        *p = str3[i];
        i++;
        p++;
    }
    *p = '\0';

    return strdup(buf);
    
}

void list_db(item_t *items, int no_items) {
    for (int i = 0; i < no_items; i++) {
        printf("%d. %s\n", i + 1, items[i].name);
    }
}

void edit_db(item_t *items, int no_items) {

    list_db(items, no_items);
    int edit = 0;
    do {
        edit = (ask_question_int("Välj en vara du vill ändra på [1-16]: ")) - 1;
    }
    while (1 > edit || edit > 16);
    
    print_item(&items[edit]);
    item_t new_item = input_item();
    items[edit] = new_item;
    return;
}

int main(int argc, char *argv[]) {
  char *array1[] = { "Liten", "Stark", "Snabb", "Glad",
        "Vacker", "Tyst", "Rolig", "Smart",
        "Lugn", "Färgglad", "Vänlig", "Läcker",
        "Ljus", "Hög", "Modig", "Ljuvlig" }; 
  char *array2[] = { "Bok", "Hund", "Bil", "Sol",
        "Träd", "Hav", "Månad", "Stol",
        "Hus", "Dator", "Mjölk", "Banan",
        "Blomma", "Penna", "Sten", "Sjö" }; 
  char *array3[] = { "Mjölnare", "Fjärrkontroll", "Tidning", "Klocka",
        "Väska", "Fjäll", "Soffa", "Mobiltelefon",
        "Katt", "Sång", "Lampa", "Kamera",
        "Skor", "Resväska", "Dröm", "Kaffe" }; 

  if (argc < 2) {
    printf("Usage: %s number\n", argv[0]);
  }
  else {
    item_t db[16]; // Array med plats för 16 varor
    int db_siz = 0; // Antalet varor i arrayen just nu

    int items = atoi(argv[1]); // Antalet varor som skall skapas

    if (items > 0 && items <= 16) {
      for (int i = 0; i < items; ++i) {
        // Läs in en vara, lägg till den i arrayen, öka storleksräknaren
        item_t item = input_item();
        db[db_siz] = item;
        ++db_siz;
      }
    }
    else {
      puts("Sorry, must have [1-16] items in database.");
      return 1; // Avslutar programmet!
    }

    for (int i = db_siz; i < 16; ++i) {
        char *name = magick(array1, array2, array3, 16); 
        char *desc = magick(array1, array2, array3, 16); 
        int price = random() % 200000;
        char shelf[] = { random() % ('Z'-'A') + 'A',
                         random() % 10 + '0',
                         random() % 10 + '0',
                         '\0' };
        item_t item = make_item(name, desc, price, shelf);

        db[db_siz] = item;
        ++db_siz;
      }

     // Skriv ut innehållet
     list_db(db, db_siz);
    edit_db(db, db_siz);
    list_db(db, db_siz);
  }
  return 0;
}
