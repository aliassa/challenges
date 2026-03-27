#ifndef SG_SOLDIER_H
#define SG_SOLDIER_H

#include "../shared/types.h"
#include <stdint.h>

typedef struct
{
    char name[MAX_NAME_LENGTH];
    Rank rank;
    uint32_t id;
    float weight;
    uint32_t points;
    Position position;
} Soldier;

void print_soldier(void* s);

#endif // SG_SOLDIER_H