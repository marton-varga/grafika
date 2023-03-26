#ifndef BRICK_H
#define BRICK_H

typedef struct Brick{
    double a;
    double b;
    double c;
} Brick;

void set_size(Brick* brick, double a,double b, double c);
double calc_volume(Brick* brick);
double calc_surface(Brick* brick);
int does_have_square_side(Brick* brick);

#endif // BRICK_H
