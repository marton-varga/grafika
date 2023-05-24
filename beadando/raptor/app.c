#include "app.h"

float jump_start_time = -1;
bool is_paused_key_pressed;

void init_app(App* app, int width, int height){
    is_paused_key_pressed = false;

    int error_code;
    int inited_loaders;

    app->is_running = false;

    error_code = SDL_Init(SDL_INIT_EVERYTHING);
    if (error_code != 0) {
        printf("[ERROR] SDL initialization error: %s\n", SDL_GetError());
        return;
    }

    app->window = SDL_CreateWindow(
        "Raptor",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        width, height,
        SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    if (app->window == NULL) {
        printf("[ERROR] Unable to create the application window!\n");
        return;
    }

    inited_loaders = IMG_Init(IMG_INIT_PNG);
    if (inited_loaders == 0) {
        printf("[ERROR] IMG initialization error: %s\n", IMG_GetError());
        return;
    }

    app->gl_context = SDL_GL_CreateContext(app->window);
    if (app->gl_context == NULL) {
        printf("[ERROR] Unable to create the OpenGL context!\n");
        return;
    }

    init_opengl();
    reshape(width, height);

    init_camera(&(app->camera));
    init_scene(&(app->scene));

    app->is_running = true;
    app->is_paused = false;
    app->is_game_over = false;
}

void init_opengl(){
    glShadeModel(GL_SMOOTH);//(GL_FLAT);

    glEnable(GL_NORMALIZE);
    glEnable(GL_AUTO_NORMAL);

    //glClearColor(0.1, 0.5, 0.8, 1.0);
    glClearColor(0.110, 0.110, 0.110, 1.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glEnable(GL_DEPTH_TEST);

    glClearDepth(2.0);

    glEnable(GL_TEXTURE_2D);

    glEnable(GL_LIGHTING);

    glEnable(GL_FOG);
    glFogi(GL_FOG_MODE, GL_LINEAR);
    glFogf(GL_FOG_START, 3);
    glFogf(GL_FOG_END, 30);
    float color[4]= {0.110, 0.110, 0.110, 1.0};
    glFogfv(GL_FOG_COLOR, color);

    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
}

void reshape(GLsizei width, GLsizei height){
    int x, y, w, h;

    //double ratio = (double)width / height;

    if(1){//ratio > VIEWPORT_RATIO) {
        w = (int)((double)height * VIEWPORT_RATIO);
        h = height;
        x = (width - w) / 2;
        y = 0;
    }
    else{
        w = width;
        h = (int)((double)width / VIEWPORT_RATIO);
        x = 0;
        y = (height - h) / 2;
    }

    glViewport(x, y, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(
        -.08, .08,
        -.045, .045,
        .1, 1000
    );
    /*
    glFrustum(
        -.08, .08,
        -.06, .06,
        .1, 10
    );
    */
}

void handle_app_events(App* app){
    SDL_Event event;
    static bool is_mouse_down = false;
    static int mouse_x = 0;
    static int mouse_y = 0;
    int x;
    int y;
    static bool is_pause_key_down = false;

    while (SDL_PollEvent(&event)){
        switch (event.type) {
            case SDL_WINDOWEVENT:
                switch(event.window.event){
                case SDL_WINDOWEVENT_MOVED:
                    {
                        double current_time = (double)SDL_GetTicks() / 1000;
                        app->uptime = current_time;
                    }
                break;
                case SDL_WINDOWEVENT_SIZE_CHANGED:
                case SDL_WINDOWEVENT_RESIZED:
                    {
                        int new_width = event.window.data1;
                        int new_height = event.window.data2;
                        reshape(new_width , new_height);
                        double current_time = (double)SDL_GetTicks() / 1000;
                        app->uptime = current_time;
                    }
                    break;
                }
                break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.scancode) {
                case SDL_SCANCODE_F1:
                    is_pause_key_down = true;
                    break;
                case SDL_SCANCODE_F5:
                    reset_scene(&(app->scene), &(app->camera), &(app->is_game_over));
                    break;
                case SDL_SCANCODE_ESCAPE:
                    app->is_running = false;
                    break;
                case SDL_SCANCODE_F10:
                    glFogf(GL_FOG_END, 30);
                    (app->scene.is_fog_on) = !(app->scene.is_fog_on);
                    break;
                case SDL_SCANCODE_F11:
                    if (SDL_GetWindowFlags(app->window) & SDL_WINDOW_FULLSCREEN) {
                        SDL_SetWindowFullscreen(app->window, 0);
                    } else {
                        SDL_SetWindowFullscreen(app->window, SDL_WINDOW_FULLSCREEN_DESKTOP);
                    }
                    break;
                case SDL_SCANCODE_F12:
                    *(app->scene.sounds.mute_ptr) = !(*(app->scene.sounds.mute_ptr));
                    break;
                default: break;
                }
            break;
            case SDL_KEYUP:
                switch (event.key.keysym.scancode) {
                case SDL_SCANCODE_F1:
                    is_pause_key_down = false;
                break;
                    case SDL_SCANCODE_W:
                    set_camera_speed(&(app->camera), 0);
                    break;
                case SDL_SCANCODE_S:
                    set_camera_speed(&(app->camera), 0);
                    break;
                case SDL_SCANCODE_A:
                    set_camera_side_speed(&(app->camera), 0);
                    break;
                case SDL_SCANCODE_D:
                    set_camera_side_speed(&(app->camera), 0);
                    break;
                case SDL_SCANCODE_KP_MINUS:
                    adjust_lighting(GL_LIGHT1, -0.1f);
                    break;
                case SDL_SCANCODE_KP_PLUS:
                    adjust_lighting(GL_LIGHT1, 0.1f);
                    break;
                default:
                    break;
                }
            break;
            case SDL_MOUSEBUTTONUP:
                is_mouse_down = false;
            break;
            case SDL_QUIT:
                app->is_running = false;
            break;
            default: break;
        }

        //Separate event_handling for events that are not allowed while the game is paused
        if(!(app->is_paused) && !(app->is_game_over)){
            switch (event.type) {
            case SDL_KEYDOWN:
                switch (event.key.keysym.scancode) {
                case SDL_SCANCODE_W:
                    set_camera_speed(&(app->camera), (app->camera.default_speed));
                    break;
                case SDL_SCANCODE_S:
                    set_camera_speed(&(app->camera), -(app->camera.default_speed));
                    break;
                case SDL_SCANCODE_A:
                    set_camera_side_speed(&(app->camera), (app->camera.default_speed));
                    break;
                case SDL_SCANCODE_D:
                    set_camera_side_speed(&(app->camera), -(app->camera.default_speed));
                    break;
                case SDL_SCANCODE_UP:
                    rotate_camera(&(app->camera), 0, app->camera.default_speed);
                    break;
                case SDL_SCANCODE_DOWN:
                    rotate_camera(&(app->camera), 0, -(app->camera.default_speed));
                    break;
                case SDL_SCANCODE_LEFT:
                    rotate_camera(&(app->camera), (app->camera.default_speed),0);
                    break;
                case SDL_SCANCODE_RIGHT:
                    rotate_camera(&(app->camera), -(app->camera.default_speed),0);
                    break;
                case SDL_SCANCODE_SPACE:
                    if((int)jump_start_time==-1){
                            pthread_t thread; pthread_create(&thread, NULL, play_sound, (void*)(app->scene.sounds.jump_sound));
                            jump_start_time = (double)SDL_GetTicks() / 1000;
                    }
                    break;
                default:
                    break;
                }
                break;
            case SDL_MOUSEBUTTONDOWN:
                is_mouse_down = true;
                break;
            case SDL_MOUSEMOTION:
                SDL_GetMouseState(&x, &y);
                if (is_mouse_down) {
                    rotate_camera(&(app->camera), mouse_x - x, mouse_y - y);
                }
                mouse_x = x;
                mouse_y = y;
                break;
            default:
                break;
            }
        }
    }
    if (is_pause_key_down) {
        app->is_paused = !app->is_paused;
        is_pause_key_down = false;
    }
}

void update_app(App* app){
    double current_time = (double)SDL_GetTicks() / 1000;
    double elapsed_time = current_time - app->uptime;;
    app->uptime = current_time;

    if(!(app->is_paused) && !(app->is_game_over)){
        if (jump_start_time > -1){
            double jump_elapsed_time = current_time - jump_start_time;
            if (app->camera.position.z > 0.99999) {
                double jump_speed = 3 - (jump_elapsed_time * 10);
                set_camera_height_speed(&(app->camera), jump_speed);
            } else {
                set_camera_height_speed(&(app->camera), 0);
                app->camera.position.z = 1;
                jump_start_time = -1;
            }
        }
        update_camera(&(app->camera), elapsed_time, (app->scene.field_length_x)/2, (app->scene.field_length_y)/2);
        update_scene(&(app->scene), &(app->camera), elapsed_time, &(app->is_game_over));

        //GLfloat fog_end = (GLfloat)(3*(sin((double)((10000)+SDL_GetTicks()) / 10000)) + 10);
        //glFogf(GL_FOG_END, fog_end);
    }
}

void render_app(App* app){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);

    glPushMatrix();
    //if(!(app->is_paused) && !(app->is_game_over)){
        set_view(&(app->camera));
        render_scene(&(app->scene));
        if((app->is_game_over)){
            render_game_over_screen();
        }else if(app->is_paused){
        render_pause_screen();
        }
    //}

    glPopMatrix();

    if (app->camera.is_preview_visible){
        show_texture_preview();
    }
    SDL_GL_SwapWindow(app->window);
}

void destroy_app(App* app){
    if (app->gl_context != NULL) {
        SDL_GL_DeleteContext(app->gl_context);
    }

    if (app->window != NULL) {
        SDL_DestroyWindow(app->window);
    }

    //if(&app->scene != NULL){
        destroy_scene(&app->scene);
    //}
}

