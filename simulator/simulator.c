#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "allocator.h"


struct params {
    char *context;
    size_t heap_size;
    size_t time;
};


void usage(void) {

    printf("usage: simulator -c <context> -h <heap-size> -t <time>\n");
    exit(2);
}

void parse_args(int argc, char **argv, struct params *params) {

    if (argc != 6) {
        usage();
    }

    int mask = 0;

    for (int i = 0; i < 6; i += 2) {

        if (!strcmp("-c", argv[i])) {

            mask |= 1;
            params->context = argv[i+1];
        }
        else if (!strcmp("-h", argv[i])) {

            mask |= 2;
            if (sscanf(argv[i + 1], "%d", &params->heap_size) != 1) {
                usage();
            }
        }
        else if (!strcmp("-t", argv[i])) {

            mask |= 4;
            if (sscanf(argv[i + 1], "%d", &params->time) != 1) {
                usage();
            }
        }
    }

    if (mask != 7) {
        usage();
    }
}

int main(int argc, char **argv) {

    struct params params;

    parse_args(argc - 1, argv + 1, &params);
}
