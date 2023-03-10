#ifndef MAXWELL_EQUATIONS_VECTOR_H
#define MAXWELL_EQUATIONS_VECTOR_H

// vectors
typedef union Vector {
    struct {
        double x;
        double y;
        double z;
    };
    double array[3];
} Vector;

// accessors
extern double get_x(Vector *vec);
extern double get_y(Vector *vec);
extern double get_z(Vector *vec);

extern double get_scalar(double *x);

// constructors
extern Vector new_vector(double x, double y, double z);
extern Vector zero_vector();

// lin alg operations
Vector scalar_mul(double c, Vector *v);
Vector vec_add(Vector *a, Vector *b);
Vector vec_sub(Vector *a, Vector *b);
double dot(Vector *a, Vector *b);
Vector cross(Vector *a, Vector *b);
double magnitude(Vector *x);
Vector normalize(Vector *v);


#endif // !MAXWELL_EQUATIONS_VECTOR_H 