#ifndef __DEBUG_H__
#define __DEBUG_H__

#include <stddef.h>
#include <stdbool.h>
#include "context.h"
#include "simulation.h"
#include "stats.h"


void print_context(struct context *context);

void print_event(struct event *event, size_t pos);

void print_simulation(struct simulation *simulation, bool events);

void print_stats(struct stats *stats);


#endif /* __DEBUG_H__ */
