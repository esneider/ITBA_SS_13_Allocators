#ifndef __SIMULATION_H__
#define __SIMULATION_H__

#include <stddef.h>
#include "context.h"


struct event {
    enum {MALLOC, FREE} type; /* Operation type */
    size_t size; /* Block size */
    double time; /* Elapsed time */
    void *address; /* Block address */
    size_t alternate; /* Corresponding operation event */
    double metadata; /* Metadata ratio after event */
    double execution; /* Execution time of operation */
    double fragmentation; /* Fragmentation after event */
};


struct simulation {
    char *context; /* Context file path */
    size_t heap_size; /* Heap size (in bytes) */
    size_t time; /* Simulation time (in ms) */
    size_t num_events; /* Number of events */
    struct event *events; /* List of events sorted chronologically */
    size_t _cap_events;
};


struct simulation *new_simulation(void);

void load_simulation(struct simulation *simulation, struct context *context);

void run_simulation(struct simulation *simulation);


#endif /* __SIMULATION_H__ */
