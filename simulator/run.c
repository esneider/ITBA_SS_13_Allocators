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
    printf("usage: run -c <context-name> -h <heap-size-in-bytes> -t <time-in-ms> -o <output-file>\n");
    exit(2);
}


static void parse_args(struct simulation *simulation, int argc, char **argv) {

    size_t mask = 0;
    int args = argc - 1;
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

        } else if (!strcmp("-o", argv[i])) {

            mask |= 8;
            simulation->name = argv[i + 1];

        } else {
            usage(simulation);
        }
    }

    if ((mask & 15) != 15 || args != -1) {
        usage(simulation);
    }
}


int main(int argc, char **argv) {

    /* Initialization */

    struct simulation *simulation = new_simulation();
    parse_args(simulation, argc - 1, argv + 1);
    srand(time(NULL));

    printf("Loading context...\n");
    struct context *context = load_context(simulation, simulation->context);


    /* Simulation */

    printf("Loading simulation...\n");
    load_simulation(simulation, context);

    printf("Running simulation...\n");
    run_simulation(simulation);

    printf("Analizing simulation...\n");
    analize_simulation(simulation);

    /* Dump */

    printf("Saving simulation...\n");
    dump_simulation(simulation);

    print_measures(&simulation->measures);
    printf("Total events: %zu\n", simulation->num_events);

    tfree(simulation);
    
    return EXIT_SUCCESS;
}
