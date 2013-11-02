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


struct measures {

    double mean_frag; /* Mean fragmentation over time */
    double max_frag; /* Max fragmentation */
    double mean_meta; /* Mean metadata ratio over time */
    double mean_malloc_time; /* Mean malloc execution time */
    double mean_free_time; /* Mean free execution time */
};

struct simulation {

    char *name; /* Output filename */
    char *context; /* Context file path */
    void *heap; /* Heap address */
    size_t heap_size; /* Heap size (in bytes) */
    size_t time; /* Simulation time (in ms) */
    size_t num_events; /* Number of events */
    struct event *events; /* List of events sorted chronologically */
    struct measures measures;

    size_t _cap_events;
};


struct simulation *new_simulation(void);

void load_simulation(struct simulation *simulation, struct context *context);

void run_simulation(struct simulation *simulation);

void analize_simulation(struct simulation *simulation);

void dump_simulation(struct simulation *simulation);


#endif /* __SIMULATION_H__ */
