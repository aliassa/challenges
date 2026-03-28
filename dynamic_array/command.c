#include <string.h>
#include "command.h"
#include <stdio.h>
#include "entities/ship.h"
#include "entities/soldier.h"
#include "entities/weapon.h"
#include "combat.h"
#include "debug.h"

#define CMD_LEN 64

static void print_help() {
    printf("Help : not yet implemented\n");
}
Command parse_command(const char* input) {
    Command ret = { .type = CMD_UNKNOWN, .ship_id = UINT32_MAX, .args = { {0} } };
    if (!input) return ret;

    char cmd[CMD_LEN];
    int  offset = 0;

    if (sscanf(input, "%31s%n", cmd, &offset) != 1)
        return ret;

    const char* rest = input + offset;
    float  f1, f2;
    size_t d0, d1, d2;

    if (strcmp(cmd, "move") == 0) {
        if (sscanf(rest, "%zu %f %f", &d0, &f1, &f2) == 3) {
            ret.type               = CMD_MOVE;
            ret.ship_id            = d0;
            ret.args.move.distance  = f1;
            ret.args.move.direction = f2;
        }
    }
    else if (strcmp(cmd, "fire") == 0) {
        if (sscanf(rest, "%zu %zu %zu", &d0, &d1, &d2) == 3) {
            ret.type               = CMD_FIRE;
            ret.ship_id            = d0;
            ret.args.fire.target_id = d1;
            ret.args.fire.attacker_id = d0;
            ret.args.fire.weapon_id = d2;
        }
    }
    else if (strcmp(cmd, "refuel") == 0) {
        if (sscanf(rest, "%zu %f", &d0, &f1) == 2) {
            ret.type                = CMD_REFUEL;
            ret.ship_id            = d0;
            ret.args.refuel.amount  = f1;
        }
    }
    else if (strcmp(cmd, "status") == 0) ret.type = CMD_STATUS;
    else if (strcmp(cmd, "quit")   == 0) ret.type = CMD_QUIT;
    else if (strcmp(cmd, "help")   == 0) ret.type = CMD_HELP;

    return ret;
}

void execute_command(GameState* gs, const Command* cmd) {
    if(cmd->ship_id >= gs->ships->length) return;
    switch (cmd->type) {
        case CMD_MOVE:
            ship_move(gs->ships->data[cmd->ship_id], cmd->args.move.distance, cmd->args.move.direction);
            break;
        case CMD_FIRE:
            Ship* attacker = vec_get(gs->ships, cmd->args.fire.attacker_id);
            Ship* target = vec_get(gs->ships, cmd->args.fire.target_id);
            CargoItem* ci = vec_get(attacker->cargo[CARGO_WEAPON], cmd->args.fire.weapon_id);
            Weapon* weapon = ci->p;
            if(!attacker || !target || !weapon) printf("NULL!!!\n");
            print_attack_result(ship_fire_weapon(gs, attacker, target, weapon));
            break;
        case CMD_REFUEL:
            ship_refuel(gs->ships->data[cmd->ship_id], cmd->args.refuel.amount);
            break;
        case CMD_STATUS:
            print_status(gs);
            break;
        case CMD_HELP:
            print_help();
            ;break;
        case CMD_QUIT:    break;
        case CMD_UNKNOWN:
            printf("Unknown command. Type 'help' for a list of commands.\n");
            break;
    }
}