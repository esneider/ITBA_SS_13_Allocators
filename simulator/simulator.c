#include <stdio.h>
#include <stdlib.h>
#include "simulator.h"
#include "allocator.h"


static void error(void) {

    printf("There was an error while loading simulation. Aborting.\n");
    exit(3);
}


static struct simulation *new_simulation(size_t num_events) {

    struct simulation *simulation = malloc(sizeof(*simulation));

    if (!simulation) error();

    simulation->num_events = num_events;
    simulation->events = calloc(num_events, sizeof(struct event));

    if (!simulation->events) {

        free_simulation(simulation);
        error();
    }

    return simulation;
}


struct simulation *load_simulation(struct params *params, struct context *context) {

    size_t num_mallocs = params->time * context->mallocs_per_sec;
    struct simulation *simulation = new_simulation(num_mallocs * 2);

    for (int i = 0; i < num_mallocs; i++) {

        // TODO
    }

    return simulation;
}


void free_simulation(struct simulation *simulation) {

    if (simulation) {

        free(simulation->events);
        free(simulation);
    }
}
