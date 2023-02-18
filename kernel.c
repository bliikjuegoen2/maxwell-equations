#include "kernel.h"

Vector *kernel_at(int i, int j, int k) {
    return TILE_AT(kernel, i, 3, j, 3, k, 3, 1);
}