#include "weapon.h"
#include <inttypes.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>

extern size_t g_id;

void print_weapon(void* s) {
    if(!s) return;
    printf("Weapon : %s\n", ((Weapon*)s)->name);
    printf("ID : %" PRIu32 "\n", ((Weapon*)s)->id);
    printf("Capacity : %" PRIu32 "\n", ((Weapon*)s)->capacity);
    printf("Weight : %.2f Kg\n", ((Weapon*)s)->weight);
    printf("Range : %.2f Kg\n", ((Weapon*)s)->range);
    printf("Damage : %" PRIu32 "\n", ((Weapon*)s)->damage);
}

Weapon* weapon_create(const char* name, uint32_t capacity, uint32_t damage,
                                 float accuracy, float weight, float range) {
    Weapon* w = calloc(1, sizeof(*w));
    if(!w) return NULL;

    snprintf(w->name, MAX_NAME_LENGTH, "%s", name ? name : "Unknown");

    w->capacity = capacity;
    w->damage = damage;
    w->weight = weight;
    w->range = range;
    w->accuracy = accuracy;
    w->id = g_id++;
    return w;
}