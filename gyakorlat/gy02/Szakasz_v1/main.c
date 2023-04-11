#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>


#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <GL/gl.h>

#define MAX_LINE_COUNT 10


	
typedef struct{
	int x1, y1;
	int x2, y2;
	float color[3];
}Line;

int isDrawing = 0;
int ind = 0;
SDL_Window* window;
SDL_GLContext gl_context;
Line lines[MAX_LINE_COUNT];

double random(int max){
	return (double)(((double)rand()/(double)(RAND_MAX)) * max);
}
void reset(){
	ind=0;
	isDrawing=0;
}
void drawLines(int x, int y){
	glClear(GL_COLOR_BUFFER_BIT);
	//glClearColor(0.0f, 0.0f, 0.0f, 0.2f);
	for(int i=0;i<ind;i++){
		glColor3f(lines[i].color[0],lines[i].color[1],lines[i].color[2]);
		glBegin(GL_LINES);
		glVertex2i(lines[i].x1, lines[i].y1);
		glVertex2i(lines[i].x2, lines[i].y2);
		glEnd();
	}
	if(isDrawing && ind<MAX_LINE_COUNT){
		glColor3f(lines[ind].color[0],lines[ind].color[1],lines[ind].color[2]);
		glBegin(GL_LINES);
		glVertex2i(lines[ind].x1, lines[ind].y1);
		glVertex2i(x, y);
		glEnd();
	}
	
	
	SDL_GL_SwapWindow(window);
}
void drawLine(int x, int y){
	if(!isDrawing && ind<MAX_LINE_COUNT){
		lines[ind].color[0]=random(1);
		lines[ind].color[1]=random(1);
		lines[ind].color[2]=random(1);
	}
	if(ind<MAX_LINE_COUNT){
		if(isDrawing){
			lines[ind].x2=x;
			lines[ind].y2=y;
			ind++;
		}else{
			lines[ind].x1=x;
			lines[ind].y1=y;
		}
	}
	isDrawing = !isDrawing;
}
int handle_event(){
	SDL_Event event;
    int x;
    int y;
	int q=1;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_KEYDOWN:
			switch (event.key.keysym.scancode) {
			case SDL_SCANCODE_ESCAPE:
				q = 0;
			break;
			case SDL_SCANCODE_R:
			reset();
			break;
			default:
			break;
			}
            break;
        case SDL_KEYUP:
            break;
        case SDL_MOUSEMOTION:
            SDL_GetMouseState(&x, &y);
            printf("x: %d, y: %d\n",x, y);
            break;
		case SDL_MOUSEBUTTONDOWN:
			switch (event.button.button) {
			case SDL_BUTTON_LEFT:
			SDL_GetMouseState(&x, &y);
			drawLine(x,y);
			break;
			default:
			break;
			}
		break;
        case SDL_QUIT:
            break;
        default:
            break;
        }
    }
	drawLines(x,y);
	
	return q;
}

int main() {
	SDL_Init(SDL_INIT_EVERYTHING);
    window = SDL_CreateWindow(
        "Szakasz Rajzolasa",
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
	
	srand((unsigned int)time(NULL));
	
	int q=1;
	while(q){
		q=handle_event();
	}
	
	SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
