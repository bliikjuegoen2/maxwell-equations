#include "process.h"
#include "kernel.h"
#include "accessor.h"
#include "world_constants.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>

void *process_field(void *arg);

Vector *alloc_vec_field() {
    return malloc(sizeof(Vector)*field_dimensions(WORLD_WIDTH)*field_dimensions(WORLD_HEIGHT)*field_dimensions(WORLD_LENGTH));
}

#define DEF_SETGET_FIELD(FIELD_NAME, TYPE) \
\
TYPE get_tile_##FIELD_NAME(int i, int j, int k) { \
    return *get_##FIELD_NAME(i,j,k); \
} \
\
void set_tile_##FIELD_NAME(int i, int j, int k, TYPE value) {\
    *get_##FIELD_NAME(i,j,k) = value;\
}\

#define DEF_BASIC_FIELD(FIELD_NAME, TYPE) \
\
TYPE *FIELD_NAME;\
\
TYPE *get_##FIELD_NAME(int i, int j, int k) {\
    return TILE_AT(\
        FIELD_NAME\
        , ABS_MOD(i, WORLD_WIDTH), WORLD_WIDTH\
        , ABS_MOD(j, WORLD_HEIGHT), WORLD_HEIGHT\
        , ABS_MOD(k,WORLD_LENGTH), WORLD_LENGTH\
        , 1);\
}\

#define DEF_PADDED_FIELD(FIELD_NAME, TYPE) \
TYPE *FIELD_NAME; \
\
TYPE *get_point_##FIELD_NAME(int i, int j, int k) {\
    return get_point_field(FIELD_NAME, i, j, k);\
}\
\
TYPE *get_node_##FIELD_NAME(int i, int j, int k) {\
    return get_node_field(FIELD_NAME, i, j, k);\
}\

// define globals

DEF_BASIC_FIELD(physical_map, int)
DEF_SETGET_FIELD(physical_map, int)
DEF_PADDED_FIELD(electric_field, Vector)
DEF_BASIC_FIELD(current_field, Vector)

void clear_field(Vector *field) {
    Vector *point = NULL;

    LOOP_FIELD(i,j,k,
        point = get_point_field(field, i, j, k);
        *point = zero_vector();
    )
}

double get_current_divergent(Vector *field, int i, int j, int k) {
    double current_divergence = 0;

    LOOP_KERNEL(u,v,w,
        double delta_dot = dot(
            kernel_at(u,v,w), 
            get_field_convolve(field,
                i,j,k,
                u,v,w));
        current_divergence += delta_dot;
    )

    return current_divergence;
}

static int _is_running;

int is_running() {
    return _is_running;
}

void quit() {
    _is_running = 0;
}

static Vector *delta_field;


void clear_delta_field() {
    clear_field(delta_field);
}

void update_field(Vector *field) {
    LOOP_FIELD(i,j,k,
        Vector *point = get_point_field(field,i,j,k);
        *point = vec_add(point, get_point_field(delta_field,i,j,k));
    )

    LOOP_WORLD(i,j,k,
        Vector average = zero_vector();

        LOOP_KERNEL(u,v,w,
            if(u == 0 && v == 0 && w == 0) {
                continue;
            }
            average = vec_add(&average, get_field_convolve(field,i,j,k,u,v,w));
        )

        average = scalar_mul(1.0/26.0, &average);

        *get_node_field(field,i,j,k) = average;
    )
}

pthread_t process_field_thread;

// run at the start of the program
void init_fields() {

    _is_running = 1;

    physical_map = malloc(sizeof(int)*WORLD_WIDTH*WORLD_HEIGHT*WORLD_LENGTH);

    LOOP_WORLD(i, j, k,
        set_tile_physical_map(i, j, k, TILETYPE_INSULATOR);
    )

    electric_field = alloc_vec_field();

    clear_field(electric_field);

    current_field = malloc(sizeof(Vector)*WORLD_WIDTH*WORLD_HEIGHT*WORLD_LENGTH);

    Vector *point = NULL;

    LOOP_WORLD(i,j,k,
        point = get_current_field(i, j, k);
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

    delta_field = alloc_vec_field();

    // this thread applies the maxwell equations to the fields

    pthread_create(&process_field_thread, NULL, &process_field, NULL);


    printf("done initializing\n");
}

// destructor
// shouldn't matter because the globals are expected to last through the life time of the program
void destr_fields() {
    pthread_join(process_field_thread, NULL);

    free(physical_map);
    free(electric_field);
    free(current_field);
    free(delta_field);

    printf("end of process\n");
}

void guass_law_electric() {
    Vector *point = NULL;

    // clear delta_field

    clear_delta_field();

    // calculate how much the field would have to change

    LOOP_WORLD(i,j,k,
        double charge_density = charge_of(get_tile_physical_map(i,j,k));
        double predicted_divergence = charge_density/EPSILON_0;
        double current_divergence = get_current_divergent(electric_field,i,j,k);

        double divergence_delta = predicted_divergence - current_divergence;

        LOOP_KERNEL(u,v,w,
            Vector d_field = scalar_mul(divergence_delta*26.0, kernel_at(u,v,w));
            Vector *point = get_field_convolve(delta_field, i, j, k, u, v, w);
            *point = vec_add(point, &d_field);
        )
    )

    // apply the field

    update_field(electric_field);
}


// (p/m)*(nE + I x B - r I)
void update_current() {
    Vector *point = NULL;

    LOOP_WORLD(i, j, k,
        point = get_node_field(delta_field, i, j, k);
        *point = zero_vector();
    )

    LOOP_WORLD(i, j, k,
        Vector *E = get_node_electric_field(i, j, k);
        Vector *I = get_current_field(i, j, k);
        Vector electric_term = scalar_mul(MOVABLE_PARTICLE_DENSITY, E);
        Vector resistance_term = scalar_mul(-WIRE_RESISTANCE, I);
        Vector sum = vec_add(&electric_term, &resistance_term);
        Vector result = scalar_mul(MOVABLE_PARTICLE_CHARGE/MOVABLE_PARTICLE_MASS, &sum);

        point = get_node_field(delta_field, i, j, k);
        *point = result;
    )

    LOOP_WORLD(i, j, k,
        Vector *delta = get_node_field(delta_field, i, j, k);
        point = get_current_field(i, j, k);
        *point = vec_add(point, delta);
    )
}

void *process_field(void *arg) {
    while(is_running) {
        guass_law_electric();
        update_current();
    }

    pthread_exit(NULL);
}
