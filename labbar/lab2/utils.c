#include <stdio.h>


int ask_question_int(char *question) {
    printf("%s\n", question);
    int result = -1;
    scanf("%d", &result);
    return result;
}


int main(void) {
    int tal;

    tal = ask_question_int("första talet:");
    printf("du skrev %d\n", tal);

    tal = ask_question_int("andra talet:");
    printf("du skrev %d\n", tal);

    return 0;
}