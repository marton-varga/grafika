#include <stdio.h>
#include <time.h>

#include "app.h"

int main(int argc, char* argv[]){
    (void)argc;
    (void)argv;
    srand((unsigned int)time(NULL));

    App app;
    init_app(&app, 1280, 720);
    while (app.is_running) {
        handle_app_events(&app);
        update_app(&app);
        render_app(&app);
    }
    destroy_app(&app);

    return 0;
}
