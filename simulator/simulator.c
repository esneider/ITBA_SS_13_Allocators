#include <stdio.h>
#include <stdlib.h>
#include "simulator.h"
#include "allocator.h"


static void error(void) {

    printf("There was an error while loading simulation. Aborting.\n");
    exit(3);
}


static struct event *new_event(struct simulation *simulation, struct context *context) {

    simulation->num_events++;

    if (simulation->num_events > simulation->_cap_events) {

        simulation->_cap_events = 2 * simulation->_cap_events + 1;

        struct simulation *aux = realloc(simulation->events, simulation->_cap_events);

        if (!aux) {

            free_simulation(simulation);
            free_context(context);
            error();
        }
    }

    return simulation->events + simulation->num_events - 1;
}


static size_t log_rnd(size_t log) {

    return (1 << log) + rand() & ((1 << log) - 1);
}


static size_t hist_bucket(double *hist, struct context *context) {

    double r = rand() / RAND_MAX;

    for (size_t pos = 0; pos < context->num_buckets; pos++) {

        r -= hist[pos];

        if (r <= 0) {

            return pos;
        }
    }

    return context->num_buckets - 1;
}


static int cmp(void *thunk, const void *a, const void *b) {

    const size_t *_a = a;
    const size_t *_b = b;

    struct simulation *simulation = thunk;

    return simulation->events[*_a].time - simulation->events[*_b].time;
}


static void sort_simulation(struct simulation *simulation, struct context *context) {

    size_t *vec = malloc(sizeof(size_t) * simulation->num_events);

    if (!vec) {

        free_simulation(simulation);
        free_context(context);
        error();
    }

    for (size_t i = 0; i < simulation->num_events; i++) {

        vec[i] = i;
    }

    qsort_r(vec, simulation->num_events, sizeof(struct event), simulation, cmp);

    for (size_t i = 0; i < simulation->num_events; i++) {

        struct event event = simulation->events[i];
        event.alternate->alternate = &event;

        simulation->events[i] = simulation->events[vec[i]];
        simulation->events[i].alternate->alternate = simulation->events + i;

        simulation->events[vec[i]] = event;
        simulation->events[vec[i]].alternate->alternate = simulation->events + vec[i];
    }

    free(vec);
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

        for (size_t alloc = 0; alloc < num_allocs; alloc++) {

            struct event *alloc_event = new_event(simulation, context);
            struct event *free_event = new_event(simulation, context);

            size_t block_size = hist_bucket(context->size_hist, context);

            alloc_event->type = MALLOC;
            alloc_event->alternate = free_event;
            alloc_event->size = log_rnd(block_size);
            alloc_event->time = ms + alloc / num_allocs;

            free_event->type = FREE;
            free_event->alternate = alloc_event;
            free_event->size = alloc_event->size;
            free_event->time = alloc_event->time;
            free_event->time += log_rnd(hist_bucket(context->life_hist[block_size], context));
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
