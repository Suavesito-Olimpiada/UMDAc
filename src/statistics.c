#include "statistics.h"

void statistics_get_mu_si_dim(unsigned const population, unsigned const dimension,
                   double **data, double *mu, double *sigma)
{
    /* validate population and pointers */
    if (!mu || !sigma || !data || !population)
        return;

    unsigned i = 0;
    double av = 0.0, sd = 0.0;
    for (i = 0; i < population; ++i) {
        av += data[i][dimension];
    }
    av /= (double)population;
    for (i = 0; i < dimension; ++i) {
        sd += pow(data[i][dimension] - av, 2.0);
    }
    sd /= (double)population;
    sd = sqrt(sd);

    *mu = av;
    *sigma = sd;
}
