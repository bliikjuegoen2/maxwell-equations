#ifndef MAXWELL_EQUARIONS_FIELDS_H
#define MAXWELL_EQUARIONS_FIELDS_H

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

// physical constants
extern const double EPSILON_0;

// physical map
extern int get_tile_physical_map(int i, int j, int k);
extern void set_tile_physical_map(int i, int j, int k, int value);

extern double charge_of(int tile_type);

// elctric field
extern Vector *get_point_electric_field(int i, int j, int k);
extern Vector *get_node_electric_field(int i, int j, int k);

// current field
extern Vector *get_current(int i, int j, int k);

// program status
extern int is_running();
extern void quit();


// constructors and destructors
extern void init_fields();
extern void destr_fields();

extern void guass_law_electric();

#endif // !MAXWELL_EQUARIONS_FIELDS_H