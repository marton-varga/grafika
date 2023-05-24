#ifndef UTILS_H
#define UTILS_H

#include <stdlib.h>
#include <math.h>

//GLSL-like three dimensional vector
typedef struct vec3{
    float x;
    float y;
    float z;
} vec3;

//Color with RGB components
typedef struct Color{
    float red;
    float green;
    float blue;
} Color;

//Material
typedef struct Material{
    struct Color ambient;
    struct Color diffuse;
    struct Color specular;
    float shininess;
} Material;

//Functions
vec3 vec3_add(const vec3* a, const vec3* b);
vec3 vec3_subtract(const vec3* a, const vec3* b);
float vec3_length_squared(const vec3* v);
vec3 vec3_normalize(const vec3* v);
vec3 vec3_scale(const vec3* a, float s);
float vec3_dot_product(const vec3* a, const vec3* b);
vec3 vec3_cross_product(const vec3* a, const vec3* b);

//Calculates radian from degree
double degree_to_radian(double degree);

//Generates a random float value between the given interval
float frand(float min, float max);

#endif /* UTILS_H */
