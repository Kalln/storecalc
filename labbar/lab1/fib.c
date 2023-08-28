#include <stdio.h>
#include <stdlib.h>

int fib(int val) {
    if (val == 0) {
        printf("0 ");
    }
    else if (val == 1) {
        printf("0 1 ");
    }
    else {
    int a = 0;
    int b = 1;
    int sum = 0;
    printf("0 1 ");
    for (int i = 0; i < val - 2; i++) {
        sum = a + b;
        a = b;
        b = sum;
        printf("%d ", b);
    }

    }
    printf("\n");
}

int main(int argc, char *argv[]){

    fib(atoi(argv[1]));

    return 0;
}


