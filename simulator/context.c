#include <stdio.h>
#include <stdlib.h>
#include "context.h"


static void error(void) {

    printf("There was an error while loading context. Aborting.\n");
    exit(3);
}


void free_context(struct context *context) {

    if (context) {

        if (context->life_hist) {
            for (int i = 0; i < context->num_buckets; i++) {
                free(context->life_hist[i]);
            }
        }

        free(context->size_hist);
        free(context->life_hist);
        free(context);
    }
}


static struct context *new_context(size_t size) {

    struct context *context = malloc(sizeof(*context));

    if (!context) error();

    context->num_buckets = size;
    context->size_hist = malloc(size * sizeof(double));
    context->life_hist = malloc(size * sizeof(double*));

    if (!context->size_hist || !context->life_hist) {

        free_context(context);
        error();
    }

    for (int i = 0; i < size; i++) {

        context->life_hist[i] = malloc(size * sizeof(double));

        if (!context->life_hist[i]) {

            free_context(context);
            error();
        }
    }

    return context;
}


struct context* load_context(const char *name) {

    FILE *f = fopen(name, "r");

    if (!f) error();

    size_t size;
    fscanf(f, "%zu", &size);

    struct context *context = new_context(size);

    for (int i = 0; i < size; i++) {
        fscanf(f, "%f", context->size_hist + i);
    }

    fscanf(f, "%f", &context->mallocs_per_sec);

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            fscanf(f, "%f", context->life_hist[i] + j);
        }
    }

    return context;
}
