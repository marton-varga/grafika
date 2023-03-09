#include "rectangle.h"

#include <stdio.h>

int main(int argc, char* argv[])
{
	Rectangle rectangle;
	double area;

	set_rectangle_data(&rectangle, 5, 10);
	area = calc_rectangle_area(&rectangle);

	printf("Rectangle area: %lf\n", area);

	return 0;
}
