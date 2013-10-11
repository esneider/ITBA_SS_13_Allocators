#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "allocator.h"


struct params {
    char *context;
    size_t heap_size;
    size_t time;
};


struct context {
    size_t num_buckets;
    double *size_hist;
    double **life_hist;
    double mallocs_per_sec;
};


void usage(void) {

    printf("usage: simulator -c <context> -h <heap-size> -t <time>\n");
    exit(2);
}

void error(void) {

    printf("There was an error. Aborting.\n");
    exit(3);
}

struct params parse_args(int argc, char **argv) {

    if (argc != 6) {
        usage();
    }

    struct params params;
    int mask = 0;

    for (int i = 0; i < 6; i += 2) {

        if (!strcmp("-c", argv[i])) {

            mask |= 1;
            params.context = argv[i+1];
        }
        else if (!strcmp("-h", argv[i])) {

            mask |= 2;
            if (sscanf(argv[i + 1], "%d%c", &params.heap_size, &mask) != 1) {
                usage();
            }
        }
        else if (!strcmp("-t", argv[i])) {

            mask |= 4;
            if (sscanf(argv[i + 1], "%d%c", &params.time, &mask) != 1) {
                usage();
            }
        }
    }

    if (mask != 7) {
        usage();
    }

    return params;
}

void free_context(struct context context) {

    if (context.life_hist) {
        for (int i = 0; i < context.num_buckets; i++) {
            free(context.life_hist[i]);
        }
    }

    free(context.size_hist);
    free(context.life_hist);
}

struct context new_context(size_t size) {

    struct context context;

    context.num_buckets = size;
    context.size_hist = malloc(size * sizeof(double));
    context.life_hist = malloc(size * sizeof(double*));

    if (!context.size_hist || !context.life_hist) {

        free_context(context);
        error();
    }

    for (int i = 0; i < size; i++) {

        context.life_hist[i] = malloc(size * sizeof(double));

        if (!context.life_hist[i]) {

            free_context(context);
            error();
        }
    }

    return context;
}

struct context load_context(char *name) {

    FILE *f = fopen(name, "r");
    if (!f) error();

    size_t size;
    fscanf(f, "%zu", &size);
    struct context context = new_context(size);

    for (int i = 0; i < size; i++) {
        fscanf(f, "%f", context.size_hist + i);
    }

    fscanf(f, "%f", &context.mallocs_per_sec);

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            fscanf(f, "%f", context.life_hist[i] + j);
        }
    }

    return context;
}

int main(int argc, char **argv) {

    struct params params = parse_args(argc - 1, argv + 1);
    struct context context = load_context(params.context);
}
