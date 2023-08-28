#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]){
    // [antal rader, ökning vid varje rad]
    if (argc <= 2 || argc > 3) {
        printf("Usage: ./a.out rows growth\n");
        return 0;
    }
    else {
        int count = 0;
        int rows = atoi(argv[1]);
        int growth = atoi(argv[2]);

        for (int i = 1; i <= rows; i++) {
            for (int j = 1; j <= i * growth; j++) {
                printf("*");
                count++;
            }
            printf("\n");
        }
        printf("Totalt: %d\n", count);
        return 0;
    }
    
}