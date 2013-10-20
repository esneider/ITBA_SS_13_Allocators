#include <stdio.h>
#include <stdlib.h>
#include "context.h"
#include "talloc.h"


static void error(void *parent) {

    tfree(parent);
    printf("There was an error while loading context. Aborting.\n");
    exit(3);
}


static struct context *new_context(void *parent, size_t size) {

    struct context *context = talloc(sizeof(*context), parent);

    if (!context) error(parent);

    context->num_buckets = size;
    context->size_hist = talloc(size * sizeof(double), context);
    context->life_hist = tcalloc(size * sizeof(double*), context);
    context->allocs_markov = tcalloc(size * sizeof(double*), context);

    if (!context->size_hist || !context->life_hist || !context->allocs_markov) {

        error(parent);
    }

    for (size_t i = 0; i < size; i++) {

        context->life_hist[i] = talloc(size * sizeof(double), context->life_hist);
        context->allocs_markov[i] = talloc(size * sizeof(double), context->allocs_markov);

        if (!context->life_hist[i] || !context->allocs_markov[i]) {

            error(parent);
        }
    }

    return context;
}


struct context* load_context(void *parent, const char *name) {

    FILE *f = fopen(name, "r");

    if (!f) error(parent);

    size_t size;
    fscanf(f, "%zu", &size);

    struct context *context = new_context(parent, size);

    for (size_t i = 0; i < size; i++) {
        fscanf(f, "%lf", context->size_hist + i);
    }

    fscanf(f, "%zu", &context->allocs_initial);

    for (size_t i = 0; i < size; i++) {
        for (size_t j = 0; j < size; j++) {
            fscanf(f, "%lf", context->allocs_markov[i] + j);
        }
    }

    for (size_t i = 0; i < size; i++) {
        for (size_t j = 0; j < size; j++) {
            fscanf(f, "%lf", context->life_hist[i] + j);
        }
    }

    return context;
}
