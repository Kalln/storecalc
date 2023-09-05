#include <stdio.h>

void cat(const char *filename) {
    FILE *f = fopen(filename, "r");
    int c = fgetc(f);

    int row = 1;
    printf("=== %s ====", filename);
    printf("\n%d  ", row);

    while (c != EOF) {

        fputc(c, stdout);

        if (c == '\n') {
            row++;
            
            printf("%d  ", row);
        }
        c = fgetc(f);
    }

    fclose(f);
    fputc('\n', stdout);
}

int main(int argc, char const *argv[]) {
    if (argc < 2) {
        fprintf(stdout, "Usage: %s fil1 ... \n", argv[0]);
    }
    else {
        for (int i = 1; i < argc; ++i) {
            cat(argv[i]);
        }
        
    }
    return 0;
}
