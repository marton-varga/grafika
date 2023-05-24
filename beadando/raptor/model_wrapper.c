#include "model_wrapper.h"

void init_model_wrapper(ModelWrapper* model_wrapper, char* obj_file, char* texture_file, float x, float y, float z){
    model_wrapper->texture_id = load_texture(texture_file);
    load_model((&model_wrapper->model), obj_file);
    model_wrapper->position.x=x;
    model_wrapper->position.y=y;
    model_wrapper->position.z=z;
    model_wrapper->rotation.x=0;
    model_wrapper->rotation.y=0;
    model_wrapper->rotation.z=0;
    model_wrapper->radius=1;
    model_wrapper->range=1;
    model_wrapper->scale.x=1;
    model_wrapper->scale.y=1;
    model_wrapper->scale.z=1;
    model_wrapper->center_position.x=x;
    model_wrapper->center_position.y=y;
    model_wrapper->center_position.z=z;
}

void init_model_wrapper_full(ModelWrapper* model_wrapper, char* obj_file, char* texture_file, vec3 position, vec3 rotation, vec3 scale, vec3 center_position){
    model_wrapper->texture_id = load_texture(texture_file);
    load_model((&model_wrapper->model), obj_file);
    model_wrapper->position=position;
    model_wrapper->rotation=rotation;
    model_wrapper->scale=scale;
    model_wrapper->center_position=center_position;
}

void init_model_wrapper_texture_id(ModelWrapper* model_wrapper, char* obj_file, GLuint text_id, float x, float y, float z){
    model_wrapper->texture_id = text_id;
    load_model((&model_wrapper->model), obj_file);
    model_wrapper->position.x=x;
    model_wrapper->position.y=y;
    model_wrapper->position.z=z;
    model_wrapper->rotation.x=0;
    model_wrapper->rotation.y=0;
    model_wrapper->rotation.z=0;
    model_wrapper->radius=1;
    model_wrapper->range=1;
    model_wrapper->scale.x=1;
    model_wrapper->scale.y=1;
    model_wrapper->scale.z=1;
    model_wrapper->center_position.x=x;
    model_wrapper->center_position.y=y;
    model_wrapper->center_position.z=z;
}

void move_model_wrapper(ModelWrapper* model_wrapper, vec3 vec){
    model_wrapper->position=vec3_add(&(model_wrapper->position), &vec);
    model_wrapper->center_position=vec3_add(&(model_wrapper->center_position), &vec);
}

void relocate_model_wrapper(ModelWrapper* model_wrapper, vec3 vec){
    vec3 pos_diff=vec3_subtract(&(model_wrapper->position), &(model_wrapper->center_position));
    model_wrapper->position=vec;
    model_wrapper->center_position = vec3_add(&vec, &pos_diff);
}

