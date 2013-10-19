#ifndef __DEBUG_H__
#define __DEBUG_H__


#include "run.h"
#include "context.h"
#include "simulator.h"


void print_args(struct params *params) {

    printf(
        "params = {\n"
            "\tcontext = \"%s\"\n"
            "\theap_size = %zu\n"
            "\ttime = %zu\n"
        "}\n",
        params->context,
        params->heap_size,
        params->time
    );
}


void print_context(struct context *context) {

    printf(
        "context = {\n"
            "\tnum_buckets = %zu\n"
            "\tsize_hist = [%.3f, %.3f, %.3f, ...]\n"
            "\tlife_hist = [\n"
                "\t\t[%.3f, %.3f, %.3f, ...],\n"
                "\t\t[%.3f, %.3f, %.3f, ...],\n"
                "\t\t[%.3f, %.3f, %.3f, ...],\n"
                "\t\t...\n"
            "\t]\n"
            "\tallocs_initial = %zu\n"
            "\tallocs_markov = [\n"
                "\t\t[%.3f, %.3f, %.3f, ...],\n"
                "\t\t[%.3f, %.3f, %.3f, ...],\n"
                "\t\t[%.3f, %.3f, %.3f, ...],\n"
                "\t\t...\n"
            "\t]\n"
        "}\n",
        context->num_buckets,
        context->size_hist[0],
        context->size_hist[1],
        context->size_hist[2],
        context->life_hist[0][0],
        context->life_hist[0][1],
        context->life_hist[0][2],
        context->life_hist[1][0],
        context->life_hist[1][1],
        context->life_hist[1][2],
        context->life_hist[2][0],
        context->life_hist[2][1],
        context->life_hist[2][2],
        context->allocs_initial,
        context->allocs_markov[0][0],
        context->allocs_markov[0][1],
        context->allocs_markov[0][2],
        context->allocs_markov[1][0],
        context->allocs_markov[1][1],
        context->allocs_markov[1][2],
        context->allocs_markov[2][0],
        context->allocs_markov[2][1],
        context->allocs_markov[2][2],
    );
}


void print_event(struct event *event, const char *name) {

    printf(
        "event %s = {\n",
            "\ttime = %.6f\n",
            "\tsize = %zu\n",
            "\tmem = %p\n",
            "\ttype = %s\n",
            "\talternate = %+zd\n",
        "}\n",
        name,
        event->time,
        event->size,
        event->mem,
        event->type ? "FREE" : "MALLOC",
        event->alternate ? event->alternate - event : 0
    );
}


void print_simulation(struct simulation *simulation) {

    printf(
        "simulation = { num_events = %zu }\n",
        simulation->num_events,
    );

    char *s = "_";

    for (size_t i = 0; i < 5 && i < simulation->num_events; i++) {

        s[0] = '0' + i;
        print_event(simulation->events + i, s);
    }
}


#endif /* __DEBUG_H__ */
