#ifndef RANDOM_H
#define RANDOM_H 1

#include <stdlib.h>
#include <math.h>

/* "random" [0,1) numbers.
 * this is not a correct algorithm, but it simple
 */
extern double random_uniform();

/* "random" [a,b) numbers. */
extern double random_uniform_ab(double, double);

/* "random" normal(0, 1) distribution samples
 * based in the [Marsaglia polar method](https://www.wikiwand.com/en/Marsaglia_polar_method)
 */
extern double random_normal();

/* "random" normal(mu, si) distribution samples */
extern double random_normal_mu_si(double, double);

#endif //TEST_RANDOM_RANDOMSTL_H


