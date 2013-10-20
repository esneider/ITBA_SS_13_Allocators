#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "talloc.h"
#include "context.h"
#include "simulation.h"
#include "debug.h"
#include "utils.h"


static void usage(struct simulation *simulation) {

    tfree(simulation);
    printf("usage: run -c <context-name> -h <heap-size-in-bytes> -t <time-in-ms> [-s <samples>]\n");
    exit(2);
}


static size_t parse_args(struct simulation *simulation, int argc, char **argv) {

    size_t samples = 1;
    size_t mask = 0;
    size_t args = argc - 1;
    char c;

    for (int i = 0; args && i < argc; i += 2, args -= 2) {

        if (!strcmp("-c", argv[i])) {

            mask |= 1;

            simulation->context = argv[i+1];

        } else if (!strcmp("-h", argv[i])) {

            mask |= 2;

            if (sscanf(argv[i + 1], "%zu%c", &simulation->heap_size, &c) != 1) {
                usage(simulation);
            }

        } else if (!strcmp("-t", argv[i])) {

            mask |= 4;

            if (sscanf(argv[i + 1], "%zu%c", &simulation->time, &c) != 1) {
                usage(simulation);
            }

        } else if (!strcmp("-s", argv[i])) {

            mask |= 8;

            if (sscanf(argv[i + 1], "%zu%c", &samples, &c) != 1) {
                usage(simulation);
            }

        } else {
            usage(simulation);
        }
    }

    if (mask & 7 != 7 || args != -1) usage(simulation);

    return samples;
}


int main(int argc, char **argv) {

    /* Initialization */

    struct simulation *simulation = new_simulation();
    struct stats *stats = new_stats(simulation);
    size_t samples = arse_args(simulation, argc - 1, argv + 1);

    srand(time(NULL));
    printf("Loading context...\n");
    struct context *context = load_context(simulation, simulation->context);


    /* Simulations */

    for (size_t i = 0; i < samples; i++) {

        printf("Loading simulation (%zu/%zu)...\n", i, samples);
        load_simulation(simulation, context);

        printf("Running simulation (%zu/%zu)...\n", i, samples);
        run_simulation(simulation);

        printf("Analizing simulation (%zu/%zu)...\n", i, samples);
        analize_simulation(simulation);
    }

    /* Finalization */

    compute_stats(stats);
    print_stats(stats);
    tfree(simulation);
}
