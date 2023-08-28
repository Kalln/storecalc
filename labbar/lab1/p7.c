#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

bool is_number(char *str) {
    // ta fram längden av str
    int len = strlen(str);
    // kontrollera negativa tal
    if (str[0] == '-' && str[1] == '\0') {
        return false;
    }
    // loopa genom str, kolla om varje 
    // tecken är en siffra, om nej -> inte int
    for (int i = str[0] == '-'; i < len; i++) {
        if (!isdigit(str[i])) {
            return false;
        }
    }
    return true;
}

int main(int argc, char *argv[]){
    if (argc > 1 && is_number(argv[1])) {
        if (argv[1][0] == '-') {
            printf("negative numbers are not supported\n");
        } else {
            printf("%s is a number\n", argv[1]);
        }
    }
    else {
        if (argc > 1) {
            printf("%s is not a number\n", argv[1]);
        }
        else {
            printf("Please provide a command line argument!\n");
        }
    }

    return 0;
}