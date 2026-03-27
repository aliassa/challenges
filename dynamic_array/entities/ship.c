#ifndef SG_SHIP_H
#define SG_SHIP_H

#include "ship.h"
#include <inttypes.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <stdlib.h>

extern size_t g_id;

void print_ship(void* s) {
    if(!s) return;
    printf("Ship : %s\n", ((Ship*)s)->name);
    printf("ID : %" PRIu32 "\n", ((Ship*)s)->id);
    printf("Posision : (%.2f, %.2f)\n", ((Ship*)s)->position.x, ((Ship*)s)->position.y);
    printf("Fuel : %.2f Gallons\n", ((Ship*)s)->fuel);
    printf("Max weight : %.2f Kg\n", ((Ship*)s)->max_weight);
    printf("Total weight : %.2f Kg\n", ((Ship*)s)->total_weight);
    printf("Radius : %.2f m\n", ((Ship*)s)->radius);
    printf("On board : %" PRIu32 "\n", ((Ship*)s)->nb_onboard);
}

Ship* ship_create(vec_t* ships,const char* name, uint32_t max_onboard, float max_weight, float radius,
                             float consumption_per_km, float max_fuel) {
    assert(consumption_per_km != 0);
    Ship* s = calloc(1, sizeof *s);
    if(!s) return NULL;
    snprintf(s->name, MAX_NAME_LENGTH, "%s", name ? name : "Unknown");

    s->id = g_id++;
    s->max_onboard = max_onboard;
    s->max_weight = max_weight;
    s->radius = radius;
    s->max_fuel = max_fuel;
    s->fuel = max_fuel;
    s->consumption_per_km = consumption_per_km;
    s->health = SHIP_HEALTH_MAX;
    for(int i = 0; i < CARGO_COUNT; i++)
        s->cargo[i] = vec_create(10);
    vec_push(ships, s);
    return s;
}

void ship_destroy(vec_t* ships, Ship* s) {
    for(size_t i = 0; i < ships->length; i++) {
        if(ships->data[i] == s)
            vec_delete(ships, i);
    }
}

float ship_get_max_range(const Ship* s) {
    return s->fuel / s->consumption_per_km;
}

float ship_move(Ship* s, float distance, float direction_rads) {
    if(!s) return -1.0f;
    if (s->consumption_per_km <= 0.0f) return -1.0f;
    // if not anaugh fuel , stop where it should (distance * s->consumption_per_km)
    if(distance * s->consumption_per_km >= s->fuel) {
        distance = ship_get_max_range(s);
    }
    // Calc new position
    s->position.x += distance * cos(direction_rads);
    s->position.y += distance * sin(direction_rads);
    // calc new fuel
    s->fuel -= distance * s->consumption_per_km;
    if (s->fuel < 0.0f) s->fuel = 0.0f;
    return distance;
}


int ship_refuel(Ship* s, float amount) {
    if(!s) return -1;
    if(s->fuel + amount >= s->max_fuel) s->fuel = s->max_fuel;
    else s->fuel += amount;
    return 0;
}

#endif // SG_SHIP_H