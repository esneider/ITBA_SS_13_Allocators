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

        struct event *aux = realloc(simulation->events, simulation->_cap_events);

        if (!aux) {

            free_simulation(simulation);
            free_context(context);
            error();
        }

        simulation->events = aux;
    }

    return simulation->events + simulation->num_events - 1;
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

    printf("event times:\n");
    for (size_t i = 0; i < simulation->num_events; i++) {
        printf("%f ", simulation->events[i].time);
    }
    printf("\n");

    qsort_r(vec, simulation->num_events, sizeof(size_t), simulation, cmp);

    for (size_t i = 0; i < simulation->num_events; i++) {

        printf("--\n");
        printf("%d\n", vec[i]); fflush(stdout);

        struct event event = simulation->events[i];

        printf("%zd\n", event.alternate - simulation->events);
        event.alternate->alternate = &event;

        printf("%d\n", vec[i]); fflush(stdout);
        simulation->events[i] = simulation->events[vec[i]];
        simulation->events[i].alternate->alternate = simulation->events + i;

        simulation->events[vec[i]] = event;
        simulation->events[vec[i]].alternate->alternate = simulation->events + vec[i];
    }

    free(vec);
}


static size_t log_rnd(size_t log) {

    return (1 << log) + (rand() & ((1 << log) - 1));
}


static size_t hist_bucket(double *hist, struct context *context) {

    double r = rand() / (double)RAND_MAX;

    for (size_t pos = 0; pos < context->num_buckets; pos++) {

        r -= hist[pos];

        if (r <= 0) {

            return pos;
        }
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

        for (size_t alloc = 0; alloc < num_allocs; alloc++) {

            size_t block_size = hist_bucket(context->size_hist, context);

            struct event *alloc_event = new_event(simulation, context);

            alloc_event->type = MALLOC;
            alloc_event->alternate = (struct event*)simulation->num_events;
            alloc_event->size = log_rnd(block_size);
            alloc_event->time = ms + alloc / (double)num_allocs;

            struct event *free_event = new_event(simulation, context);

            free_event->type = FREE;
            free_event->alternate = (struct event*)(simulation->num_events - 2);
            free_event->size = alloc_event->size;
            free_event->time = alloc_event->time;
            free_event->time += log_rnd(hist_bucket(context->life_hist[block_size], context));
        }

        markov_state = hist_bucket(context->allocs_markov[markov_state], context);
    }

    for (size_t e = 0; e < simulation->num_events; e++) {

        simulation->events[e].alternate = simulation->events + (size_t)simulation->events[e].alternate;
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
