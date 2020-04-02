#include "logger.h"

logger_t *logger_new()
{
    logger_t *new = (logger_t *)malloc(sizeof(logger_t));
    if (!new)
        return NULL;

    new->counter = 0;
    new->size = 0;
    new->data = NULL;

    return new;
}

void logger_free(logger_t *logger) { free(logger); }

int logger_init(logger_t *logger, unsigned const size)
{
    if (!logger || !size)
        return -1;

    logger->size = size;

    return 0;
}

void logger_deinit(logger_t *logger)
{
    if (!logger)
        return;

    logger->size = 0;
    logger->counter = 0;
    free(logger->data);
}

int logger_swap(logger_t *logger)
{
    if (!logger)
        return -1;

    char *data = logger->data;
    char *swap = (char *)malloc(logger->size);
    if (!swap)
        return -1;

    unsigned i = 0;
    for (i = 0; i < (unsigned)(logger->counter / 2); ++i) {
        /* strict aliase rules in C enforce us to use memcpy */
        memcpy(swap, data + i * logger->size, logger->size);
        memcpy(data + i * logger->size,
               data + (logger->counter - i - 1) * logger->size, logger->size);
        memcpy(data + (logger->counter - i - 1) * logger->size, swap,
               logger->size);
    }
    free(swap);

    return 0;
}

int logger_push(logger_t *logger, void *element)
{
    if (!logger || !element)
        goto f;

    logger->counter += 1;

    void *new = (void *)realloc(logger->data, logger->size * logger->counter);
    if (!new)
        goto f1;

    logger->data = new;

    char *data = logger->data;
    /* strict aliase rules in C enforce us to use memcpy */
    memcpy(data + (logger->counter - 1) * logger->size, element, logger->size);

    return 0;

f1:
    logger->counter -= 1;
f:
    return -1;
}

int logger_front(logger_t *logger, void *element)
{
    if (!logger || !element)
        return -1;

    char *data = logger->data;
    memcpy(element, data + (logger->counter - 1) * logger->size, logger->size);

    return 0;
}

int logger_pop(logger_t *logger, void *element, char freed)
{
    if (!logger || !element)
        goto f;

    if (!logger->counter || !logger->data)
        goto f;

    char *data = logger->data;
    memcpy(element, data + (logger->counter - 1) * logger->size, logger->size);

    logger->counter -= 1;

    if (freed) {
        void *new =
            (void *)realloc(logger->data, logger->size * logger->counter);
        if (!new && logger->counter)
            goto f1;

        logger->data = new;
    }

    return 0;

f1:
    memset(element, 0, logger->size);
    logger->counter += 1;
f:
    return -1;
}

int logger_empty(logger_t *logger)
{
    if (!logger)
        return 1;

    return !(logger->counter);
}
