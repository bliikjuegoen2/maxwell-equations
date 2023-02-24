
#ifndef MAXWELL_EQUATIONS_KERNEL_H
#define MAXWELL_EQUATIONS_KERNEL_H

#include "vector.h"

#define DECL_KERNEL(KERNEL, TYPE) \
\
extern TYPE KERNEL[3*3*3]; \
\
extern TYPE *KERNEL##_at(int i, int j, int k); \


DECL_KERNEL(kernel_vec, Vector)
DECL_KERNEL(kernel_scalar_x, double)
DECL_KERNEL(kernel_scalar_y, double)
DECL_KERNEL(kernel_scalar_z, double)
DECL_KERNEL(kernel_scalar_disperse, double)

#endif // !MAXWELL_EQUATIONS_KERNEL_H