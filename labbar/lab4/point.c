#include <stdio.h>

struct point {
    int x;
    int y;
};

typedef struct point point_t;

int main(int argc, char const *argv[]) {

    point_t p = { .x = 10, .y = -42};

    printf("point(x=%d,y=%d)\n", p.x, p.y);
    return 0;
}

