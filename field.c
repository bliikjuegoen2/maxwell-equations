#include "field.h"
#include "world_constants.h"
#include "accessor.h"
#include <stdlib.h>

#define DEF_SETGET_FIELD(FIELD_NAME, TYPE) \
\
TYPE get_tile_##FIELD_NAME(int i, int j, int k) { \
    return *get_##FIELD_NAME(i,j,k); \
} \
\
void set_tile_##FIELD_NAME(int i, int j, int k, TYPE value) {\
    *get_##FIELD_NAME(i,j,k) = value;\
}\

#define DEF_BASIC_FIELD(FIELD_NAME, TYPE) \
\
TYPE *FIELD_NAME;\
\
TYPE *get_##FIELD_NAME(int i, int j, int k) {\
    return TILE_AT(\
        FIELD_NAME\
        , ABS_MOD(i, WORLD_WIDTH), WORLD_WIDTH\
        , ABS_MOD(j, WORLD_HEIGHT), WORLD_HEIGHT\
        , ABS_MOD(k,WORLD_LENGTH), WORLD_LENGTH\
        , 1);\
}\
\
TYPE *FIELD_NAME##_data() {\
    return FIELD_NAME;\
}\

#define DEF_PADDED_FIELD(FIELD_NAME, TYPE) \
TYPE *FIELD_NAME; \
\
TYPE *get_point_##FIELD_NAME(int i, int j, int k) {\
    return get_point_field(FIELD_NAME, i, j, k);\
}\
\
TYPE *get_node_##FIELD_NAME(int i, int j, int k) {\
    return get_node_field(FIELD_NAME, i, j, k);\
}\
\
TYPE *FIELD_NAME##_data() {\
    return FIELD_NAME;\
}\

DEF_BASIC_FIELD(physical_map, int)
DEF_SETGET_FIELD(physical_map, int)
DEF_PADDED_FIELD(electric_field, Vector)
DEF_BASIC_FIELD(current_field, Vector)

Vector *alloc_vec_field() {
    return malloc(sizeof(Vector)*field_dimensions(WORLD_WIDTH)*field_dimensions(WORLD_HEIGHT)*field_dimensions(WORLD_LENGTH));
}

void alloc_fields() {
    physical_map = malloc(sizeof(int)*WORLD_WIDTH*WORLD_HEIGHT*WORLD_LENGTH);
    electric_field = alloc_vec_field();
    current_field = malloc(sizeof(Vector)*WORLD_WIDTH*WORLD_HEIGHT*WORLD_LENGTH);
}

void free_fields() {
    free(physical_map);
    free(electric_field);
    free(current_field);
}