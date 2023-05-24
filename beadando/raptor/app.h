#ifndef APP_H
#define APP_H

#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <GL/gl.h>
#include <pthread.h>

#include "camera.h"
#include "scene.h"

#define VIEWPORT_RATIO (16.0 / 9.0)
#define VIEWPORT_ASPECT 50.0

typedef struct App{
    SDL_Window* window;
    SDL_GLContext gl_context;
    bool is_running;
    double uptime;
    Camera camera;
    Scene scene;
    bool is_paused;
    bool is_game_over;
} App;

//Initialize the application.
void init_app(App* app, int width, int height);

//Initialize the OpenGL context.
void init_opengl();

//Reshape the window.
void reshape(GLsizei width, GLsizei height);

//Handle the events of the application.
void handle_app_events(App* app);

//Update the application.
void update_app(App* app);

//Render the application.
void render_app(App* app);

//Destroy the application.
void destroy_app(App* app);

#endif /* APP_H */
