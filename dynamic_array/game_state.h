#ifndef SG_GAME_STATE_H
#define SG_GAME_STATE_H
#include "shared/dynamic_array.h"
#include <stdint.h>

typedef struct {
    vec_t* ships;
    vec_t* soldiers;
    uint32_t turn;
} GameState;

#endif