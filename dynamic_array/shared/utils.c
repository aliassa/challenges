#include "types.h"
#include <stdlib.h>
#include <math.h>


float random_float() {
    return (float)rand() / (float)RAND_MAX;
}

float distance(Position p1, Position p2) {
    float dx = p1.x - p2.x;
    float dy = p1.y - p2.y;
    return hypotf(dx, dy);
}