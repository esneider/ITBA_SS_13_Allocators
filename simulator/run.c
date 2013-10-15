#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "run.h"
#include "context.h"
#include "simulator.h"


static void usage(void) {

    printf("usage: run -c <context> -h <heap-size> -t <time>\n");
    exit(2);
}


static struct params parse_args(int argc, char **argv) {

    if (argc != 6) usage();

    struct params params;
    int mask = 0;

    for (int i = 0; i < 6; i += 2) {

        if (!strcmp("-c", argv[i])) {

            mask |= 1;

            params.context = argv[i+1];

        } else if (!strcmp("-h", argv[i])) {

            mask |= 2;

            if (sscanf(argv[i + 1], "%d%c", &params.heap_size, &mask) != 1) {
                usage();
            }

        } else if (!strcmp("-t", argv[i])) {

            mask |= 4;

            if (sscanf(argv[i + 1], "%f%c", &params.time, &mask) != 1) {
                usage();
            }
        }
    }

    if (mask != 7) usage();

    return params;
}


int main(int argc, char **argv) {

    struct params params = parse_args(argc - 1, argv + 1);
    struct context *context = load_context(params.context);
    struct simulation *simulation = load_simulation(&params, context);

    free_simulation(simulation);
    free_context(context);
}
