#include "process.h"
#include "kernel.h"
#include "accessor.h"
#include "world_constants.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>
#include "field.h"

void *process_field(void *arg);

int debug_x = 0;
int debug_y = 0;
int debug_z = 0;

void set_debug_x(int x) {
    debug_x = x;
}

void set_debug_y(int y) {
    debug_y = y;
}

void set_debug_z(int z) {
    debug_z = z;
}


#define INIT_SCALAR_KERNEL(GETTER, AXIS) \
\
LOOP_KERNEL(i, j, k, \
    *GETTER(i, j, k) = (AXIS - 1.0) / 9.0; \
)\

// game state

static int _is_running;

int is_running() {
    return _is_running;
}

void quit() {
    _is_running = 0;
}

// multi variable calculus functions

double get_current_divergent(Vector *field, int i, int j, int k) {
    double current_divergence = 0;

    LOOP_KERNEL(u,v,w,
        double delta_dot = dot(
            kernel_vec_at(u,v,w), 
            get_field_convolve(field,
                i,j,k,
                u,v,w));
        current_divergence += delta_dot;
    )

    return current_divergence;
}

Vector get_current_curl(Vector *field, int i, int j, int k) {
    Vector current_curl = zero_vector();

    LOOP_KERNEL(u,v,w,
        Vector delta_cross = cross(
            kernel_vec_at(u,v,w), 
            get_field_convolve(field,
                i,j,k,
                u,v,w));
        current_curl = vec_add(&current_curl, &delta_cross);
    )

    return current_curl;
}

// update field using delta field

