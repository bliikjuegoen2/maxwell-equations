#ifndef MAXWELL_EQUATIONS_KERNEL_H
#define MAXWELL_EQUATIONS_KERNEL_H

#include "vector.h"

Vector kernel[3*3*3];

Vector *kernel_at(int i, int j, int k);

#endif // !MAXWELL_EQUATIONS_KERNEL_H