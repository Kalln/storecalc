#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

bool is_number(char *str) {
    int len = strlen(str);
    if (str[0] == '-' && str[1] == '\0') {
        return false;
    }
    for (int i = str[0] == '-'; i < len; i++) {
        if (!isdigit(str[i])) {
            return false;
        }
    }
    return true;
}

void print_number(int num) {
    if (num % 3 == 0 && num % 5 == 0) {
            printf("Fizz Buzz");
        }
        else if (num % 3 == 0) {
            printf("Fizz");
        }
        else if (num % 5 == 0) {
            printf("Buzz");
        }
        else { 
            printf("%d", num);
        }
}

int main(int argc, char *argv[]) {
    if (argc < 2 || argc > 2 || !is_number(argv[1])) {
        printf("Usage: ./fizzbuzz int\n");
        return 0;
    }

    int limit = atoi(argv[1]);
    int i = 1;
    while (i <= limit) {
        print_number(i);
        if (i == limit) {
            break;
        }
        printf(", ");
        i++;
    }
    printf("\n");
    
    return 0;
}