void update_field(Vector *field) {
    LOOP_FIELD(i,j,k,
        Vector *point = get_point_field(field,i,j,k);
        *point = vec_add(point, get_point_field(delta_vec_padded_field_data(),i,j,k));
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

// constructor and destructor

// run at the start of the program
void init_fields() {

    _is_running = 1;

    alloc_fields();

    LOOP_WORLD(i, j, k,
        set_tile_physical_map(i, j, k, TILETYPE_INSULATOR);
    )

    clear_electric_field();
    clear_magnetic_field();

    Vector *point = NULL;

    // LOOP_WORLD(i,j,k,
    //     point = get_current_field(i, j, k);
    //     *point = zero_vector();
    // )

    // point = NULL;

    // clear_charge_field();

    // vec kernel
    LOOP_KERNEL(i, j, k, 
        point = kernel_vec_at(i, j, k);
        point->x = i - 1;
        point->y = j - 1;
        point->z = k - 1;
        Vector normalized_point = normalize(point);
        *point = scalar_mul(1.0/26.0, &normalized_point);
    )

    // various scalar kernels

    INIT_SCALAR_KERNEL(kernel_scalar_x_at, i)
    INIT_SCALAR_KERNEL(kernel_scalar_y_at, j)
    INIT_SCALAR_KERNEL(kernel_scalar_z_at, k)

    LOOP_KERNEL(i, j, k,
        *kernel_scalar_disperse_at(i, j, k) = i == 1 && j == 1 && k == 1 ? -1.0 : 1.0/26.0;
    )

    // this thread applies the maxwell equations to the fields

    pthread_create(&process_field_thread, NULL, &process_field, NULL);


    printf("done initializing\n");
}

// destructor
// shouldn't matter because the globals are expected to last through the life time of the program
void destr_fields() {
    pthread_join(process_field_thread, NULL);

    free_fields();

    printf("end of process\n");
}

// field functions

void guass_law_electric() {

    // clear delta_field

    clear_delta_vec_padded_field();

    // calculate how much the field would have to change

    LOOP_WORLD(i,j,k,
        double charge_density = charge_of(get_tile_physical_map(i,j,k));

        double predicted_divergence = charge_density/EPSILON_0;
        double current_divergence = get_current_divergent(electric_field_data(),i,j,k);

        double divergence_delta = predicted_divergence - current_divergence;

        LOOP_KERNEL(u,v,w,
            Vector d_field = scalar_mul(divergence_delta*26.0, kernel_vec_at(u,v,w));
            Vector *point = get_field_convolve(delta_vec_padded_field_data(), i, j, k, u, v, w);
            *point = vec_add(point, &d_field);
        )
    )

    // apply the field

    update_field(electric_field_data());
}

// this is the optimal for curl to reach equilibrium
#define CURL_SCALE_VALUE 0.3 


void ampere_law() {

    // clear delta_field

    clear_delta_vec_padded_field();

    // calculate how much the field would have to change

    LOOP_WORLD(i,j,k,
        Vector current_density = current_of(get_tile_physical_map(i,j,k));
        Vector current_density_adjusted = scalar_mul(-1, &current_density); // so that the curl can comply with the right hand rule


        Vector predicted_curl = scalar_mul(MU_0, &current_density_adjusted);
        Vector current_curl = get_current_curl(magnetic_field_data(),i,j,k);

        Vector curl_delta = vec_sub(&predicted_curl, &current_curl);
        Vector curl_delta_scaled = scalar_mul(26.0 * (3.0 - CURL_SCALE_VALUE) / (2.0 + CURL_SCALE_VALUE), &curl_delta);\

        LOOP_KERNEL(u,v,w,
            Vector d_field = cross(&curl_delta_scaled, kernel_vec_at(u,v,w));
            Vector *point = get_field_convolve(delta_vec_padded_field_data(), i, j, k, u, v, w);
            *point = vec_add(point, &d_field);
        )
    )

    // apply the field

    update_field(magnetic_field_data());
}



// (p/m)*(nE + I x B - r I)
// void update_current() {
//     Vector *point = NULL;

//     clear_delta_vec_basic_field();

//     LOOP_WORLD(i, j, k,
//         double R = resistance_of(get_tile_physical_map(i, j, k));
//         point = get_delta_vec_basic_field(i, j, k);

//         if(R == -1) {
//             *point = zero_vector();
//             continue;
//         }

//         Vector *E = get_node_electric_field(i, j, k);
//         Vector *I = get_current_field(i, j, k);
//         Vector electric_term = scalar_mul(MOVABLE_PARTICLE_DENSITY, E);
//         Vector resistance_term = scalar_mul(-R, I);
//         Vector sum = vec_add(&electric_term, &resistance_term);
//         Vector result = scalar_mul(MOVABLE_PARTICLE_CHARGE/MOVABLE_PARTICLE_MASS, &sum);

//         *point = result;
//     )

//     LOOP_WORLD(i, j, k,
//         Vector *delta = get_delta_vec_basic_field(i, j, k);
//         point = get_current_field(i, j, k);
//         *point = vec_add(point, delta);
//     )
// }

// void update_charge() {
//     Vector *point = NULL;

//     clear_delta_vec_padded_field();

//     LOOP_WORLD(i, j, k,
//         point = get_current_field(i,j,k);

//         LOOP_KERNEL(u,v,w,
//             *get_point_delta_float_padded_field(i*2+u, j*2+v, k*2+w) 
//                 += point->x * *kernel_scalar_x_at(u, v, w)
//                 + point->y * *kernel_scalar_y_at(u, v, w)
//                 + point->z * *kernel_scalar_z_at(u, v, w); 
            
//             *get_point_delta_float_padded_field(i + u - 1, j + v - 1, k + w - 1) 
//                 += *get_node_charge_field(i, j, k) * CHARGE_TRANSFER_COEF * *kernel_scalar_disperse_at(u,v,w);
//         )
//     )

//     LOOP_FIELD(i, j, k,
//         *get_point_charge_field(i, j, k) += *get_point_delta_float_padded_field(i,j,k);
//     )

//     LOOP_WORLD(i, j, k,
//         double average = 0.0;
        
//         LOOP_KERNEL(u,v,w,
//             if(u == 0 && v == 0 && w == 0) {
//                 continue;
//             }
//             average += *get_point_charge_field(i*2+u, j*2+v, k*2+w);
//         )

//         average /= 26.0;

//         if(average > 1.0)
//             average = 1.0;
//         else if(average < -1.0)
//             average = -1.0;

//         *get_node_charge_field(i, j, k) = average;
//     )
// }

void *process_field(void *arg) {
    while(is_running()) {
        guass_law_electric();
        ampere_law();
        // update_current();
        // update_charge();
    }

    pthread_exit(NULL);
}
