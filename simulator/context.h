#ifndef __CONTEXT_H__
#define __CONTEXT_H__

#include <stddef.h>


struct context {
    size_t num_buckets;
    double *size_hist;
    double **life_hist;
    size_t allocs_initial;
    double **allocs_markov;
};


struct context* load_context(void *parent, const char *name);


#endif /* __CONTEXT_H__ */
