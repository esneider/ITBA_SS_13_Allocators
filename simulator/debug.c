#include <stdio.h>
#include "debug.h"


void print_params(struct params *params) {

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
            "\tsize_hist = [%.3lf, %.3lf, %.3lf, ...]\n"
            "\tlife_hist = [\n"
                "\t\t[%.3lf, %.3lf, %.3lf, ...],\n"
                "\t\t[%.3lf, %.3lf, %.3lf, ...],\n"
                "\t\t[%.3lf, %.3lf, %.3lf, ...],\n"
                "\t\t...\n"
            "\t]\n"
            "\tallocs_initial = %zu\n"
            "\tallocs_markov = [\n"
                "\t\t[%.3lf, %.3lf, %.3lf, ...],\n"
                "\t\t[%.3lf, %.3lf, %.3lf, ...],\n"
                "\t\t[%.3lf, %.3lf, %.3lf, ...],\n"
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
        context->allocs_markov[2][2]
    );
}


void print_event(struct event *event, size_t pos) {

    printf(
        "event %zd = {\n"
            "\tmem = %p\n"
            "\ttype = %s\n"
            "\tsize = %zu\n"
            "\ttime = %.6lf\n"
            "\talternate = %zu\n"
        "}\n",
        pos,
        event->mem,
        event->type ? "FREE" : "MALLOC",
        event->size,
        event->time,
        event->alternate
    );
}


void print_simulation(struct simulation *simulation) {

    printf(
        "simulation = { num_events = %zu }\n",
        simulation->num_events
    );

    for (size_t i = 0; i < simulation->num_events; i++) {

        print_event(simulation->events + i, i);
    }
}
