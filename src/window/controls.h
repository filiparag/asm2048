#pragma once

#include <stdint.h>
#include <stdbool.h>

#include "../game/game.h"

#define UNDO_BUFFER 8

typedef struct {
    int x, y;
    bool lmb, rmb;
} input_pointer;

typedef struct {
    bool enabled;
    input_pointer mouse;
    game_store undo[UNDO_BUFFER];
    uint16_t undo_count, undo_current;
} control_store;
