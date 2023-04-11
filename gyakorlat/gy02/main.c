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
float color[3]={1.0f,1.0f,1.0f};
SDL_Window* window;
SDL_GLContext gl_context;
Line lines[MAX_LINE_COUNT];

int mode = 0;
int rectLine = 0;

double random(int max){
	return (double)(((double)rand()/(double)(RAND_MAX)) * max);
}
void reset(){
	ind=0;
	isDrawing=0;
}

void drawPalette(){
	//glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(1.0f, 1.0f, 1.0f);
	glScalef(255.0f, 255.0f, 255.0f);
	
	glBegin(GL_QUADS);
	glColor3f(1.0f, 0.0f, 0.0f); glVertex2f(0.0f, 0.0f);
	glColor3f(0.0f, 1.0f, 0.0f); glVertex2f(1.0f, 0.0f);
	glColor3f(0.0f, 0.0f, 1.0f); glVertex2f(1.0f, 1.0f);
	glColor3f(1.0f, 1.0f, 1.0f); glVertex2f(0.0f, 1.0f);
	glEnd();
	
}
void drawLines(int x, int y){
	//glClear(GL_COLOR_BUFFER_BIT);
	//gl_context = SDL_GL_CreateContext(window);
	//glMatrixMode(GL_PROJECTION);
    //glLoadIdentity();
    //glOrtho(0, 640, 480, 0, -200, 200);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	
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

    
}

void drawRectangles(int x, int y){
	//glClear(GL_COLOR_BUFFER_BIT);
    //gl_context = SDL_GL_CreateContext(window);
    //glMatrixMode(GL_PROJECTION);
    //glLoadIdentity();
    //glOrtho(0, 640, 480, 0, -200, 200);

    for(int i=0;i<ind;i++){
        glColor3f(lines[i].color[0],lines[i].color[1],lines[i].color[2]);
        glRecti(lines[i].x1, lines[i].y1, lines[i].x2, lines[i].y2);
    }
    if(isDrawing && ind<MAX_LINE_COUNT){
        glColor3f(lines[ind].color[0],lines[ind].color[1],lines[ind].color[2]);
        glRecti(lines[ind].x1, lines[ind].y1, x, y);
    }
}

void draw(int x, int y){
	glClear(GL_COLOR_BUFFER_BIT);
	switch(mode){
		case 0:
			switch(rectLine){
				case 0:
					drawLines(x,y);
					break;
				case 1:
					drawRectangles(x,y);
					break;
			}
			break;
		case 1:
			drawPalette();
			break;
		default:
			break;
	}
	SDL_GL_SwapWindow(window);
}

void drawLine(int x, int y){
	if(!isDrawing && ind<MAX_LINE_COUNT){
		lines[ind].color[0]=color[0];
		lines[ind].color[1]=color[1];
		lines[ind].color[2]=color[2];
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
			case SDL_SCANCODE_P:
				mode=!mode;
				break;
			case SDL_SCANCODE_T:
				rectLine=!rectLine;
				break;
			case SDL_SCANCODE_D:
				printf("rectLine:%d, mode:%d\n",rectLine,mode);
				printf("ind:%d\n",ind);
				printf("color:%f %f %f\n",color[0],color[1],color[2]);
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
				switch(mode){
					case 0:
						SDL_GetMouseState(&x, &y);
						drawLine(x,y);
						break;
					case 1:
						SDL_GetMouseState(&x, &y);
						GLfloat pixelColor[3];
						glReadPixels(x, 480-y, 1, 1, GL_RGB, GL_FLOAT, pixelColor);
						color[0]=pixelColor[0];
						color[1]=pixelColor[1];
						color[2]=pixelColor[2];
						break;
					default:
						break;
				}
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
	SDL_GetMouseState(&x, &y);
	draw(x,y);
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

