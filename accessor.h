#ifndef MAXWELL_EQUATIONS_ACCESSOR_H
#define MAXWELL_EQUATIONS_ACCESSOR_H

#include "vector.h"

#define FOR3D(I, WIDTH, J, HEIGHT, K, LENGTH, BODY) \
    for (int I = 0; I < WIDTH; I++) { \
        for (int J = 0; J < HEIGHT; J++) { \
            for (int K = 0; K < LENGTH; K++) { \
                BODY \
            } \
        } \
    } \

#define LOOP_FIELD(I,J,K, BODY) FOR3D(I, field_dimensions(WORLD_WIDTH), J, field_dimensions(WORLD_HEIGHT), K, field_dimensions(WORLD_LENGTH), BODY)

#define LOOP_WORLD(I,J,K, BODY) FOR3D(I, WORLD_WIDTH, J, WORLD_HEIGHT, K, WORLD_LENGTH, BODY)

#define LOOP_KERNEL(I,J,K, BODY) FOR3D(I, 3, J, 3, K, 3, BODY)

#define TILE_AT(ARRAY, I, WIDTH, J, HEIGHT, K, LENGTH, SIZE) ARRAY + (I*HEIGHT*LENGTH + J*LENGTH + K)*SIZE

#define ABS_MOD(I, N) ((I) % (N) + (N)) % (N)

int field_dimensions(int dim);

Vector *get_point_field(Vector *field, int i, int j, int k);
Vector *get_node_field(Vector *field, int i, int j, int k);
Vector *get_field_convolve(Vector *field, int i, int j, int k, int u, int v, int w);

#endif // !MAXWELL_EQUATIONS_ACCESSOR_H