#include "kernel.h"
#include "accessor.h"

#define DEF_KERNEL(KERNEL, TYPE) \
\
TYPE KERNEL[3*3*3]; \
\
TYPE *KERNEL##_at(int i, int j, int k) { \
    return TILE_AT(KERNEL, i, 3, j, 3, k, 3, 1); \
} \

DEF_KERNEL(kernel_vec, Vector)
DEF_KERNEL(kernel_scalar_x, float)
DEF_KERNEL(kernel_scalar_y, float)
DEF_KERNEL(kernel_scalar_z, float)