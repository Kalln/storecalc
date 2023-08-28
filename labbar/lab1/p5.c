#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(int argc, char *argv[]){
    
    if (argc < 2 || argc > 2) {
        printf("Usage: ./a.out number\n");
        return 0;
    }
    else {
        int number = atoi(argv[1]);
        int sqrt_number = floor(sqrt(number)) + 1;


        for (int i = 2; i < sqrt_number; i++) {

            if (number % i == 0) {
                printf("%d is not a prime number\n", number);
                return 0;
            }

        }
        printf("%d is a prime number\n",  number);
        return 0;
    }
    
}