#!/usr/sbin/dtrace -s

/* Usage: sudo ./sample.d -o output.txt -c application */

#pragma D option quiet


dtrace:::BEGIN {
    self->start = timestamp;
}

pid$target::malloc:entry {
    @size = quantize(arg0);
    @memory = sum(arg0);
    @allocs = count();
}

pid$target::calloc:entry {
    @size = quantize(arg0 * arg1);
    @memory = sum(arg0 * arg1);
    @allocs = count();
}

pid$target::realloc:entry {
    @size = quantize(arg1);
    @memory = sum(arg1);
    @allocs = count();
}

dtrace:::END {

    printf("{\n");

    printf("    \"elapsed-ms\": %llu,\n", (unsigned long long)(timestamp - self->start) / 1000000ll);

    printf("    \"total-mallocs\": ");
    printa("%@u,\n", @allocs);

    printf("    \"total-memory\": ");
    printa("%@u,\n", @memory);

    printf("    \"size-hist\": {\n");
    printa("%@u", @size);

    printf("    }\n}\n");
}

/* pid$target::malloc:entry { */

    /* trace() */
    /* ustack(); */
    /* printf("[[ %s %d ]]\n", execname, (int)arg0); */
    /* printf("[[ %s | %s | %s | %s ]]\n", probefunc, probemod, probename, probeprov); */
    /* @count_table[probefunc, arg0, arg1] = count(); */
/* } */

/* pid$target::malloc:return { */

    /* printf("[[ %s | %s | %s | %s ]]\n", probefunc, probemod, probename, probeprov); */
/* } */

