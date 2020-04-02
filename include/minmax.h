#ifndef MINMAX_H
#define MINMAX_H 1

#include <stdarg.h>
#include <stdlib.h>

double **minmax_new(unsigned dim);
void minmax_free(double **);

/* varargs function to create minmax boxes */
double **minmax_make_args(unsigned, ...);
/* n-dimentional box with [min, max] ^ dim shape */
double **minmax_make_box(unsigned, double, double);

#endif /* end of include guard: MINMAX_H */
