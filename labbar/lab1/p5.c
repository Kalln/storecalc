#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]){
    // [antal rader, ökning vid varje rad]
    if (argc < 2 || argc > 2) {
        printf("Usage: ./a.out number\n");
        return 0;
    }
    else {
        int number = atoi(argv[1]);

        for (int i = 2; i <= number; i++) {
            for (int j = 2; j <= number; j++) {
                if (i * j == number) {
                    printf("%d is not a prime number\n", number);
                    return 0;
                }
            }

        }
        printf("%d is a prime number\n", number);
        return 0;
    }
    
}