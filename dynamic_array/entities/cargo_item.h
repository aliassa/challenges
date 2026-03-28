#ifndef SG_CARGO_ITEM_H
#define SG_CARGO_ITEM_H

#include "../shared/types.h"

typedef struct {
    void* p; // Soldier*, Weapon*, Food*, ....
    CargoType type;
    float weight;
} CargoItem;

#endif // SG_CARGO_ITEM_H