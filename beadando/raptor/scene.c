#include "scene.h"

#define MAP_FILE "models.csv"
#define MAX_LINE_LENGTH 256

#define COIN_MODEL "assets/models/coin.obj"
#define COIN_TEXTURE "assets/textures/coin.jpg"

#define SCORE_BASE_PATH "assets/score_img/"
#define SCORE_IMAGE_WIDTH 50
#define SCORE_IMAGE_HEIGHT 50

#define ENEMY_START_POSITION_X 10
#define ENEMY_START_POSITION_Y 0
#define ENEMY_START_POSITION_Z 0

GLuint score_textures[10];
GLuint pause_texture_id;
GLuint game_over_texture_id;
GLuint fog_texture_id;
GLuint fog_texture_id2;
int g_score=0;

/*READ FILE*/
void create_model_wrapper_from_line(ModelWrapper* model_wrapper, char* line){
    line[strcspn(line, "\n")] = '\0';

    char* token = strtok(line, ",");
    char model[MAX_LINE_LENGTH];
    strcpy(model, token);
    token = strtok(NULL, ",");
    char texture[MAX_LINE_LENGTH];
    strcpy(texture, token);

    vec3 position;
    token = strtok(NULL, ",");
    position.x=atof(token);
    token = strtok(NULL, ",");
    position.y=atof(token);
    token = strtok(NULL, ",");
    position.z = atof(token);

    vec3 rotation;
    token = strtok(NULL, ",");
    rotation.x=atof(token);
    token = strtok(NULL, ",");
    rotation.y=atof(token);
    token = strtok(NULL, ",");
    rotation.z = atof(token);

    vec3 scale;
    token = strtok(NULL, ",");
    scale.x=atof(token);
    token = strtok(NULL, ",");
    scale.y=atof(token);
    token = strtok(NULL, ",");
    scale.z = atof(token);

    vec3 center_pos;
    token = strtok(NULL, ",");
    center_pos.x=atof(token);
    token = strtok(NULL, ",");
    center_pos.y=atof(token);
    token = strtok(NULL, ",");
    center_pos.z = atof(token);

    token = strtok(NULL, ",");
    float radius = atof(token);

    token = strtok(NULL, ",");
    float range = atof(token);

    init_model_wrapper_full(model_wrapper, model, texture, position, rotation, scale, center_pos);
    model_wrapper->radius=radius;
    model_wrapper->range=range;

}
void read_field_values(Scene* scene, char* line){
    char* token = strtok(line, ",");
    scene->field_length_x = atoi(token);
    token = strtok(NULL, ",");
    scene->field_length_y = atoi(token);
    token = strtok(NULL, ",");
    scene->coin_objects_length = atoi(token);
}
void read_objects(Scene* scene, char* file_name){
    FILE* file = fopen(file_name, "r");
    if (file == NULL) {
        printf("Failed to open the file.\n");
        exit(1);
    }
    int models_cnt = 0;
    char line[MAX_LINE_LENGTH];
    int ch = 0;
    int i = 0;
    bool is_empty_line = true;
    bool first_line = true;

    while((ch = fgetc(file)) != EOF){
        line[i] = ch;
        i++;

        if(ch != ' ' && ch != '\t' && ch != '\n' && ch != '\r'){
            is_empty_line = false;
        }
        if(ch == '\n'){
            if(line[0] != '#' && !is_empty_line){
                if(first_line){
                    read_field_values(scene, line);
                    first_line = false;
                }else{
                    create_model_wrapper_from_line((&scene->field_objects[models_cnt]), line);
                    models_cnt++;

                }
            }
            i = 0;
            is_empty_line = true;
        }
    }
    fclose(file);
}
int file_count_lines(char* file_name){
    FILE *fp = fopen(file_name,"r");
    int lines = 0;
    int ch = 0;
    bool is_comment = false;
    bool empty_line = true;

    while((ch = fgetc(fp)) != EOF){
        if(ch == '\n'){
            if(!is_comment && !empty_line){
                lines++;
            }
            is_comment = false;
            empty_line = true;
        } else if(ch == '#'){
            is_comment = true;
        } else if(ch != '\r' && ch != ' ' && ch != '\t'){
            empty_line = false;
        }
    }

    fclose(fp);
    return lines-1;
}

