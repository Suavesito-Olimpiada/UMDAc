#include "minmax.h"

double **minmax_new(unsigned dim)
{
    double **minmax = (double **)malloc(sizeof(double *) * dim);
    if (!minmax)
        goto f;

    minmax[0] = (double *)malloc(sizeof(double) * dim * 2);
    if (!minmax[0])
        goto f1;

    unsigned i = 0;
    for (i = 0; i < dim; ++i) {
        minmax[i] = minmax[0] + 2 * i;
    }

    return minmax;

f1:
    free(minmax);
f:
    return NULL;
}

void minmax_free(double **minmax)
{
    if (minmax[0]) {
        free(minmax[0]);
    }
    free(minmax);
}

double **minmax_make_args(unsigned dim, ...)
{
    double **minmax = minmax_new(dim);
    if (!minmax)
        return NULL;

    unsigned i = 0;
    va_list args;
    va_start(args, dim);
    for (i = 0; i < dim; ++i) {
        minmax[i][0] = va_arg(args, double);
        minmax[i][1] = va_arg(args, double);
    }
    va_end(args);

    return minmax;
}

double **minmax_make_box(unsigned dim, double min, double max)
{
    double **minmax = minmax_new(dim);
    if (!minmax)
        return NULL;

    unsigned i = 0;
    for (i = 0; i < dim; ++i) {
        minmax[i][0] = min;
        minmax[i][1] = max;
    }

    return minmax;
}
