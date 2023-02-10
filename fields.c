#include "fields.h" 
#include <stdlib.h>
#include <stdio.h>

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
int get_x(Vector *vec) {
    return vec->x;
}

int get_y(Vector *vec) {
    return vec->y;
}

int get_z(Vector *vec) {
    return vec->z;
}

// define globals
int *physical_map;

int *physical_map_tile(int i, int j, int k) {
    return TILE_AT(physical_map, i, WORLD_WIDTH, j, WORLD_HEIGHT, k, WORLD_LENGTH, 1);
}

int get_tile_physical_map(int i, int j, int k) {
    return *physical_map_tile(i,j,k);
}

void set_tile_physical_map(int i, int j, int k, int value) {
    *physical_map_tile(i,j,k) = value;
}

// run at the start of the program
void init_fields() {

    physical_map = malloc(sizeof(int)*WORLD_WIDTH*WORLD_HEIGHT*WORLD_LENGTH);

    FOR3D(i, WORLD_WIDTH, j, WORLD_HEIGHT, k, WORLD_LENGTH,
        physical_map[i*WORLD_HEIGHT*WORLD_LENGTH + j*WORLD_LENGTH + k] = TILETYPE_INSULATOR;
    )
}

// destructor
// shouldn't matter because the globals are expected to last through the life time of the program
void destr_fields() {
    free(physical_map);
}