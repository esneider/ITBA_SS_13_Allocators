#include <stdio.h>
#include <stdlib.h>
#include "utils.h"

void error(void *root) {

    tfree(root);
    printf("There was an error. Aborting.\n");
    exit(1);
}
