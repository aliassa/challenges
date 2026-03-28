#ifndef SG_SHIP_H
#define SG_SHIP_H

#include "ship.h"
#include "../entities/cargo_item.h"
#include <inttypes.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

extern size_t g_id;

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

int ship_has_cargo(Ship* s, void* cargo, CargoType type) {
    // Check if a ship has a specefic cargo
    if(!s || !cargo) return 0;
    assert(type >= 0 && type < CARGO_COUNT);
    vec_t* v = s->cargo[type];
    for(size_t i = 0; i < v->length; i++) {
        CargoItem* item = v->data[i];
        if(item->p == cargo) return 1;
    }
    return 0;
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

int ship_cargo_remove(Ship* ship, CargoType type, void* obj) {
    assert(type >= 0 || type < CARGO_COUNT);
    if (!ship || !ship->cargo[type]) return -1;

    vec_t* v = ship->cargo[type];

    for(size_t i = 0; i < v->length; i++) {
        CargoItem* item = v->data[i];
        if(item->p == obj) {
            ship->total_weight -= item->weight;

            if(item->type == CARGO_SOLDIER || item->type == CARGO_STAFF)
                ship->nb_onboard--;

            free(item);

            memmove(&v->data[i], &v->data[i+1], (v->length -i - 1) * sizeof (void*));
            v->length--;
            v->data[v->length] = NULL;

            return 0;
        }
    }
    return -2;
}

int ship_cargo_add(Ship* ship, CargoType type, void* obj, float weight) {
    if (!ship) return -1;

    if(ship->total_weight + weight > ship->max_weight)  return -2;
    if(ship->nb_onboard  == ship->max_onboard)  return -3;
    CargoItem* item = malloc(sizeof *item);
    if(!item) return -4;
    
    ship->total_weight += weight;
    item->type = type;
    item->p = obj;
    item->weight = weight;

    if(type == CARGO_SOLDIER || type == CARGO_STAFF) ship->nb_onboard++;

    vec_push(ship->cargo[type], item);

    return 0;
}

int ship_refuel(Ship* s, float amount) {
    if(!s) return -1;
    if(s->fuel + amount >= s->max_fuel) s->fuel = s->max_fuel;
    else s->fuel += amount;
    return 0;
}

#endif // SG_SHIP_H