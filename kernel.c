#include "kernel.h"
#include "accessor.h"

Vector kernel[3*3*3];

Vector *kernel_at(int i, int j, int k) {
    return TILE_AT(kernel, i, 3, j, 3, k, 3, 1);
}