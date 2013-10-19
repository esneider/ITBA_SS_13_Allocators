#include <stdio.h>
#include <stdlib.h>
#include "simulator.h"
#include "allocator.h"
#include "debug.h"


static void error(void) {

    printf("There was an error while loading simulation. Aborting.\n");
    exit(3);
}


static struct event *new_events(struct simulation *simulation, struct context *context, size_t amount) {

    size_t num = simulation->num_events + amount;
    size_t cap = simulation->_cap_events;

    for (; num > cap; cap = 2*cap + 1) ;

    struct event *mem = realloc(simulation->events, cap * sizeof(struct event));

    if (!mem) {

        free_simulation(simulation);
        free_context(context);
        error();
    }

    simulation->events = mem;
    simulation->num_events = num;
    simulation->_cap_events = cap;

    return simulation->events + num - amount;
}


static int cmp(void *thunk, const void *a, const void *b) {

    const size_t *aa = a;
    const size_t *bb = b;
    const struct simulation *simulation = thunk;

    double sub = simulation->events[*aa].time - simulation->events[*bb].time;

    return (sub > 0) - (sub < 0);
}


static void sort_simulation(struct simulation *simulation, struct context *context) {

    size_t *vec = malloc(sizeof(size_t) * simulation->num_events);
    size_t *alt = malloc(sizeof(size_t) * simulation->num_events);

    if (!vec || !alt) {

        free(vec);
        free(alt);
        free_simulation(simulation);
        free_context(context);
        error();
    }

    for (size_t i = 0; i < simulation->num_events; i++) vec[i] = i;

    qsort_r(vec, simulation->num_events, sizeof(size_t), simulation, cmp);

    for (size_t i = 0; i < simulation->num_events; i++) alt[vec[i]] = i;

    for (size_t i = 0; i < simulation->num_events; i++) {

        /* swap events */
        struct event event = simulation->events[i];
        simulation->events[i] = simulation->events[vec[i]];
        simulation->events[vec[i]] = event;

        /* fix alternates */
        simulation->events[simulation->events[i].alternate].alternate = i;
        simulation->events[simulation->events[vec[i]].alternate].alternate = vec[i];

        /* fix vec & alt */
        vec[alt[i]] = vec[i];
        alt[vec[i]] = alt[i];
    }

    free(vec);
    free(alt);
}


static size_t log_rnd(size_t log) {

    return (1 << log) + (rand() & ((1 << log) - 1));
}


static size_t hist_bucket(double *hist, struct context *context) {

    double r = rand() / (double)RAND_MAX;

    for (size_t pos = 0; pos < context->num_buckets; pos++) {

        r -= hist[pos];

        if (r <= 0) return pos;
    }

    return context->num_buckets - 1;
}


struct simulation *load_simulation(struct params *params, struct context *context) {

    struct simulation *simulation = calloc(1, sizeof(*simulation));

    if (!simulation) {

        free_context(context);
        error();
    }

    size_t markov_state = context->allocs_initial;

    for (size_t ms = 0; ms < params->time; ms++) {

        size_t num_allocs = log_rnd(markov_state);
        struct event *event = new_events(simulation, context, num_allocs * 2);

        for (size_t alloc = 0; alloc < num_allocs; alloc++) {

            size_t block_size = hist_bucket(context->size_hist, context);
            size_t final_size = log_rnd(block_size);
            double start_time = ms + alloc / (double)num_allocs;

            event->mem = NULL;
            event->type = MALLOC;
            event->size = final_size;
            event->time = start_time;
            event->alternate = (event - simulation->events) + 1;
            event++;

            event->mem = NULL;
            event->type = FREE;
            event->size = final_size;
            event->time = start_time;
            event->time += log_rnd(hist_bucket(context->life_hist[block_size], context));
            event->alternate = (event - simulation->events) - 1;
            event++;
        }

        markov_state = hist_bucket(context->allocs_markov[markov_state], context);
    }

    sort_simulation(simulation, context);

    return simulation;
}


void free_simulation(struct simulation *simulation) {

    if (simulation) {

        free(simulation->events);
        free(simulation);
    }
}
