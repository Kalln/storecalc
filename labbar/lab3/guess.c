#include <stdlib.h>
#include "utils.h"
#include <stdio.h>

int main(void) {
    int G = rand() % 1000;

    char *buf;
    int T = 0;
    int try = 0;
    

    buf = ask_question_string("Skriv in ditt namn:");
    printf("Du %s jag tänker på ett tal ... kan du gissa vilket?", buf);
    T = ask_question_int("");

    while (true) {
        try++;
        if (try == 15) {
            printf("Nu har du slut gissningar! Jag tänkte på %d\n", G);
            break;
        }
        if (T == G) {
            printf("Bingo!\nDet tog %s %d gissingar att komma fram till %d\n", buf, try, G);
            break;
        }
        else if (T < G) {
            printf("För litet!\n");

        }
        else {
            printf("För stort!\n");

        }
        
        T = ask_question_int("Enter a number: ");
    }

    return 0;
}