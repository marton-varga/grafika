#ifndef SCENE_H
#define SCENE_H

#include <string.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <obj/load.h>
#include <obj/draw.h>
#include <sys/types.h>
#include <pthread.h>

#include "sound.h"
#include "camera.h"
#include "texture.h"
#include "model_wrapper.h"

typedef struct Scene{
    int field_length_x;
    int field_length_y;

    ModelWrapper* field_objects;
    int field_objects_length;

    ModelWrapper* coin_objects;
    int coin_objects_length;

    ModelWrapper enemy;
    Material material;

    bool is_fog_on;
    Sounds sounds;
} Scene;

/*READ FILE*/
void create_model_wrapper_from_line(ModelWrapper* model_wrapper, char* line);
void read_field_values(Scene* scene, char* line);
void read_objects(Scene* scene, char* file_name);
int file_count_lines(char* file_name);


/*INIT*/
void init_scene(Scene* scene);
void init_coins(Scene* scene);
void init_lighting();
void init_material(Scene* scene);
void init_score_texture_ids();

/*LIGHTING, MATERIAL AND SOUND*/
void set_lighting();
void adjust_lighting(GLenum l, float x);
void set_material(const Material* material);

/*LOGIC*/
void reset_scene(Scene* scene, Camera* camera, bool* is_game_over);
void set_coin_position_random(Scene* scene, ModelWrapper* coin);
void turn_towards_camera(ModelWrapper* model_wrapper, Camera* camera, float default_rotation);
void move_towards_camera(ModelWrapper* model_wrapper, Camera* camera, double elapsed_time);
void prevent_collision_camera(Camera* camera, ModelWrapper* modelWrapper2);
void prevent_collision(ModelWrapper* modelWrapper, ModelWrapper* modelWrapper2);
bool camera_collide(Camera model1, ModelWrapper model2);
bool collide(ModelWrapper model1, ModelWrapper model2);

/*UPDATE SCENE*/
void update_scene(Scene* scene, Camera* camera, double elapsed_time, bool* is_game_over);

/*RENDERING*/
//2D rendering
void render_2d_object(float x, float y, float width, float height, GLuint textureID, double opacity);
void render_fog();
void render_pause_screen();
void render_game_over_screen();
void render_score();

//3D rendering
void render_scene(const Scene* scene);
void render_object(const ModelWrapper* model_wrapper);

/*DTOR*/
void destroy_scene(Scene* scene);

#endif /* SCENE_H */
