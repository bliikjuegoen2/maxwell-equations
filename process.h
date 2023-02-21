#ifndef MAXWELL_EQUARIONS_FIELDS_H
#define MAXWELL_EQUARIONS_FIELDS_H

// program status
extern int is_running();
extern void quit();

extern void init_fields();
void destr_fields();

extern void set_debug_x(int x);
extern void set_debug_y(int y);
extern void set_debug_z(int z);

#endif // !MAXWELL_EQUARIONS_FIELDS_H