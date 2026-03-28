#ifndef SG_SOLDIER_H
#define SG_SOLDIER_H

#include "../shared/types.h"
#include "../shared/dynamic_array.h"
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

Soldier* soldier_create(vec_t* soldiers, const char* name, Rank rank, float weight);

#endif // SG_SOLDIER_H