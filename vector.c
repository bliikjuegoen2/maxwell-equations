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

Vector zero_vector() {
    Vector r;
    r.x = 0;
    r.y = 0;
    r.z = 0;
    return r;
}

// lin alg operations

Vector scalar_mul(double c, Vector *v) {
    Vector r;
    r.x = c*v->x;
    r.y = c*v->y;
    r.z = c*v->z;
    return r;
}

Vector vec_add(Vector *a, Vector *b) {
    Vector r;

    r.x = a->x + b->x;
    r.y = a->y + b->y;
    r.z = a->z + b->z;

    return r;
}

double dot(Vector *a, Vector *b) {
    return a->x * b->x + a->y * b->y + a->z * b->z;
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