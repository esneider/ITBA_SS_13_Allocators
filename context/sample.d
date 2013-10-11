#!/usr/sbin/dtrace -Cs

/* Usage: sudo ./sample.d -o output.txt -c application */

#pragma D option quiet
#pragma D option dynvarsize=100m


long long time_factor;


#define POW_2(x)  \
    x |= x >> 1;  \
    x |= x >> 2;  \
    x |= x >> 4;  \
    x |= x >> 8;  \
    x |= x >> 16; \
    x = (x + 1) >> 1


#define MS_SINCE(x) ((timestamp - x) / 1000000ll)


dtrace:::BEGIN {

    self->start = timestamp;
}

pid$target::malloc:entry {

    self->size = arg0;
    self->trace = 1;
}

pid$target::calloc:entry {

    self->size = arg0 * arg1;
    self->trace = 1;
}

pid$target::realloc:entry {

    self->size = arg1;
    self->trace = 1;
}

pid$target::malloc:return,
pid$target::calloc:return,
pid$target::realloc:return /self->trace/ {

    @total_memory = sum(self->size);
    @num_allocs = count();

    POW_2(self->size);

    self->alloc_time[arg1] = timestamp;
    self->alloc_size[arg1] = self->size;

    @allocs_per_size[self->size] = count();
}

pid$target::free:entry /arg0/ {

    @num_frees = count();

    this->size = self->alloc_size[arg0];
    this->time = MS_SINCE(self->alloc_time[arg0]);

    self->alloc_size[arg0] = 0;
    self->alloc_time[arg0] = 0;

    POW_2(this->time);

    @frees_per_size[this->size] = count();
    @allocs_per_size_per_time[this->size, this->time] = count();
}

dtrace:::END {

    printf("{\n");

    printf("    \"total-time\": %u,\n", MS_SINCE(self->start));

    printa("    \"total-memory\": %@u,\n", @total_memory);

    printa("    \"num-allocs-total\": %@u,\n", @num_allocs);

    printf("    \"num-allocs-per-size\": [\n");
    printa("        [%u, %@u],\n", @allocs_per_size);
    printf("    ],\n");

    printa("    \"num-frees-total\": %@u,\n", @num_frees);

    printf("    \"num-frees-per-size\": [\n");
    printa("        [%u, %@u],\n", @frees_per_size);
    printf("    ],\n");

    printf("    \"num-allocs-per-size-per-time\": [\n");
    printa("        [%u, %u, %@u],\n", @allocs_per_size_per_time);
    printf("    ],\n");

    printf("}\n");
}
