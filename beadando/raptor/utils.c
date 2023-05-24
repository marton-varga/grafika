#include "utils.h"

vec3 vec3_add(const vec3* a, const vec3* b) {
    vec3 res;
    res.x = a->x + b->x;
    res.y = a->y + b->y;
    res.z = a->z + b->z;
    return res;
}

vec3 vec3_subtract(const vec3* a, const vec3* b) {
    vec3 res;
    res.x = a->x - b->x;
    res.y = a->y - b->y;
    res.z = a->z - b->z;
    return res;
}

float vec3_length_squared(const vec3* v) {
    return (v->x * v->x) + (v->y * v->y) + (v->z * v->z);
}

vec3 vec3_normalize(const vec3* v) {
    vec3 res;
    float len = sqrtf(vec3_length_squared(v));
    if (len != 0) {
        res.x = v->x / len;
        res.y = v->y / len;
        res.z = v->z / len;
    }
    return res;
}

vec3 vec3_scale(const vec3* a, float s) {
    vec3 res;
    res.x = a->x * s;
    res.y = a->y * s;
    res.z = a->z * s;
    return res;
}

float vec3_dot_product(const vec3* a, const vec3* b){
    return a->x * b->x + a->y * b->y + a->z * b->z;
}

vec3 vec3_cross_product(const vec3* a, const vec3* b){
    vec3 res;
    res.x = a->y * b->z - a->z * b->y;
    res.y = a->z * b->x - a->x * b->z;
    res.z = a->x * b->y - a->y * b->x;
    return res;
}

double degree_to_radian(double degree){
	return degree * M_PI / 180.0;
}

float frand(float min, float max){
    return ((float)rand()/(float)(RAND_MAX)) * (max - min) + min;
}
