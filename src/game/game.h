#pragma once

#include <stdlib.h>
#include <stdio.h>

#include "./types.h"

void game_board_clear(game_store* store);
bool game_board_full(game_store* store);
bool game_board_insert_random(game_store* store);
bool game_board_move(game_store* store, const game_move move);
void game_initialize(game_store* store, const dim rows, const dim cols);
bool game_make_move(game_store* store, const game_move move);
void game_print(game_store* store);