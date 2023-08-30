#include <stdlib.h>
#include "utils.h"
#include <stdio.h>

int main(void) {
    int G = rand() % 1000;
    int buf_siz = 255;
    char buf[buf_siz];
    int T = 0;
    int try = 0;
    

    ask_question_string("Skriv in ditt namn:", buf, buf_siz);
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
            printf("För litet!");

        }
        else {
            printf("För stort!");

        }
        T = ask_question_int("");
    }

    return 0;
}