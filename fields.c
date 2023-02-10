#include "fields.h" 
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>

#define FOR3D(I, WIDTH, J, HEIGHT, K, LENGTH, BODY) \
    for (int I = 0; I < WIDTH; I++) { \
        for (int J = 0; J < HEIGHT; J++) { \
            for (int K = 0; K < LENGTH; K++) { \
                BODY \
            } \
        } \
    } \

#define TILE_AT(ARRAY, I, WIDTH, J, HEIGHT, K, LENGTH, SIZE) ARRAY + (I*HEIGHT*LENGTH + J*LENGTH + K)*SIZE

// world dimensions
const int WORLD_WIDTH = 35;
const int WORLD_HEIGHT = 15;
const int WORLD_LENGTH = 35;

// tile types
const int TILETYPE_WIRE = 0;
const int TILETYPE_POSITIVE_CHARGE = 1;
const int TILETYPE_NEGATIVE_CHARGE = 2;
const int TILETYPE_INSULATOR = 3;


// getters for vector struct
float get_x(Vector *vec) {
    return vec->x;
}

float get_y(Vector *vec) {
    return vec->y;
}

float get_z(Vector *vec) {
    return vec->z;
}

float dot(Vector *a, Vector *b) {
    return a->x * b->x + a->y * b->y + a->z * b->z;
}

Vector scalarMul(float c, Vector *v) {
    Vector r;
    r.x = c*v->x;
    r.y = c*v->y;
    r.z = c*v->z;
    return r;
}

Vector normalize(Vector *v) {
    float magnitude_v = magnitude(v);

    // test if zero vector
    if (magnitude_v == 0) {
        return *v;
    }

    return scalarMul(1/magnitude_v, v);
}

float magnitude(Vector *x) {
    return sqrt(dot(x,x));
}

int field_dimensions(int dim) {
    return dim*2 + 1;
} 

// define globals
static int *physical_map;

int *physical_map_tile(int i, int j, int k) {
    return TILE_AT(physical_map, i, WORLD_WIDTH, j, WORLD_HEIGHT, k, WORLD_LENGTH, 1);
}

int get_tile_physical_map(int i, int j, int k) {
    return *physical_map_tile(i,j,k);
}

void set_tile_physical_map(int i, int j, int k, int value) {
    *physical_map_tile(i,j,k) = value;
}

static Vector *electric_field;

Vector *get_point_electric_field(int i, int j, int k) {
    return TILE_AT(electric_field, i, field_dimensions(WORLD_WIDTH), j, field_dimensions(WORLD_HEIGHT), k, field_dimensions(WORLD_LENGTH), 1);
}

Vector *get_node_electric_field(int i, int j, int k) {
    return get_point_electric_field(field_dimensions(i), field_dimensions(j), field_dimensions(k));
}

static int _is_running;

int is_running() {
    return _is_running;
}

void quit() {
    _is_running = 0;
}

static Vector kernel[3*3*3];

Vector *kernel_at(int i, int j, int k) {
    return TILE_AT(kernel, i, 3, j, 3, k, 3, 1);
}

// run at the start of the program
void init_fields() {

    _is_running = 1;

    physical_map = malloc(sizeof(int)*WORLD_WIDTH*WORLD_HEIGHT*WORLD_LENGTH);

    FOR3D(i, WORLD_WIDTH, j, WORLD_HEIGHT, k, WORLD_LENGTH,
        set_tile_physical_map(i, j, k, TILETYPE_INSULATOR);
    )

    electric_field = malloc(sizeof(Vector)*field_dimensions(WORLD_WIDTH)*field_dimensions(WORLD_HEIGHT)*field_dimensions(WORLD_LENGTH));

    Vector *point = NULL;

    FOR3D(i, field_dimensions(WORLD_WIDTH), j, field_dimensions(WORLD_HEIGHT), k, field_dimensions(WORLD_LENGTH),
        point = get_point_electric_field(i,j,k);
        point->x = 0;
        point->y = 0;
        point->z = 0;
    )

    point = NULL;

    FOR3D(i, 3, j, 3, k, 3, 
        point = kernel_at(i, j, k);
        point->x = i - 1;
        point->y = j - 1;
        point->z = k - 1;
        Vector normalized_point = normalize(point);
        *point = scalarMul(1.0/26.0, &normalized_point);
    )
    printf("done initializing\n");
}

// destructor
// shouldn't matter because the globals are expected to last through the life time of the program
void destr_fields() {
    free(physical_map);
    free(electric_field);
}