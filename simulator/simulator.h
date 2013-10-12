#ifndef __SIMULATOR_H__
#define __SIMULATOR_H__

#include <stddef.h>
#include "context.h"


struct params {
    char *context;
    size_t heap_size;
    size_t time;
};


struct simulation {
    // TODO
};


struct simulation *simulate(struct params *params, struct context *context);


#endif /* __SIMULATOR_H__ */