/*INIT*/
void init_scene(Scene* scene){
    pause_texture_id = load_texture_RGBA("assets/textures/pause.png");
    game_over_texture_id = load_texture_RGBA("assets/textures/game_over.png");
    fog_texture_id = load_texture_RGBA("assets/textures/fog.png");
    fog_texture_id2 = load_texture_RGBA("assets/textures/fog2.png");
    init_score_texture_ids();

    scene->field_objects_length = file_count_lines(MAP_FILE);
    scene->field_objects = (ModelWrapper*)malloc(sizeof(ModelWrapper)*(scene->field_objects_length));
    read_objects(scene, MAP_FILE);

    //init_model_wrapper(&(scene->field_objects[0]), "assets/models/cat.obj", "assets/textures/tree.jpg", 3, 3, 0);
    init_coins(scene);

    init_model_wrapper(&(scene->enemy), "assets/models/raptor.obj", "assets/textures/raptor.png", ENEMY_START_POSITION_X, ENEMY_START_POSITION_Y, ENEMY_START_POSITION_Z);
    scene->enemy.radius=0.1f;

    init_material(scene);
    init_lighting();

    scene->is_fog_on = true;
    init_sounds(&(scene->sounds));
}
void init_coins(Scene* scene){
    scene->coin_objects=(ModelWrapper*)malloc(sizeof(ModelWrapper)*(scene->coin_objects_length));
    GLuint texture_id = load_texture(COIN_TEXTURE);
    for(int i=0;i<scene->coin_objects_length;i++){
        init_model_wrapper_texture_id(&(scene->coin_objects[i]), COIN_MODEL, texture_id, 0, 0, 0);
        set_coin_position_random(scene, &(scene->coin_objects[i]));
        scene->coin_objects[i].scale.x = 0.2;
        scene->coin_objects[i].scale.y = 0.2;
        scene->coin_objects[i].scale.z = 0.2;
        scene->coin_objects[i].radius = 0.1;
        scene->coin_objects[i].range = 0.8;
    }
}
void init_lighting(){
    set_lighting();
    float global_ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    float global_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    float global_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    float global_position[] = { 0.0f, 0.0f, 10.0f, 0.0f }; // Set the light position
    float global_falloff_const = 1.0f; // Adjust the light attenuation as needed
    float global_falloff_lin = 0.0f;
    float global_falloff_quad = 0.0f;
    glLightfv(GL_LIGHT1, GL_AMBIENT, global_ambient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, global_diffuse);
    glLightfv(GL_LIGHT1, GL_SPECULAR, global_specular);
    glLightfv(GL_LIGHT1, GL_POSITION, global_position);
    glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, global_falloff_const);
    glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, global_falloff_lin);
    glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, global_falloff_quad);
}
void init_material(Scene* scene){
    scene->material.ambient.red = 0.5;
    scene->material.ambient.green = 0.5;
    scene->material.ambient.blue = 0.5;

    scene->material.diffuse.red = 1.0;
    scene->material.diffuse.green = 1.0;
    scene->material.diffuse.blue = 1.0;

    scene->material.specular.red = 1.0;
    scene->material.specular.green = 1.0;
    scene->material.specular.blue = 1.0;

    scene->material.shininess = 1.0;
}
void init_score_texture_ids(){
    char filename[MAX_LINE_LENGTH];
    for (int i = 0; i < 10; i++) {
        sprintf(filename, "%s%d.png", SCORE_BASE_PATH, i);
        score_textures[i] = load_texture_RGBA(filename);
    }
}

/*LIGHTING, MATERIAL AND SOUND*/
void set_lighting(){
    float ambient_light[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    float diffuse_light[] = { 1.0f, 1.0f, 1.0, 1.0f };
    float specular_light[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    float position[] = { 0.0f, 0.0f, 1.0f, 1.0f };
    float falloff_const = 0.5f;
    float falloff_lin = 0.1f;
    float falloff_quad = 0.04f;
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient_light);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse_light);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular_light);
    glLightfv(GL_LIGHT0, GL_POSITION, position);
    glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, falloff_const);
    glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, falloff_lin);
    glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, falloff_quad);
}
void adjust_lighting(GLenum l, float x) {
    GLfloat ambient_light[4];
    GLfloat diffuse_light[4];
    GLfloat specular_light[4];
    glGetLightfv(l, GL_AMBIENT, ambient_light);
    glGetLightfv(l, GL_DIFFUSE, diffuse_light);
    glGetLightfv(l, GL_SPECULAR, specular_light);

    for (int i = 0; i < 3; ++i) {
        ambient_light[i] += x;
        diffuse_light[i] += x;
        specular_light[i] += x;


        //if(ambient_light[i]){}
        ambient_light[i] = fmaxf(-1.0f, fminf(ambient_light[i], 1.0f));
        diffuse_light[i] = fmaxf(-1.0f, fminf(diffuse_light[i], 1.0f));
        specular_light[i] = fmaxf(-1.0f, fminf(specular_light[i], 1.0f));

    }

    glLightfv(l, GL_AMBIENT, ambient_light);
    glGetLightfv(l, GL_AMBIENT, ambient_light);
    glLightfv(l, GL_DIFFUSE, diffuse_light);
    glLightfv(l, GL_SPECULAR, specular_light);
}
void set_material(const Material* material){
    float ambient_material_color[] = {
        material->ambient.red,
        material->ambient.green,
        material->ambient.blue
    };

    float diffuse_material_color[] = {
        material->diffuse.red,
        material->diffuse.green,
        material->diffuse.blue
    };

    float specular_material_color[] = {
        material->specular.red,
        material->specular.green,
        material->specular.blue
    };

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient_material_color);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse_material_color);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular_material_color);

    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &(material->shininess));
}

