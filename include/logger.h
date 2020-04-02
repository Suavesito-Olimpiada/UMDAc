#ifndef LOGGER_H
#define LOGGER_H 1

#include <stdlib.h>
#include <string.h>

/*! \struct logger_t
 *  \brief Logger of any data
 *
 *  This struct maintains a general logger for results of functions.
 */
typedef struct {
    unsigned counter; /*!< counter of elements */
    unsigned size;    /*!< size of each element */
    void *data;       /*!< data of the logger */
} logger_t;

logger_t *logger_new();
void logger_free(logger_t *logger);

int logger_init(logger_t *logger, unsigned const size);
void logger_deinit(logger_t *logger);

/* swap values on data */
int logger_swap(logger_t *logger);

/* push element content to the botom */
int logger_push(logger_t *logger, void *element);
/* show data content in the botom throw element */
int logger_front(logger_t *logger, void *element);
/* pop element content in the botom and assign it to element
 *
 * freed the memory of logger just if freed is true
 */
int logger_pop(logger_t *logger, void *element, char freed);

/* is empty the logger data? */
int logger_empty(logger_t *logger);

#endif /* end of include guard: LOGGER_H */
