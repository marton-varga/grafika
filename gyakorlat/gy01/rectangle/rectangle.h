#ifndef RECTANGLE_H
#define RECTANGLE_H

typedef struct Rectangle
{
	double a;
	double b;
} Rectangle;

void set_rectangle_data(Rectangle* rectangle, double a, double b);

double calc_rectangle_area(const Rectangle* rectangle);

#endif // RECTANGLE_H
