#include <stdio.h>

int main(void){
    int count = 0;
    for (int i = 1; i <= 10; i++) {
        for (int j = 1; j <= i; j++) {
            printf("*");
            count++;
        }
        printf("\n");
    }
    printf("Totalt: %d\n", count);
    return 0;
}