/*LOGIC*/
void reset_scene(Scene* scene, Camera* camera, bool* is_game_over){
    *is_game_over = false;
    g_score = 0;
    for(int i=0;i< scene->coin_objects_length;i++){
        set_coin_position_random(scene, &(scene->coin_objects[i]));
    }

    init_camera(camera);

    scene->enemy.position.x=ENEMY_START_POSITION_X;
    scene->enemy.position.y=ENEMY_START_POSITION_Y;
    scene->enemy.position.z=ENEMY_START_POSITION_Z;
    scene->enemy.center_position.x=ENEMY_START_POSITION_X;
    scene->enemy.center_position.y=ENEMY_START_POSITION_Y;
    scene->enemy.center_position.z=ENEMY_START_POSITION_Z;
}
void set_coin_position_random(Scene* scene, ModelWrapper* coin){
    int i;
    do{
        float x = frand(-((scene->field_length_x / 2)-coin->radius), (scene->field_length_x /2)-coin->radius);
        float y = frand(-((scene->field_length_y / 2)-coin->radius), (scene->field_length_y /2)-coin->radius);
        coin->position.x = x;
        coin->position.y = y;

        i=0;
        while(i<scene->field_objects_length && !collide(*coin, scene->field_objects[i])){
            i++;
        }
    }while(i<scene->field_objects_length);
}
void turn_towards_camera(ModelWrapper* model_wrapper, Camera* camera, float default_rotation){
    vec3 direction = vec3_subtract(&(camera->position), &(model_wrapper->position));
    //vec3 cross_product = vec3_cross_product(&direction, &(model_wrapper->rotation));
    //float dot_production = vec3_dot_product(&direction, &(model_wrapper->rotation));
    float angle = atan2f(direction.y, direction.x);

    angle = angle *(180.0 / M_PI) + default_rotation;
    model_wrapper->rotation.z = angle;

    if (model_wrapper->rotation.z < 0) {
        model_wrapper->rotation.z += 360.0;
    }
    if (model_wrapper->rotation.z > 360.0) {
        model_wrapper->rotation.z -= 360.0;
    }
}
void move_towards_camera(ModelWrapper* model_wrapper, Camera* camera, double elapsed_time){
    vec3 direction = vec3_subtract(&(camera->position), &(model_wrapper->position));
    vec3 normalized_direction = vec3_normalize(&direction);
    float speed = 3 * elapsed_time;
    //float speed = 0.0075;
    vec3 scaled_vec = vec3_scale(&normalized_direction, speed);
    vec3 new_position = vec3_add(&(model_wrapper->position), &scaled_vec);
    //model_wrapper->position.x = new_position.x;
    //model_wrapper->position.y = new_position.y;
    new_position.z = 0;
    relocate_model_wrapper(model_wrapper, new_position);
}
void prevent_collision_camera(Camera* camera, ModelWrapper* modelWrapper){
    vec3 distance = vec3_subtract(&(camera->position), &(modelWrapper->center_position));
    float combined_radius = camera->radius + modelWrapper->radius;
    if (vec3_length_squared(&distance) < pow(combined_radius, 2)) {
        distance = vec3_normalize(&distance);
        distance = vec3_scale(&distance, combined_radius);
        //camera->position = vec3_add(&modelWrapper->center_position, &distance);
        camera->position.x = vec3_add(&modelWrapper->center_position, &distance).x;
        camera->position.y = vec3_add(&modelWrapper->center_position, &distance).y;
    }
}
void prevent_collision(ModelWrapper* model_wrapper, ModelWrapper* model_wrapper2){
    vec3 distance = vec3_subtract(&(model_wrapper->center_position), &(model_wrapper2->center_position));
    float combined_radius = model_wrapper->radius + model_wrapper2->radius;
    if (vec3_length_squared(&distance) < pow(combined_radius, 2)) {
        distance = vec3_normalize(&distance);
        distance = vec3_scale(&distance, combined_radius);
        //model_wrapper->position = vec3_add(&model_wrapper2->position, &distance);
        //model_wrapper->position.x = vec3_add(&model_wrapper2->position, &distance).x;
        //model_wrapper->position.y = vec3_add(&model_wrapper2->position, &distance).y;

        vec3 new_position= vec3_add(&model_wrapper2->center_position, &distance);
        new_position.z = 0;
        relocate_model_wrapper(model_wrapper, new_position);
    }
}
bool camera_collide(Camera model1, ModelWrapper model2){
    float distance_squared = pow(model1.position.x - model2.position.x, 2) + pow(model1.position.y - model2.position.y, 2) + pow(model1.position.z - model2.position.z, 2);
    float radius_sum_squared = pow(model1.radius + model2.range, 2);
    return distance_squared <= radius_sum_squared;
}
bool collide(ModelWrapper model1, ModelWrapper model2){
    float distance_squared = pow(model1.position.x - model2.position.x, 2) + pow(model1.position.y - model2.position.y, 2) + pow(model1.position.z - model2.position.z, 2);
    float radius_sum_squared = pow(model1.radius + model2.radius, 2);
    return distance_squared <= radius_sum_squared;
    return 0;
}

