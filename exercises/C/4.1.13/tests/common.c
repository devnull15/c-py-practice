#include "common.h"
#include <stdlib.h>

int rand_range(int min, int max)
{
    int range_size = max - min + 1;
    return rand() % range_size + min;
}

int rand_port(void)
{
    return rand_range(PORT_MIN, PORT_MAX);
}

