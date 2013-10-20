#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "talloc.h"
#include "context.h"
#include "simulation.h"
#include "debug.h"


static void usage(void) {

    printf("usage: run -c <context-name> -h <heap-size-in-bytes> -t <time-in-ms>\n");
    exit(2);
}


static void parse_args(struct simulation *simulation, int argc, char **argv) {

    if (argc != 6) usage();

    int mask = 0;
    char c;

    for (int i = 0; i < 6; i += 2) {

        if (!strcmp("-c", argv[i])) {

            mask |= 1;

            simulation->context = argv[i+1];

        } else if (!strcmp("-h", argv[i])) {

            mask |= 2;

            if (sscanf(argv[i + 1], "%zu%c", &simulation->heap_size, &c) != 1) {
                usage();
            }

        } else if (!strcmp("-t", argv[i])) {

            mask |= 4;

            if (sscanf(argv[i + 1], "%zu%c", &simulation->time, &c) != 1) {
                usage();
            }
        }
    }

    if (mask != 7) usage();
}


int main(int argc, char **argv) {

    srand(time(NULL));

    struct simulation *simulation = new_simulation();

    parse_args(simulation, argc - 1, argv + 1);

    struct context *context = load_context(simulation, simulation->context);

    load_simulation(simulation, context);

    print_context(context);
    print_simulation(simulation, false);

    tfree(simulation);
}
