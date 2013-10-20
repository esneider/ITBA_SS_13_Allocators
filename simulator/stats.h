#ifndef __STATS_H__
#define __STATS_H__

#include <stddef.h>


struct measure {

    size_t num_values;
    double *values;
    double mean;
    double std;
    size_t _cap;
};


struct stats {

    struct measure mean_frag; /* Mean fragmentation over time */
    struct measure max_frag; /* Max fragmentation */
    struct measure mean_meta; /* Mean metadata ratio over time */
    struct measure mean_malloc_time; /* Mean malloc execution time */
    struct measure mean_free_time; /* Mean free execution time */
};


struct stats *new_stats(void *parent);

void add_value(void *parent, struct measure *measure, double value);

void compute_stats(struct stats *stats);


#endif /* __STATS_H__ */
