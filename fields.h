#ifndef MAXWELL_EQUARIONS_FIELDS_H
#define MAXWELL_EQUARIONS_FIELDS_H

// world dimensions
extern const int WORLD_WIDTH;
extern const int WORLD_HEIGHT;
extern const int WORLD_LENGTH;

// tile types
extern const int TILETYPE_WIRE;
extern const int TILETYPE_POSITIVE_CHARGE;
extern const int TILETYPE_NEGATIVE_CHARGE;
extern const int TILETYPE_INSULATOR;

typedef union Vector {
    struct {
        int x;
        int y;
        int z;
    };
    int array[3];
} Vector;

extern int get_x(Vector *vec);
extern int get_y(Vector *vec);
extern int get_z(Vector *vec);


extern int get_y(Vector *vec);

#endif // !MAXWELL_EQUARIONS_FIELDS_H