#include "rectangle.h"

#include <math.h>

void set_rectangle_data(Rectangle* rectangle, double a, double b)
{
	rectangle->a = a;
	rectangle->b = b;
}

double calc_rectangle_area(const Rectangle* rectangle)
{
	double area = rectangle->a * rectangle->b;
	return area;
}

