#ifndef CIRCLE_H
#define CIRCLE_H

typedef struct Circle{
	double x;
	double y;
	double radius;
	float color[3];
} Circle;

void set_circle_data(Circle* circle, double x, double y, double radius, float color[]);

double calc_circle_area(const Circle* circle);

#endif // CIRCLE_H
