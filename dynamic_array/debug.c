#include "debug.h"
#include <inttypes.h>
#include <stdio.h>

static const char* rank_to_str(Rank rank) {
    switch (rank)
    {
        case RANK_SEAMN: return "SEAMN"; break;
        case RANK_PETTY_OFFICER: return "PETTY_OFFICER"; break;
        case RANK_CAPTAIN: return "CAPTAIN"; break;
        case RANK_WARRANT_OFFICER: return "WARRANT_OFFICER"; break;
        case RANK_LIEUTENANT: return "LIEUTENANT"; break;
        case RANK_COMMANDER: return "COMMANDER"; break;
        case RANK_ADMIRAL: return "ADMIRAL"; break;
        default: return "UNKNOWN"; break;
    }
}
static const char* type_to_str(CargoType type) {
    switch (type)
    {
        case CARGO_SOLDIER: return "SOLDIER"; break;
        case CARGO_STAFF: return "STAFF"; break;
        case CARGO_WEAPON: return "WEAPON"; break;
        case CARGO_FOOD: return "FOOD"; break;
        default: return "UNKNOWN"; break;
    }
}

void print_soldier(const Soldier* s) {
    if(!s) return;
    printf("Soldier : %s\n", s->name);
    printf("ID : %" PRIu32 "\n", s->id);
    printf("Points : %" PRIu32 "\n", s->points);
    printf("Weight : %.2f Kg\n", s->weight);
    printf("Rank : %s\n", rank_to_str(s->rank));
}

void print_weapon(const Weapon* w) {
    if(!w) return;
    printf("Weapon : %s\n", w->name);
    printf("ID : %" PRIu32 "\n", w->id);
    printf("Capacity : %" PRIu32 "\n", w->capacity);
    printf("Weight : %.2f Kg\n", w->weight);
    printf("Range : %.2f Kg\n", w->range);
    printf("Damage : %" PRIu32 "\n", w->damage);
}

void print_ship(const Ship* s) {
    if(!s) return;
    printf("Ship : %s\n", s->name);
    printf("ID : %" PRIu32 "\n", s->id);
    printf("Posision : (%.2f, %.2f)\n", s->position.x, s->position.y);
    printf("Fuel : %.2f Gallons\n", s->fuel);
    printf("Max weight : %.2f Kg\n", s->max_weight);
    printf("Total weight : %.2f Kg\n", s->total_weight);
    printf("Radius : %.2f m\n", s->radius);
    printf("On board : %" PRIu32 "\n", s->nb_onboard);
}

void print_cargo(const CargoItem* item) {
    if(!item) return;
    printf("Cargo\n");
    printf("Weight : %.2f Kg\n", item->weight );
    printf("Type : %s\n", type_to_str(item->type));
    switch (item->type)
    {
    case CARGO_SOLDIER:
        print_soldier(item->p);
        break;
    case CARGO_WEAPON:
        print_weapon(item->p);
    default:
        break;
    }
}
void print_soldier_cb(void* p) { print_soldier((const Soldier*)p); }
void print_weapon_cb(void* p) { print_weapon((const Weapon*)p); }
void print_ship_cb(void* p) { print_ship((const Ship*)p); }
void print_cargo_cb(void* p) { print_cargo((const CargoItem*)p); }