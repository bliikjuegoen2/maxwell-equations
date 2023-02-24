#include "vector.h"
#include <math.h>


// getters for vector struct
double get_x(Vector *vec) {
    return vec->x;
}

double get_y(Vector *vec) {
    return vec->y;
}

double get_z(Vector *vec) {
    return vec->z;
}

double get_scalar(double *x) {
    return *x;
}

// constructors

Vector new_vector(double x, double y, double z) {
    Vector vec;
    vec.x = x;
    vec.y = y; 
    vec.z = z;
    return vec;
}

Vector zero_vector() {
    return new_vector(0, 0, 0);
}

// lin alg operations

Vector scalar_mul(double c, Vector *v) {
    return new_vector(
        c*v->x 
        , c*v->y 
        , c*v->z
    );
}

Vector vec_add(Vector *a, Vector *b) {
    return new_vector(
    a->x + b->x
    , a->y + b->y
    , a->z + b->z
    );
}


Vector vec_sub(Vector *a, Vector *b) {
    Vector neg_b = scalar_mul(-1, b);
    return vec_add(a, &neg_b);
}

double dot(Vector *a, Vector *b) {
    return a->x * b->x + a->y * b->y + a->z * b->z;
}

Vector cross(Vector *a, Vector *b) {
    return new_vector(
        a->y * b->z - a->z * b->y
        , a->z * b->x - a->x * b->z
        , a->x * b->y - a->y * b->x
    );
}


double magnitude(Vector *x) {
    return sqrt(dot(x,x));
}

Vector normalize(Vector *v) {
    double magnitude_v = magnitude(v);

    // test if zero vector
    if (magnitude_v == 0) {
        return *v;
    }

    return scalar_mul(1/magnitude_v, v);
}