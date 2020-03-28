#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "types.h"

void board_print(
	game_board board
);

dim index_first(
	const direction dir, const dim border
);

dim index_last(
	const direction dir, const dim border
);

sdim index_next(
	const direction dir
);

void index_cell(
	const direction dir,
  const dim a, const dim b,
  dim* r, dim* c, dim* r_next, dim* c_next
);

bool board_move_continue(
	const val cell,
  const direction dir,
  const dim r_next, const dim c_next
);

bool board_move_next(
	const direction dir,
  dim* r_next, dim* c_next
);

void delta_move(
	board_delta* delta, game_board board,
  const dim ro, const dim co,
  const dim rd, const dim cd,
  const bool merge
);

bool delta_add_move(
	board_delta* delta,
  const dim ro, const dim co,
  const dim rd, const dim cd
);

void delta_add(
	board_delta* delta,
  const val value,
  const dim row, const dim col
);

void delta_insert(
	board_delta* delta, const val value,
  const dim row, const dim col
);

void delta_clear(
	board_delta* delta
);

val board_add(
	game_board board, const direction dir,
	board_delta* delta
);

bool board_move(
	game_board board, const direction dir,
	board_delta* delta, const bool merge
);

val random_value();

dim random_cell(
	const dim count
);

dim board_empty(
	game_board board, board_cell empty[]
);

bool board_insert(
	game_board board, board_delta* delta
);

void board_clear(
	game_board board
);

val board_max(
	game_board board
);

bool board_out_of_moves(
	game_board board
);

void game_initialize(
	game_store *game
);

void game_action(
	game_store *game, const direction dir
);