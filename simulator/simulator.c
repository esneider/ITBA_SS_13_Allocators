#include <stdlib.h>
#include "simulator.h"
#include "allocator.h"


struct event {
    size_t time;
    size_t size;
    void *mem;
    enum {MALLOC, FREE} type;
    struct event *alternate;
};


struct simulation *simulate(struct params *params, struct context *context) {

    return NULL;
}
