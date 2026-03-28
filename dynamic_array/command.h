// command.h
#ifndef COMMAND_H
#define COMMAND_H

#include "game_state.h"

typedef enum {
    CMD_MOVE,
    CMD_FIRE,
    CMD_REFUEL,
    CMD_STATUS,
    CMD_QUIT,
    CMD_HELP,
    CMD_UNKNOWN,
} CommandType;

typedef struct {
    CommandType type;
    size_t ship_id;
    union {
        struct { float distance; float direction; } move;   // CMD_MOVE
        struct { size_t attacker_id; size_t target_id; size_t weapon_id; } fire; // CMD_FIRE
        struct { float amount; } refuel;                    // CMD_REFUEL
    } args;
} Command;

Command parse_command(const char* input);
void    execute_command(GameState* gs, const Command* cmd);

#endif