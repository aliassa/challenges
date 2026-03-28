// debug.h
#ifndef DEBUG_H
#define DEBUG_H

#include "entities/ship.h"
#include "entities/soldier.h"
#include "entities/weapon.h"
#include "entities/cargo_item.h"

void print_soldier(const Soldier* s);
void print_weapon(const Weapon* w);
void print_ship(const Ship* s);
void print_cargo(const CargoItem* item);

// vec_print shims
void print_soldier_cb(void* p);
void print_weapon_cb(void* p);
void print_ship_cb(void* p);
void print_cargo_cb(void* p);

#endif