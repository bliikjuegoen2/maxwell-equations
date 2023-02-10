#include "fields.h" 
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>

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

// world dimensions
const int WORLD_WIDTH = 50;
const int WORLD_HEIGHT = 15;
const int WORLD_LENGTH = 50;

// tile types
const int TILETYPE_WIRE = 0;
const int TILETYPE_POSITIVE_CHARGE = 1;
const int TILETYPE_NEGATIVE_CHARGE = 2;
const int TILETYPE_INSULATOR = 3;

// physical constants
const double EPSILON_0 = 1.0;

double charge_of(int tile_type) {
    if (tile_type == TILETYPE_POSITIVE_CHARGE)
        return 1;
    
    if (tile_type == TILETYPE_NEGATIVE_CHARGE)
        return -1;

    return 0;
}

// getters for vector struct
double get_x(Vector *vec) {
    return vec->x;
}

double get_y(Vector *vec) {
    return vec->y;
}

double get_z(Vector *vec) {
    return vec->z;
}

double dot(Vector *a, Vector *b) {
    return a->x * b->x + a->y * b->y + a->z * b->z;
}

Vector zero_vector() {
    Vector r;
    r.x = 0;
    r.y = 0;
    r.z = 0;
    return r;
}

Vector scalar_mul(double c, Vector *v) {
    Vector r;
    r.x = c*v->x;
    r.y = c*v->y;
    r.z = c*v->z;
    return r;
}

Vector vec_add(Vector *a, Vector *b) {
    Vector r;

    r.x = a->x + b->x;
    r.y = a->y + b->y;
    r.z = a->z + b->z;

    return r;
}


static double magnitude(Vector *x) {
    return sqrt(dot(x,x));
}

Vector normalize(Vector *v) {
    double magnitude_v = magnitude(v);

    // test if zero vector
    if (magnitude_v == 0) {
        return *v;
    }

    return scalar_mul(1/magnitude_v, v);
}


int field_dimensions(int dim) {
    return dim*2 + 1;
} 

// define globals
static int *physical_map;

int *physical_map_tile(int i, int j, int k) {
    return TILE_AT(physical_map, i, WORLD_WIDTH, j, WORLD_HEIGHT, k, WORLD_LENGTH, 1);
}

int get_tile_physical_map(int i, int j, int k) {
    return *physical_map_tile(i,j,k);
}

void set_tile_physical_map(int i, int j, int k, int value) {
    *physical_map_tile(i,j,k) = value;
}

Vector *get_point_field(Vector *field, int i, int j, int k) {
    return TILE_AT(field, i, field_dimensions(WORLD_WIDTH), j, field_dimensions(WORLD_HEIGHT), k, field_dimensions(WORLD_LENGTH), 1);
}

Vector *get_node_field(Vector *field, int i, int j, int k) {
    return get_point_field(field, field_dimensions(i), field_dimensions(j), field_dimensions(k));
}

Vector *get_field_convolve(Vector *field, int i, int j, int k, int u, int v, int w) {
    return get_point_field(field, i*2+u, j*2+v, k*2+w);
}

static Vector *electric_field;

Vector *get_point_electric_field(int i, int j, int k) {
    return get_point_field(electric_field, i, j, k);
}

Vector *get_node_electric_field(int i, int j, int k) {
    return get_node_field(electric_field, i, j, k);
}

static int _is_running;

int is_running() {
    return _is_running;
}

void quit() {
    _is_running = 0;
}

static Vector kernel[3*3*3];

Vector *kernel_at(int i, int j, int k) {
    return TILE_AT(kernel, i, 3, j, 3, k, 3, 1);
}

static Vector *delta_field;

// run at the start of the program
void init_fields() {

    _is_running = 1;

    physical_map = malloc(sizeof(int)*WORLD_WIDTH*WORLD_HEIGHT*WORLD_LENGTH);

    LOOP_WORLD(i, j, k,
        set_tile_physical_map(i, j, k, TILETYPE_INSULATOR);
    )

    electric_field = malloc(sizeof(Vector)*field_dimensions(WORLD_WIDTH)*field_dimensions(WORLD_HEIGHT)*field_dimensions(WORLD_LENGTH));

    Vector *point = NULL;

    LOOP_FIELD(i, j, k, 
        point = get_point_electric_field(i,j,k);
        *point = zero_vector();
    )

    point = NULL;

    LOOP_KERNEL(i, j, k, 
        point = kernel_at(i, j, k);
        point->x = i - 1;
        point->y = j - 1;
        point->z = k - 1;
        Vector normalized_point = normalize(point);
        *point = scalar_mul(1.0/26.0, &normalized_point);
    )

    // delta_field is expected to be initialized by caller be for use

    delta_field = malloc(sizeof(Vector)*field_dimensions(WORLD_WIDTH)*field_dimensions(WORLD_HEIGHT)*field_dimensions(WORLD_LENGTH));


    printf("done initializing\n");
}

// destructor
// shouldn't matter because the globals are expected to last through the life time of the program
void destr_fields() {
    free(physical_map);
    free(electric_field);
    free(delta_field);
    printf("end of process\n");
}

void guass_law_electric() {
    Vector *point;

    // clear delta_field

    LOOP_FIELD(i,j,k,
        point = get_point_field(delta_field, i, j, k);
        *point = zero_vector();
    )

    // calculate how much the field would have to change

    LOOP_WORLD(i,j,k,
        double charge_density = charge_of(get_tile_physical_map(i,j,k));
        double predicted_divergence = charge_density/EPSILON_0;

        double current_divergence = 0;

        LOOP_KERNEL(u,v,w,
            double delta_dot = dot(
                kernel_at(u,v,w), 
                get_field_convolve(electric_field,
                    i,j,k,
                    u,v,w));
            current_divergence += delta_dot;
        )

        double divergence_delta = predicted_divergence - current_divergence;

        LOOP_KERNEL(u,v,w,
            Vector d_field = scalar_mul(divergence_delta*26.0, kernel_at(u,v,w));
            Vector *point = get_field_convolve(delta_field, i, j, k, u, v, w);
            *point = vec_add(point, &d_field);
        )
    )

    // apply the field

    LOOP_FIELD(i,j,k,
        Vector *point = get_point_electric_field(i,j,k);
        *point = vec_add(point, get_point_field(delta_field,i,j,k));
    )

    LOOP_WORLD(i,j,k,
        Vector average = zero_vector();

        LOOP_KERNEL(u,v,w,
            if(u == 0 && v == 0 && w == 0) {
                continue;
            }
            average = vec_add(&average, get_field_convolve(electric_field,i,j,k,u,v,w));
        )

        average = scalar_mul(1.0/26.0, &average);

        *get_node_electric_field(i,j,k) = average;
    )

    printf("gauss\n");
}