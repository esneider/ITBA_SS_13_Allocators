#include <stdio.h>
#include "debug.h"


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
            "\ttype = %s\n"
            "\tsize = %zu\n"
            "\ttime = %.6lf\n"
            "\taddress = %p\n"
            "\talternate = %zu\n"
        "}\n",
        pos,
        event->type ? "FREE" : "MALLOC",
        event->size,
        event->time,
        event->address,
        event->alternate
    );
}


void print_simulation(struct simulation *simulation, bool events) {

    printf(
        "simulation = {\n"
            "\tcontext = \"%s\"\n"
            "\theap_size = %zu\n"
            "\ttime = %zu\n"
            "\tnum_events = %zu\n"
        "}\n",
        simulation->context,
        simulation->heap_size,
        simulation->time,
        simulation->num_events
    );

    if (events) {
        for (size_t i = 0; i < simulation->num_events; i++) {
            print_event(simulation->events + i, i);
        }
    }
}


void print_stats(struct stats *stats) {

    printf(
        "stats = {\n"
            "\tmean_frag = [%.6lf, %.6lf]\n"
            "\tmax_frag = [%.6lf, %.6lf]\n"
            "\tmean_meta = [%.6lf, %.6lf]\n"
            "\tmean_malloc_time = [%.6lf, %.6lf]\n"
            "\tmean_free_time = [%.6lf, %.6lf]\n"
        "}\n",
        stats->mean_frag.mean,
        stats->mean_frag.std,
        stats->max_frag.mean,
        stats->max_frag.std,
        stats->mean_meta.mean,
        stats->mean_meta.std,
        stats->mean_malloc_time.mean,
        stats->mean_malloc_time.std,
        stats->mean_free_time.mean,
        stats->mean_free_time.std
    );
}
