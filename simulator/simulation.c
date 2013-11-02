#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include "simulation.h"
#include "allocator.h"
#include "talloc.h"
#include "utils.h"


struct stats {

   /* Mesures the heap external fragmentation:
    * rel_ext_frag = 1 - larger free block / total free memory */
    double rel_ext_frag;

   /* Mesures the ratio of metadata memory vs total memory:
    * rel_metadata = total metadata size / total memory */
    double rel_metadata;
};


struct simulation *new_simulation(void) {

    struct simulation *simulation = tcalloc(sizeof(struct simulation), NULL);

    if (!simulation) error(simulation);

    simulation->events = talloc(0, simulation);
    simulation->heap = talloc(0, simulation);

    if (!simulation->events || !simulation->heap) error(simulation);

    return simulation;
}


static struct event *new_events(struct simulation *simulation, size_t amount) {

    size_t num = simulation->num_events + amount;
    size_t cap = simulation->_cap_events;

    for (; num > cap; cap = 2 * cap + 1) ;

    simulation->events = vrealloc(simulation->events, cap);
    simulation->num_events = num;
    simulation->_cap_events = cap;

    if (!simulation->events) error(simulation);

    return simulation->events + num - amount;
}


static int cmp(void *thunk, const void *a, const void *b) {

    const size_t *aa = a;
    const size_t *bb = b;
    const struct simulation *simulation = thunk;

    double sub = simulation->events[*aa].time - simulation->events[*bb].time;

    return (sub > 0) - (sub < 0);
}


static void sort_simulation(struct simulation *simulation) {

    size_t *vec = talloc(sizeof(size_t) * simulation->num_events, simulation);
    size_t *alt = talloc(sizeof(size_t) * simulation->num_events, simulation);

    if (!vec || !alt) error(simulation);

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

    tfree(vec);
    tfree(alt);
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


void load_simulation(struct simulation *simulation, struct context *context) {

    simulation->num_events = 0;

    size_t markov_state = context->allocs_initial;

    for (size_t ms = 0; ms < simulation->time; ms++) {

        size_t num_allocs = log_rnd(markov_state);
        struct event *event = new_events(simulation, num_allocs * 2);

        for (size_t alloc = 0; alloc < num_allocs; alloc++) {

            size_t block_size = hist_bucket(context->size_hist, context);
            size_t final_size = log_rnd(block_size);
            double start_time = ms + alloc / (double)num_allocs;

            event->type = MALLOC;
            event->size = final_size;
            event->time = start_time;
            event->address = NULL;
            event->alternate = (event - simulation->events) + 1;
            event++;

            event->type = FREE;
            event->size = final_size;
            event->time = start_time;
            event->time += log_rnd(hist_bucket(context->life_hist[block_size], context));
            event->address = NULL;
            event->alternate = (event - simulation->events) - 1;
            event++;
        }

        markov_state = hist_bucket(context->allocs_markov[markov_state], context);
    }

    sort_simulation(simulation);

    struct event *mem = vrealloc(simulation->events, simulation->num_events);
    simulation->events = mem ? mem : simulation->events;
}


void run_simulation(struct simulation *simulation) {

    simulation->heap = trealloc(simulation->heap, simulation->heap_size);

    if (!simulation->heap) error(simulation);

    alloc_init(simulation->heap_size, simulation->heap);

    struct timeval stop, start;

    for (
        struct event *event = simulation->events;
        event < simulation->events + simulation->num_events;
        event++
    ) {

        switch (event->type) {

            case MALLOC:
                gettimeofday(&start, NULL);
                event->address = alloc_malloc(event->size);
                gettimeofday(&stop, NULL);
                simulation->events[event->alternate].address = event->address;
                break;

            case FREE:
                gettimeofday(&start, NULL);
                alloc_free(event->address);
                gettimeofday(&stop, NULL);
                break;
        }

        struct stats stats = alloc_stats();

        event->metadata = stats.rel_metadata;
        event->execution = stop.tv_usec - start.tv_usec;
        event->fragmentation = stats.rel_ext_frag;
    }
}


void analize_simulation(struct simulation *simulation) {

    (void)simulation;

    // TODO
}


void dump_simulation(struct simulation *simulation, const char *name) {

    (void)simulation;
    (void)name;

    // TODO
}
