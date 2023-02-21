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
TYPE *get_point_##FIELD_NAME(int i, int j, int k); \
TYPE *get_node_##FIELD_NAME(int i, int j, int k); \
TYPE *FIELD_NAME##_data();\

DECL_BASIC_FIELD(physical_map, int)
DECL_SETGET_FIELD(physical_map, int)
DECL_PADDED_FIELD(electric_field, Vector)
DECL_BASIC_FIELD(current_field, Vector)

// constructors and destructors
void alloc_fields();
void free_fields();

Vector *alloc_vec_field();

#endif // !MAXWELL_EQUARIONS_FIELD_H