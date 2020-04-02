/*
 *  autor       : Suavesito-Olimpiada
 *  date        : 12-10-2019
 *  subject     :
 *  description : Description of the project.
 *  license     : GPL v3 (https://www.gnu.org/licenses/gpl-3.0.en.html)
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "umdac.h"

double spheref(unsigned, double *);

int main()
{
    srand(time(NULL));

    /* we will be searching for the solution in the 3D sphere function */
    unsigned const dim = 3;
    /* our search volume will be the [-5.12, 5.12]^3 box */
    double const min = -5.12;
    double const max = 5.12;

    /* we will use a population of 100 individuals.
     *
     * NOTE: when we increase the dimension we (in general) NEED to increase
     * the size of the population, this is beacuse we will have (in the same
     * edge sized box) d^n hyper-volume, and so a much bigger search space to
     * find the solution.
     *
     * Experimentally, we can have linear increase in the population with
     * respect to the dimensions.
     */
    unsigned const population = 100;
    /* happens the same as with the population */
    unsigned const active = 20;

    /* if you don't want to increase the population, let say beacause you are
     * running this in an embedded device and you have constrains with memory,
     * you still can achieve the same results in bigger dimentions, increassing
     * the number of iterations.
     *
     * Experimentally, this increase linearly (or quasi-linearly) as the
     * dimensions increase.
     */
    unsigned const iterations = 1000;
    /* experimentally this is a good ratio of convergence (?) */
    double const tolerance = pow(2.5, (double)dim);

    /* ask for memory for our UMDAc problem */
    umdac_t *sphere = umdac_new();
    if (!sphere)
        goto f;

    /* make a box of [min, max]^dim for our original search space */
    double **minmax = minmax_make_box(dim, min, max);
    if (!minmax)
        goto f1;

    /* initialize our UMDAc problem */
    if (umdac_init(sphere, dim, minmax, population, active, spheref))
        goto f2;

    /* ask for memory for our logger */
    logger_t *logger = logger_new();
    if (!logger)
        goto f3;

    /* initialize our logger */
    logger_init(
        logger,
        sizeof(double)); /* this will not fail if logger_new didn't fail */

    /* run the UMDAc proble!m */
    int v = umdac_run(sphere, tolerance, iterations, logger);
    if (v == 0) {
        printf("Success!(?)\n");
    }
    else if (v == -4) {
        printf(
            "Success!(?\?)\n"); /* The scape code is to not trigger trigraph */
    }
    /* this last failure can just happen is we are unable to push a value to
     * logger*/
    else {
        printf("Failure!!(??\?)\n"); /* The scape code is to not trigger
                                        trigraph */
        goto f4;
    }

    /* the data was pushed in secuential order, lets swap the content to have
     * the more recent weights first
     */
    logger_swap(logger);

    FILE *fp = fopen("../test/out/data.txt", "w");
    if (!fp)
        goto f4;

    unsigned i = 0;
    double w = 0.0;
    /* until it is empty */
    while (!logger_empty(logger)) {
        logger_pop(logger, &w,
                   0); /* this will not fail as freed is set to false */

        ++i;
        /* print our logger data */
        /* printf("%d :\t%lf\n", i, w); */

        /* and to a txt file, I could write a spetialize funtion for this, but
         * it is too much for me right now
         */
        fprintf(fp, "%d :\t%lf\n", i, w);
    }

    fclose(fp);

f4:
    logger_deinit(logger);
    logger_free(logger);
f3:
    /* we can get rid of the sphere UMDAc instance */
    umdac_deinit(sphere);
f2:
    /* get rid of the minmax memory, the data was copied into sphere->minmax */
    minmax_free(minmax);
f1:
    umdac_free(sphere);
f:
    return 0; /* I didn't added more error checking for the system because I was
                 tired */
}

double spheref(unsigned dim, double *val)
{
    double res = 0.0;
    while (dim) {
        res += pow(val[--dim], 2.0);
    }
    return res;
}
