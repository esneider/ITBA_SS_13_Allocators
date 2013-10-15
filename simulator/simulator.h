#ifndef __SIMULATOR_H__
#define __SIMULATOR_H__

#include <stddef.h>
#include "context.h"
#include "run.h"


struct event {
    double time;
    size_t size;
    void *mem;
    enum {MALLOC, FREE} type;
    struct event *alternate;
};


struct simulation {
    size_t num_events;
    struct event *events;
    size_t _cap_events;
};


struct simulation *load_simulation(struct params *params, struct context *context);

void free_simulation(struct simulation *simulation);


#endif /* __SIMULATOR_H__ */