/*UPDATE SCENE*/
void update_scene(Scene* scene, Camera* camera, double elapsed_time, bool* is_game_over){
    for(int i=0; i<scene->field_objects_length; i++) {
        prevent_collision_camera(camera, &(scene->field_objects[i]));
        prevent_collision(&(scene->enemy), &(scene->field_objects[i]));
    }
    turn_towards_camera(&(scene->enemy), camera, 270);
    move_towards_camera(&(scene->enemy), camera, elapsed_time);
    //enemy catches camera
    if(camera_collide(*camera, scene->enemy)){
        //if(scene->sound[0]==0){
            //strcpy(scene->sound, OUCH_SOUND);
            pthread_t thread;
            pthread_create(&thread, NULL, play_sound, (void*)(scene->sounds.ouch_sound));
            *is_game_over = true;
        //}
    }
    //animating coins
    for(int i=0; i< scene->coin_objects_length; i++){
        scene->coin_objects[i].rotation.x = 0;
        scene->coin_objects[i].rotation.y = 0;
        scene->coin_objects[i].rotation.z += 60 * elapsed_time;
        scene->coin_objects[i].position.z = 0.2 * sin((double)((i*1000)+SDL_GetTicks()) / 1000) + 0.1;
    }
    //getting coins
    for(int i=0; i<scene->coin_objects_length;i++){
        if(camera_collide(*camera, scene->coin_objects[i])){
            g_score++;
            //if(scene->sound[0]==0){
                //strcpy(scene->sound, COIN_SOUND);
                pthread_t thread; pthread_create(&thread, NULL, play_sound, (void*)(scene->sounds.coin_sound));
            //}
            set_coin_position_random(scene, &(scene->coin_objects[i]));
        }
    }

    //prevent_collision_camera(camera, &(scene->enemy));
    set_lighting();
    //float position[4] = {(camera->position.x), (camera->position.y), (camera->position.z-0.5), 1.0f};
    //glLightfv(GL_LIGHT0, GL_POSITION, position);
}

