#include <stdlib.h>
#include <math.h>
#include "stats.h"
#include "talloc.h"
#include "utils.h"


struct stats *new_stats(void *parent) {

    struct stats *stats = tcalloc(sizeof(struct stats), parent);

    if (!stats) error(parent);

    stats->mean_frag.values = talloc(0, stats);
    stats->max_frag.values = talloc(0, stats);
    stats->mean_meta.values = talloc(0, stats);
    stats->mean_malloc_time.values = talloc(0, stats);
    stats->mean_free_time.values = talloc(0, stats);

    if (
        !stats->mean_frag.values || !stats->max_frag.values ||
        !stats->mean_meta.values || !stats->mean_malloc_time.values ||
        !stats->mean_free_time.values
    ) {
        error(parent);
    }

    return stats;
}


void add_value(void *parent, struct measure *measure, double value) {

    if (measure->num_values + 1 > measure->_cap) {

        measure->_cap = 2 * measure->_cap + 1;
        measure->values = vrealloc(measure->values, measure->_cap);

        if (!measure->values) error(parent);
    }

    measure->values[measure->num_values++] = value;
}


static void compute(struct measure *measure) {

    measure->mean = measure->std = 0;

    for (size_t i = 0; i < measure->num_values; i++) {

        measure->mean += measure->values[i];
    }

    measure->mean /= measure->num_values;

    for (size_t i = 0; i < measure->num_values; i++) {

        double dist = measure->values[i] - measure->mean;
        measure->std += dist * dist;
    }

    measure->std = sqrt(measure->std / measure->num_values);
}


void compute_stats(struct stats *stats) {

    compute(&stats->mean_frag);
    compute(&stats->max_frag);
    compute(&stats->mean_meta);
    compute(&stats->mean_malloc_time);
    compute(&stats->mean_free_time);
}
