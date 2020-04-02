#include "umdac.h"
#include "statistics.h"

/* constext to cmp, comparison function does not accept context until C11, so we
 * pass context in the global varibles.
 *
 * NOTICE: that this variables (as all globals) are initialize to NULL and 0
 * respectively.
 */
double (*f_global)(unsigned, double *);
int dimension_global;

umdac_t *umdac_new()
{
    umdac_t *new = NULL;
    new = (umdac_t *)malloc(sizeof(umdac_t));
    if (!new)
        goto f;

    new->f = NULL;
    new->population = 0;
    new->active = 0;
    new->dimension = 0;
    new->minmax = NULL;
    new->data = NULL;
    new->origin = NULL;

f:
    return new;
}

void umdac_free(umdac_t *umda) { free(umda); }

int umdac_cmp(const void *a, const void *b)
{
    double **_a = (double **)a;
    double **_b = (double **)b;

    return (f_global(dimension_global, *_a) > f_global(dimension_global, *_b)) -
           (f_global(dimension_global, *_a) < f_global(dimension_global, *_b));
}

int umdac_init(umdac_t *umdac, unsigned const dimension, double **minmax,
               unsigned const population, unsigned const active,
               double (*f)(unsigned, double *))
{
    /* Validate population and active elements*/
    if (population < active || population == 0 || !umdac)
        return -1;

    umdac->dimension = dimension;
    umdac->population = population;
    umdac->active = active;
    umdac->f = f;

    umdac->minmax = (double **)malloc(sizeof(double *) * dimension);
    if (!umdac->minmax)
        goto f;

    umdac->minmax[0] = (double *)malloc(sizeof(double) * 2 * dimension);
    if (!umdac->minmax[0])
        goto f1;

    unsigned i = 0;
    for (i = 0; i < dimension; ++i) {
        umdac->minmax[i] = umdac->minmax[0] + 2 * i;
    }

    /* Copy limits of min-max */
    for (i = 0; i < dimension; ++i) {
        umdac->minmax[i][0] = minmax[i][0];
        umdac->minmax[i][1] = minmax[i][1];
    }

    umdac->data = (double **)malloc(sizeof(double *) * population);
    if (!umdac->data)
        goto f2;

    umdac->data[0] = (double *)malloc(sizeof(double) * dimension * population);
    if (!umdac->data[0])
        goto f3;

    umdac->origin = umdac->data[0];

    for (i = 0; i < population; ++i) {
        umdac->data[i] = umdac->data[0] + dimension * i;
    }

    unsigned j = 0;
    for (i = 0; i < population; ++i) {
        for (j = 0; j < dimension; ++j) {
            umdac->data[i][j] =
                random_uniform_ab(umdac->minmax[j][0], umdac->minmax[j][1]);
        }
    }

    f_global = umdac->f;
    dimension_global = umdac->dimension;

    /* sort the population */
    qsort(umdac->data, umdac->population, sizeof(double *), umdac_cmp);

    f_global = NULL;
    dimension_global = 0;

    return 0; /* ran with exit */

/* for error correction this section */
f3:
    free(umdac->data);
f2:
    free(umdac->minmax[0]);
f1:
    free(umdac->minmax);
f:
    return -1;
}

void umdac_deinit(umdac_t *umdac)
{
    if (!umdac)
        return;

    umdac->dimension = 0;
    umdac->population = 0;
    umdac->active = 0;
    umdac->f = NULL;

    free(umdac->minmax[0]);
    free(umdac->minmax);
    umdac->minmax = NULL;

    free(umdac->origin);
    umdac->origin = NULL;
    free(umdac->data);
    umdac->data = NULL;
}

int umdac_next(umdac_t *umdac)
{
    if (!umdac)
        return -1;

    if (!umdac->data)
        return -1;

    /* this is to add variability to the data and avoid getting stuck in some
     * "hot point"
     *
     * I decided to add this after debugging and finding that in some cases the
     * standard deviation `sd` get a value of zero, just making the whole
     * process stuck in some value `av`.
     *
     * NOTICE: that this variability get smaller every time, as the iterations
     * go to infinity, and this gives the solutions stability as the iterations
     * increase
     */
    static double iteration;
    iteration += 1;
    f_global = umdac->f;
    dimension_global = umdac->dimension;

    /* sort the population, notice that "sorting" is just moving arround the
     * pointers to the vectors of doubles */
    qsort(umdac->data, umdac->population, sizeof(double *), umdac_cmp);

    f_global = NULL;
    dimension_global = 0;

    unsigned i = 0, j = 0;
    double av = 0.0, sd = 0.0;
    for (i = 0; i < umdac->dimension; ++i) {
        /* get the distribution sigma and mu of data */
        statistics_get_mu_si_dim(umdac->active, i, umdac->data, &av, &sd);

        for (j = umdac->active; j < umdac->population; ++j) {
            /* re-assign the last non-actve elements with N(mu, sigma)
             * distribution
             */
            umdac->data[j][i] = random_normal_mu_si(av, sd + 1.0 / iteration);
        }
    }

    return 0;
}

int umdac_run(umdac_t *umdac, double tolerance, unsigned const max_run,
              logger_t *logger)
{
    if (!umdac || !max_run || (tolerance < 0.0))
        return -1; /* wring parameters in function */

    if (!umdac->data)
        return -2; /* void data */

    double w = umdac->f(umdac->dimension, umdac->data[0]);
    double wp = w;
    if (logger)
        if (logger_push(logger, &w))
            return -3;

    unsigned i = 0;
    for (i = 0; i < max_run; ++i) {
        umdac_next(umdac); /* cannot fail in this point */

        w = umdac->f(umdac->dimension, umdac->data[0]);
        if (logger)
            if (logger_push(logger, &w))
                return -3; /* error pushing in logger */

        /* Stop the algorithm when the absolute difference divided by the bigger
         * of them, between two consecutive different values is less than the
         * inverse of a tolerance. This can be interpreted as the population not
         * being improved with a velocty bigger than that tolerance.
         *
         * The `w != wp` is to avoid a false positive if the population was not
         * improved.
         *
         * A better stop stop criteria could be when the joint standard
         * deviation is less then a tolerance, this can be interpreted as the
         * population not being improved any more in that region.
         */
        if (fabs((wp - w) / (w > wp ? w : wp)) < 1.0 / tolerance && wp != w)
            return 0;

        wp = w;
    }

    return -4; /* max number of runs, is not technically an error, but it must
                  be flagged to know if the algorithm "fails" */
}
