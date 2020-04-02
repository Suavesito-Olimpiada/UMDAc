#ifndef UMDAC_H
#define UMDAC_H 1

#include <math.h>
#include <stdlib.h>

#include "logger.h"
#include "minmax.h"
#include "random.h"
#include "statistics.h"

/*! \struct umdac_t
 *  \brief Type for UMDAc problem description
 *
 *  Data type that represent a UMDAc problem.
 */
typedef struct {
    double (*f)(unsigned, double *); /*!< function */
    unsigned population;             /*!< population */
    unsigned active;                 /*!< active population */
    double **minmax;                 /*!< dimension x 2 min and max */
    unsigned dimension;              /*!< dimension */
    double **data;                   /*!< data of the population
                                          as a population x dimension */
    double *origin;                  /*!< it is necessary to keep track
                                          of the begin of data,
                                          and eventually free it */
} umdac_t;

extern umdac_t *umdac_new();
extern void umdac_free(umdac_t *umdac);

/* initialize the UMDAc problem,
 *  - `dimension` is the dimension in wich our problem remains
 *  - `minmax` is a matrix describing the hyper-box that we will be
 *     using originally for our search
 *  - `population` is the size of the population we will generate
 *  - `active` is the population we will take account to stimate the
 *     distribution of the function
 *  - `f` is the weight functino of our problem
 */
extern int umdac_init(umdac_t *umdac, unsigned const dimension, double **minmax,
                      unsigned const population, unsigned const active,
                      double (*f)(unsigned, double *));
extern void umdac_deinit(umdac_t *umdac);

/* get the next status of our UMDAc problem */
extern int umdac_next(umdac_t *umdac);
/* This run through status of our UMDAc problem until the stopping criterion is
 * met, the stopping criterion of this implementation can be read in the
 * umdac.c file.
 *
 * TODO: Allow pass a final argument to change the stopping criterion, maybe
 *       we can add this as a function pointer to our umdac_t type and here
 *       just check if the function return something less that our tolerance.
 */
extern int umdac_run(umdac_t *umdac, double tolerance, unsigned const max_run,
                     logger_t *logger);

#endif /* end of include guard: UMDAC_H */
