#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int gcd(int a, int b) {
    if (a == b) {
        return a;
    }
    else if (a > b) {
        return gcd(a - b, b);
    } 
    else if (a < b) {
        return gcd(a, b - a);
    }
}

int main(int argc, char *argv[]){
    
    if (argc < 3 || argc > 3) {
        printf("Usage: ./a.out number1 number2\n");
        return 0;
    }
    else {
        int a = atoi(argv[1]);
        int b = atoi(argv[2]);
        int res = gcd(a, b);

        printf("gcd(%d, %d) = %d\n", a, b, res);
        return 0;
    }
    
}

