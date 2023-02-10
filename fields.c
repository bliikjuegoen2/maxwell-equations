#include "fields.h" 
#include <stdlib.h>
#include <stdio.h>

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