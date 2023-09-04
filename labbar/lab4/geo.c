#include <stdio.h>
#include <stdbool.h>

struct point {
    int x;
    int y;
};

typedef struct point point_t;

struct rectangle { 
    point_t upper_left;
    point_t lower_right;
};

typedef struct rectangle rectangle_t;


void translate(point_t *p1, point_t *p2) {
  p1->x += p2->x;
  p1->y += p2->y;
}

void print_point(point_t *p) {
    printf("point(%d,%d)", p->x, p->y);
}

void print_rect(rectangle_t *rect) {
    printf("rectangle(upper_left(");
    print_point(&rect->upper_left);
    printf("), lower_right=(");
    print_point(&rect->lower_right);
    printf("))\n");
}

point_t make_point(int x, int y) {
    point_t p = {.x = x, .y = y};
    return p;
}

int area_rect(rectangle_t *rect) {
    int result = 0;
    // b * h.
    int dy = rect->upper_left.y - rect->lower_right.y;
    int dx = rect->upper_left.x - rect->lower_right.x;

    result = dy * dx;
    if (result < 0) {
        return (result * -1); 
    }
    return result;
}

bool intersect_rect(rectangle_t *rect1, rectangle_t *rect2) {
    if (rect1->upper_left.y < rect2->lower_right.y 
        || rect1->lower_right.y > rect2->upper_left.y) {
            return false;
        }
    if (rect1->lower_right.x < rect2->upper_left.x 
        || rect1->upper_left.x > rect2->lower_right.x) {
            return false;
        }

    return true;
}

rectangle_t make_rect(int x1, int y1, int x2, int y2) {
    rectangle_t rect = {.upper_left = make_point(x1, y1), .lower_right = make_point(x2, y2)};
    return rect;
}

rectangle_t intersection_rect(rectangle_t *rect1, rectangle_t *rect2) {
    rectangle_t r = make_rect(0, 0, 0, 0);
    if (intersect_rect(rect1, rect2)) {
        r.upper_left.x = rect1->upper_left.x < rect2->upper_left.x ? rect2->upper_left.x : rect1->upper_left.x;
        r.upper_left.y = rect1->upper_left.y < rect2->upper_left.y ? rect1->upper_left.y : rect2->upper_left.y;
        r.lower_right.x = rect1->lower_right.x < rect2->lower_right.x ? rect1->lower_right.x : rect2->lower_right.x; 
        r.upper_left.y = rect1->lower_right.y < rect2->lower_right.y ? rect2->lower_right.y : rect1->lower_right.y;
    }

    return r;
}


int main(int argc, char const *argv[]) {
    //point_t p1 = make_point(4 , 312);
    //point_t p2 = make_point(3, 20);
    //rectangle_t r = {.upper_left = p1, .lower_right = p2};

    rectangle_t r2 = make_rect(0, 10, 10, 0);
    rectangle_t r3 = make_rect(3, 5, 6, -5);
    rectangle_t sm = intersection_rect(&r2, &r3);
    printf("intersect: %d\n\n", intersect_rect(&r2, &r3));
    
    print_rect(&sm);
    printf("rect1 area: %d\n", area_rect(&r2));
    //print_point(&p1);
    return 0;
}
