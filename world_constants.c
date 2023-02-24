#include "world_constants.h"

// world dimensions
const int WORLD_WIDTH = 100;
const int WORLD_HEIGHT = 25;
const int WORLD_LENGTH = 100;

// tile types
const int TILETYPE_WIRE = 0;
const int TILETYPE_POSITIVE_CHARGE = 1;
const int TILETYPE_NEGATIVE_CHARGE = 2;
const int TILETYPE_INSULATOR = 3;
const int TILETYPE_UP_CURRENT = 4;
const int TILETYPE_DOWN_CURRENT = 5;
const int TILETYPE_LEFT_CURRENT = 6;
const int TILETYPE_RIGHT_CURRENT = 7;
const int TILETYPE_FORWARD_CURRENT = 8;
const int TILETYPE_BACKWARD_CURRENT = 9;

// properties of wire
// const double MOVABLE_PARTICLE_DENSITY = 10;
// const double MOVABLE_PARTICLE_CHARGE = 0.01;
// const double MOVABLE_PARTICLE_MASS = 1;
// const double CHARGE_TRANSFER_COEF = 1.0/2.0;
// const double WIRE_RESISTANCE = 0.001;

// physical constants
const double EPSILON_0 = 1.0;

double charge_of(int tile_type) {
    if (tile_type == TILETYPE_POSITIVE_CHARGE)
        return 1;
    
    if (tile_type == TILETYPE_NEGATIVE_CHARGE)
        return -1;

    return 0;
}

// negative resistance = insulator

// double resistance_of(int tile_type) {
//     if (tile_type == TILETYPE_WIRE)
//         return 0.001;
//     if (tile_type == TILETYPE_INSULATOR) 
//         return -1;
    
//     return -1;
// }