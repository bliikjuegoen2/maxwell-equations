#include "accessor.h"
#include "world_constants.h"

Vector *get_point_field(Vector *field, int i, int j, int k) {
    return TILE_AT(field, i, field_dimensions(WORLD_WIDTH), j, field_dimensions(WORLD_HEIGHT), k, field_dimensions(WORLD_LENGTH), 1);
}

Vector *get_node_field(Vector *field, int i, int j, int k) {
    return get_point_field(field, field_dimensions(ABS_MOD(i, WORLD_WIDTH)), field_dimensions(ABS_MOD(j, WORLD_HEIGHT)), field_dimensions(ABS_MOD(k,WORLD_LENGTH)));
}

int field_dimensions(int dim) {
    return dim*2 + 1;
} 