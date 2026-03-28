#ifndef SG_SHUP_H
#define SG_SHUP_H

#include <stdint.h>
#include "../shared/types.h"
#include "../shared/dynamic_array.h"
#include "game_state.h"

#define SHIP_HEALTH_MAX 1000

typedef struct {
    char name[MAX_NAME_LENGTH];
    uint32_t id;
    uint32_t nb_onboard;
    uint32_t max_onboard;
    uint32_t health;
    float radius;
    float max_weight;
    float total_weight;

    float fuel;
    float max_fuel;
    float consumption_per_km;
    Position position;

    vec_t* cargo[CARGO_COUNT]; // Contains items on the chip
} Ship;

void ship_destroy(vec_t* ships, Ship* s);
float ship_get_max_range(const Ship* s);
float ship_move(Ship* s, float distance, float direction_rads);
int ship_refuel(Ship* s, float amount);
Ship* ship_create(GameState* gs,const char* name, uint32_t max_onboard, float max_weight, float radius,
                             float consumption_per_km, float max_fuel);
int ship_has_cargo(Ship* s, void* cargo, CargoType type);
int ship_cargo_add(Ship* ship, CargoType type, void* obj, float weight);
int ship_cargo_remove(Ship* ship, CargoType type, void* obj);
float ship_approach_other(Ship* s1, Ship* s2, float distance);
float ship_go_to_point(Ship* s, Position p, float distance_before);
#endif // SG_SHUP_H
