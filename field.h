#ifndef MAXWELL_EQUARIONS_FIELD_H
#define MAXWELL_EQUARIONS_FIELD_H

#include "vector.h"

// SETGET

#define DECL_SETGET_FIELD(FIELD_NAME, TYPE) \
\
extern TYPE get_tile_##FIELD_NAME(int i, int j, int k);\
extern void set_tile_##FIELD_NAME(int i, int j, int k, TYPE value);\

// BASIC FIELD

#define DECL_BASIC_FIELD(FIELD_NAME, TYPE) \
\
TYPE *get_##FIELD_NAME(int i, int j, int k); \
TYPE *FIELD_NAME##_data();\

// padded field

#define DECL_PADDED_FIELD(FIELD_NAME, TYPE) \
\
extern TYPE *get_point_##FIELD_NAME(int i, int j, int k); \
extern TYPE *get_node_##FIELD_NAME(int i, int j, int k); \
TYPE *FIELD_NAME##_data();\

// clear field

#define DECL_CLEAR_FIELD(FIELD) \
void clear_##FIELD(); \

// fields
DECL_BASIC_FIELD(physical_map, int)
DECL_SETGET_FIELD(physical_map, int)
DECL_PADDED_FIELD(electric_field, Vector)
DECL_CLEAR_FIELD(electric_field)
DECL_BASIC_FIELD(electric_field_delta, Vector)
DECL_PADDED_FIELD(magnetic_field, Vector)
DECL_CLEAR_FIELD(magnetic_field)
DECL_BASIC_FIELD(magnetic_field_delta, Vector)
// DECL_BASIC_FIELD(current_field, Vector)
// DECL_PADDED_FIELD(charge_field, double)
// DECL_CLEAR_FIELD(charge_field)

// temporary values
DECL_PADDED_FIELD(delta_vec_padded_field, Vector)
DECL_CLEAR_FIELD(delta_vec_padded_field)
// DECL_BASIC_FIELD(delta_vec_basic_field, Vector)
// DECL_CLEAR_FIELD(delta_vec_basic_field)
// DECL_PADDED_FIELD(delta_float_padded_field, double)
// DECL_CLEAR_FIELD(delta_float_padded_field)


// constructors and destructors
void alloc_fields();
void free_fields();

Vector *alloc_vec_field();

#endif // !MAXWELL_EQUARIONS_FIELD_H