#ifndef MODEL_WRAPPER_H
#define MODEL_WRAPPER_H

#include <obj/load.h>
#include <GL/gl.h>
#include "utils.h"
#include "texture.h"


typedef struct ModelWrapper{
    Model model;
    vec3 position;
    vec3 rotation;
    vec3 scale;
    vec3 center_position;
    GLuint texture_id;
    float radius;
    float range;
    vec3 speed;
}ModelWrapper;

void init_model_wrapper(ModelWrapper* model_wrapper, char* obj_file, char* texture_file, float x, float y, float z);

void init_model_wrapper_full(ModelWrapper* model_wrapper, char* obj_file, char* texture_file, vec3 position, vec3 rotation, vec3 scale, vec3 center_position);

void init_model_wrapper_texture_id(ModelWrapper* model_wrapper, char* obj_file, GLuint text_id, float x, float y, float z);

void move_model_wrapper(ModelWrapper* model_wrapper, vec3 vec);

void relocate_model_wrapper(ModelWrapper* model_wrapper, vec3 vec);

#endif /* MODEL_WRAPPER_H */

