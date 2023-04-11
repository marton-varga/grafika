#include "circle.h"
#include <stdio.h>
#include <math.h>
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <GL/gl.h>

#define NUM_OF_LINES_TO_MAKE_CIRCLE 7

SDL_Window* window;
SDL_GLContext gl_context;

void drawCircle(Circle* circle){
	const int n = NUM_OF_LINES_TO_MAKE_CIRCLE;
	float r=circle->radius;
	double angle = 2 * M_PI / n;

	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0.4f,0.4f,0.4f);
	glBegin(GL_LINES);
	glVertex2i(circle->x, circle->y+circle->radius);
	glVertex2i(circle->x, circle->y-circle->radius);
	glVertex2i(circle->x-circle->radius, circle->y);
	glVertex2i(circle->x+circle->radius, circle->y);
	glEnd();
    
	glColor3f(circle->color[0],circle->color[1],circle->color[2]);
	glBegin(GL_LINES);
	
	for (int i = 0; i < n; i++) {
		int x1, y1, x2, y2;
        x1 = circle->x + r * cos(i * angle);
        y1 = circle->y + r * sin(i * angle);
        x2 = circle->x + r * cos((i+1) * angle);
        y2 = circle->y + r * sin((i+1) * angle);
        glVertex2i(x1, y1);
        glVertex2i(x2, y2);
    }
	glEnd();
    
    SDL_GL_SwapWindow(window);
}

int main(){
	SDL_Init(SDL_INIT_EVERYTHING);
    window = SDL_CreateWindow(
        "Circle",
        SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
        640,
		480,
        SDL_WINDOW_OPENGL
		);
    gl_context = SDL_GL_CreateContext(window);
	glShadeModel(GL_SMOOTH);
    glClearColor(0.1, 0.1, 0.1, 1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, 640, 480, 0, -200, 200);
    glViewport(0, 0, 640, 480);	
	
	Circle circle;
	double area;
	
	float clr[3]={1.0f,0.0f,1.0f};
	set_circle_data(&circle, 320, 240, 180, clr);
	area = calc_circle_area(&circle);
	printf("Circle area: %lf\n", area);
	
	int q=1;
	while(q){
		drawCircle(&circle);
		SDL_Delay(2000);
	}
	
	return 0;
}
