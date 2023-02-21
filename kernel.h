
#ifndef MAXWELL_EQUATIONS_KERNEL_H
#define MAXWELL_EQUATIONS_KERNEL_H

#include "vector.h"

#define DECL_KERNEL(KERNEL, TYPE) \
\
extern TYPE KERNEL[3*3*3]; \
\
TYPE *KERNEL##_at(int i, int j, int k); \


DECL_KERNEL(kernel_vec, Vector)
DECL_KERNEL(kernel_scalar_x, float)
DECL_KERNEL(kernel_scalar_y, float)
DECL_KERNEL(kernel_scalar_z, float)

#endif // !MAXWELL_EQUATIONS_KERNEL_H