#ifndef STATISTICS_H
#define STATISTICS_H 1

#include <math.h>

/* get the mu and sigma of some population in the dimension dim */
void statistics_get_mu_si_dim(unsigned const population, unsigned const dimension,
                   double **data, double *mu, double *sigma);

#endif /* end of include guard: STATISTICS_H */
