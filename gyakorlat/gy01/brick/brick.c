#include "brick.h"


void set_size(Brick* brick, double a, double b, double c){
    brick->a=a;
    brick->b=b;
    brick->c=c;
}
double calc_volume(Brick* brick){
    return brick->a * brick->b * brick->c;
}
double calc_surface(Brick* brick){
    double a=brick->a;
    double b=brick->b;
    double c=brick->c;

    return 2*(a*b+a*c+b*c);
}

int does_have_square_side(Brick* brick){
    return brick->a == brick->b || brick->a == brick->c || brick->b == brick->c;
}

