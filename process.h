#ifndef MAXWELL_EQUARIONS_FIELDS_H
#define MAXWELL_EQUARIONS_FIELDS_H

#include "vector.h"

#define DECL_SETGET_FIELD(FIELD_NAME, TYPE) \
\
TYPE get_tile_##FIELD_NAME(int i, int j, int k);\
void set_tile_##FIELD_NAME(int i, int j, int k, TYPE value);\

#define DECL_BASIC_FIELD(FIELD_NAME, TYPE) \
\
TYPE *get_##FIELD_NAME(int i, int j, int k); \

#define DECL_PADDED_FIELD(FIELD_NAME, TYPE) \
\
TYPE *get_point_##FIELD_NAME(int i, int j, int k); \
TYPE *get_node_##FIELD_NAME(int i, int j, int k); \


DECL_BASIC_FIELD(physical_map, int)
DECL_SETGET_FIELD(physical_map, int)
DECL_PADDED_FIELD(electric_field, Vector)
DECL_BASIC_FIELD(current_field, Vector)

// program status
extern int is_running();
extern void quit();

// constructors and destructors
extern void init_fields();
extern void destr_fields();

extern void guass_law_electric();

#endif // !MAXWELL_EQUARIONS_FIELDS_H