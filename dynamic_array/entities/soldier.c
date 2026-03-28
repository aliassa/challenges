#include "soldier.h"
#include "../shared/types.h"

#include <stdint.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

extern size_t g_id;

Soldier* soldier_create(GameState* gs, const char* name, Rank rank, float weight) {
    Soldier* s = calloc(1, sizeof *s);
    if(!s) return NULL;
    snprintf(s->name, MAX_NAME_LENGTH, "%s", name ? name : "Unknown");
    s->rank = rank;

    s->id = g_id++;
    s->weight = weight;
    vec_push(gs->soldiers, s);
    return s;
}