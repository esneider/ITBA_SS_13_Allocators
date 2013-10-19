#ifndef __DEBUG_H__
#define __DEBUG_H__

#include <stddef.h>
#include "run.h"
#include "context.h"
#include "simulator.h"


void print_params(struct params *params);

void print_context(struct context *context);

void print_event(struct event *event, size_t pos);

void print_simulation(struct simulation *simulation);


#endif /* __DEBUG_H__ */