/*RENDERING*/
//2D rendering
void render_2d_object(float x, float y, float width, float height, GLuint textureID, double opacity){
    glPushAttrib(GL_ENABLE_BIT | GL_TEXTURE_BIT | GL_LIGHTING_BIT);  // Preserve the current enable, texture, and lighting state
    glDisable(GL_LIGHTING);  // Disable lighting
    glDisable(GL_DEPTH_TEST);  // Disable depth testing
    glEnable(GL_BLEND);  // Enable blending
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_TEXTURE_2D);  // Enable 2D texturing

    GLint matrixMode;
    glGetIntegerv(GL_MATRIX_MODE, &matrixMode);  // Save the current matrix mode

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();

    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    int screenWidth = viewport[2];
    int screenHeight = viewport[3];

    //float resizedWidth, resizedHeight;
    //calculateImageSize(&resizedWidth, &resizedHeight, width, height);

    y = screenHeight - y - height;

    glOrtho(0, screenWidth, 0, screenHeight, -1.0, 1.0);  // Set orthographic projection for 2D rendering

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glBindTexture(GL_TEXTURE_2D, textureID);

    float rectLeft = x;
    float rectRight = x + width;
    float rectTop = y + height;
    float rectBottom = y;

    GLfloat vertexCoords[] = {
        rectLeft, rectTop,       // Top-left
        rectRight, rectTop,      // Top-right
        rectRight, rectBottom,   // Bottom-right
        rectLeft, rectBottom     // Bottom-left
    };

    GLfloat texCoords[] = {
        0.0, 0.0,   // Bottom-left
        1.0, 0.0,   // Bottom-right
        1.0, 1.0,   // Top-right
        0.0, 1.0    // Top-left
    };

    glTexCoordPointer(2, GL_FLOAT, 0, texCoords);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    glVertexPointer(2, GL_FLOAT, 0, vertexCoords);
    glEnableClientState(GL_VERTEX_ARRAY);

    glColor4f(1.0, 1.0, 1.0, opacity);

    glDrawArrays(GL_QUADS, 0, 4);

    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);

    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(matrixMode);  // Restore the previous matrix mode

    glPopAttrib();  // Restore the previous enable, texture, and lighting state
}
void render_fog(){
    GLfloat fog_end = (GLfloat)(3*(sin((double)((10000)+SDL_GetTicks()) / 10000)) + 10);
    glFogf(GL_FOG_END, fog_end);

    float pos = (300*(sin((double)((10000)+SDL_GetTicks()) / 10000)) + 10);
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    int viewportWidth = viewport[2];
    int viewportHeight = viewport[3];

    float opacity = 0.01 + ((sin((double)(SDL_GetTicks()) / 5000) + 1) / 2) * (0.2 - 0.01);

    render_2d_object((-viewportWidth/2)+pos, 0, viewportWidth, viewportHeight, fog_texture_id, opacity);
    render_2d_object((viewportWidth/2)+pos, 0, viewportWidth, viewportHeight, fog_texture_id2, opacity);
}
void render_pause_screen(){
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    int viewportWidth = viewport[2];
    int viewportHeight = viewport[3];
    render_2d_object(0, 0, viewportWidth, viewportHeight, pause_texture_id, 1);
}
void render_game_over_screen(){
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    int viewportWidth = viewport[2];
    int viewportHeight = viewport[3];
    render_2d_object(0, 0, viewportWidth, viewportHeight, game_over_texture_id, 1);
}
void render_score(){
    int width = SCORE_IMAGE_WIDTH;
    int height= SCORE_IMAGE_HEIGHT;
    char numStr[12];
    sprintf(numStr, "%d", g_score);

    for (int i = 0; numStr[i] != '\0'; i++) {
        render_2d_object(i*(width-20), 0, width, height, score_textures[numStr[i]-48], 1);
    }
}

//3D rendering
void render_scene(const Scene* scene){
    set_material(&(scene->material));

    //field_objects
    for(int i=0; i<scene->field_objects_length;i++){
        render_object(&(scene->field_objects[i]));
    }
    //coins
    for(int i=0; i<scene->coin_objects_length;i++){
        render_object(&(scene->coin_objects[i]));
    }
    //enemy
    render_object(&(scene->enemy));

    if(scene->is_fog_on){
        render_fog();
    }
    render_score();
}
void render_object(const ModelWrapper* model_wrapper){
    glBindTexture(GL_TEXTURE_2D, model_wrapper->texture_id);
    glPushMatrix();
    glTranslatef(model_wrapper->position.x,model_wrapper->position.y,model_wrapper->position.z);
    glScalef(model_wrapper->scale.x, model_wrapper->scale.y, model_wrapper->scale.z);
    glRotatef(model_wrapper->rotation.x, 1, 0, 0);
    glRotatef(model_wrapper->rotation.y, 0, 1, 0);
    glRotatef(model_wrapper->rotation.z, 0, 0, 1);
    draw_model(&(model_wrapper->model));
    glPopMatrix();
}

/*DTOR*/
void destroy_scene(Scene* scene){
    for(int i=0;i<scene->field_objects_length;i++){
        free_model(&(scene->field_objects[i].model));
    }
    free(scene->field_objects);

    for(int i=0;i<scene->coin_objects_length;i++){
        free_model(&(scene->coin_objects[i].model));
    }
    free(scene->coin_objects);
}

