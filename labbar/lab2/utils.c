#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// hur fungerar den här?
void clear_input_buffer() {
    int c;
    do {
        // hur vet den var i minnet den ska hämta symbolen?
        // är tangentsbords buffret (stdin), vid en allokerad plats
        // vid körning av programmet?
        c = getchar();
    }
    while(c != '\n' && c != EOF);
}
int ask_question_int(char *question) {
    int result = 0;
    int conversion = 0; 
    do {
        printf("%s\n", question);

        // conversion blir 1 om det finns innehåll som är en int?
        conversion = scanf("%d", &result);
        
        clear_input_buffer();
        
    }
    while (conversion < 1);
    return result;
}

// 
int read_string(char *buf, int buf_size) {
    int MAX_BUF_SIZE = buf_size - 1;
    int counter = 0;
    char result = 0;

    /* Vi vill:
        1. Vi har buffer och buffer size. Den får inte vara längre än buf_size - 1.
        2. Läs in en string. Kontrollera om den är längre än MAX_BUF_SIZE
        3. Om den är det return 0.
        4. Om inte så ska vi läsa varje sträng -> kopiera till buf -> avsluta med '\0'
    */

   // will read string and scan
    do {
        scanf("%s", result);
        printf("%s", result);
    }
    while (strlen(result) == 0);

    return 1;
}


//unused for now...
char *ask_question_string(char *question) {
    char result[255];

    do {
        printf("%s\n", question);
        scanf("%s", result);
    }
    while (strlen(result) == 0);

    return strdup(result);
}


int main(void) {
    int buf_siz = 255;
    int read = 0;
    char buf[buf_siz];

    read = read_string(buf, buf_siz);

/*     
    puts("Läs in en sträng:");
    read = read_string(buf, buf_siz);
    printf("'%s' (%d tecken)\n", buf, read);

    puts("Läs in en sträng till:");
    read = read_string(buf, buf_siz);
    printf("'%s' (%d tecken)\n", buf, read); 

*/

    return 0;
}