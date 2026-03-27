#include "soldier.h"
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

void print_soldier(void* s) {
    if(!s) return;
    printf("Soldier : %s\n", ((Soldier*)s)->name);
    printf("ID : %" PRIu32 "\n", ((Soldier*)s)->id);
    printf("Points : %" PRIu32 "\n", ((Soldier*)s)->points);
    printf("Weight : %.2f Kg\n", ((Soldier*)s)->weight);
    printf("Rank : %s\n", rank_to_str(((Soldier*)s)->rank));
}