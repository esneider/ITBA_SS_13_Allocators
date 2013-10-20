#include <stdio.h>
#include "utils.h"
#include "talloc.h"

void error(void *root) {

    tfree(root);
    printf("There was an error. Aborting.\n");
    exit(2);
}
