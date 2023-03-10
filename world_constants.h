#ifndef MAXWELL_EQUARIONS_WORLD_CONSTANTS_H
#define MAXWELL_EQUARIONS_WORLD_CONSTANTS_H

#include "vector.h"

// world dimensions
extern const int WORLD_WIDTH;
extern const int WORLD_HEIGHT;
extern const int WORLD_LENGTH;

// tile types
extern const int TILETYPE_WIRE;
extern const int TILETYPE_POSITIVE_CHARGE;
extern const int TILETYPE_NEGATIVE_CHARGE;
extern const int TILETYPE_INSULATOR;
extern const int TILETYPE_UP_CURRENT;
extern const int TILETYPE_DOWN_CURRENT;
extern const int TILETYPE_LEFT_CURRENT;
extern const int TILETYPE_RIGHT_CURRENT;
extern const int TILETYPE_FORWARD_CURRENT;
extern const int TILETYPE_BACKWARD_CURRENT;

// properties of wire
// extern const double MOVABLE_PARTICLE_DENSITY;
// extern const double MOVABLE_PARTICLE_CHARGE;
// extern const double MOVABLE_PARTICLE_MASS;
// extern const double CHARGE_TRANSFER_COEF;
// extern const double WIRE_RESISTANCE;

// physical constants
extern const double EPSILON_0;
extern const double MU_0;

double charge_of(int tile_type);
Vector current_of(int tile_type);
// double resistance_of(int tile_type);

#endif // !MAXWELL_EQUARIONS_WORLD_CONSTANTS_H