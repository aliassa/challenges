#ifndef SG_WEAPON_H
#define SG_WEAPON_H

#include "../shared/types.h"
#include <stdint.h>

typedef struct
{
    char name[MAX_NAME_LENGTH];
    uint32_t id;
    uint32_t capacity;
    uint32_t count;
    uint32_t damage;
    float accuracy; // from [0..1]
    float weight;
    float range;
} Weapon;

Weapon* weapon_create(const char* name, uint32_t capacity, uint32_t damage,
                                 float accuracy, float weight, float range);
void weapon_recharge_munition(Weapon* w);
#endif // SG_WEAPON_H