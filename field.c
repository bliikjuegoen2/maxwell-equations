#include "field.h"
#include "world_constants.h"
#include "accessor.h"
#include <stdlib.h>
#include <stdio.h>

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


#define DEF_CLEAR_FIELD(FIELD, TYPE, LOOP, GETTER, ZERO) \
void clear_##FIELD() { \
    TYPE *point = NULL; \
    \
    LOOP(i,j,k, \
        point = GETTER(i, j, k); \
        *point = ZERO; \
    ) \
} \

// fields
DEF_BASIC_FIELD(physical_map, int)
DEF_SETGET_FIELD(physical_map, int)
DEF_PADDED_FIELD(electric_field, Vector)
DEF_CLEAR_FIELD(
    electric_field
    , Vector
    , LOOP_FIELD
    , get_point_electric_field
    , zero_vector()
    )
DEF_BASIC_FIELD(current_field, Vector)
DEF_BASIC_FIELD(charge_field, double)
DEF_SETGET_FIELD(charge_field, double)
DEF_CLEAR_FIELD(
    charge_field
    , double
    , LOOP_WORLD
    , get_charge_field
    , 0.0
    )

//temporary values
DEF_PADDED_FIELD(delta_vec_padded_field, Vector)
DEF_CLEAR_FIELD(
    delta_vec_padded_field
    , Vector
    , LOOP_FIELD
    , get_point_delta_vec_padded_field
    , zero_vector()
    )
DEF_BASIC_FIELD(delta_vec_basic_field, Vector)
DEF_CLEAR_FIELD(
    delta_vec_basic_field
    , Vector
    , LOOP_WORLD
    , get_delta_vec_basic_field
    , zero_vector()
    )
DEF_BASIC_FIELD(delta_float_basic_field, double)
DEF_CLEAR_FIELD(
    delta_float_basic_field
    , double
    , LOOP_WORLD
    , get_delta_float_basic_field
    , 0.0
    )

Vector *alloc_vec_field() {
    return malloc(sizeof(Vector)*field_dimensions(WORLD_WIDTH)*field_dimensions(WORLD_HEIGHT)*field_dimensions(WORLD_LENGTH));
}

void alloc_fields() {
    physical_map = malloc(sizeof(int)*WORLD_WIDTH*WORLD_HEIGHT*WORLD_LENGTH);
    electric_field = alloc_vec_field();
    current_field = malloc(sizeof(Vector)*WORLD_WIDTH*WORLD_HEIGHT*WORLD_LENGTH);
    charge_field = malloc(sizeof(double)*WORLD_WIDTH*WORLD_HEIGHT*WORLD_LENGTH);

    //delta fields
    delta_vec_padded_field = alloc_vec_field();
    delta_vec_basic_field = malloc(sizeof(Vector)*WORLD_WIDTH*WORLD_HEIGHT*WORLD_LENGTH);
    delta_float_basic_field = malloc(sizeof(double)*WORLD_WIDTH*WORLD_HEIGHT*WORLD_LENGTH);

}

void free_fields() {
    free(physical_map);
    free(electric_field);
    free(current_field);
    free(charge_field);

    // delta fields
    free(delta_vec_padded_field);
    free(delta_vec_basic_field);
    free(delta_float_basic_field);
}