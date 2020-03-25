#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define BOARD_DIM 4
#define RND_POW_MAX 2
#define INIT_CELL_MAX 5

typedef enum {
  STATE_PLAYING = 0,
  STATE_LOST = 1,
  STATE_WON = 2,
  STATE_OUT_OF_MOVES = 3,
  STATE_QUIT = 4
} game_state;

typedef enum {
  DIRECTION_UP = 0,
  DIRECTION_DOWN = 1,
  DIRECTION_LEFT = 2,
  DIRECTION_RIGHT = 3
} direction;

typedef __uint16_t val;
typedef __uint8_t dim;
typedef __int8_t sdim;

typedef val game_board[BOARD_DIM][BOARD_DIM];

typedef struct {
  dim row, col;
  val value;
} board_cell;

typedef struct {
  board_cell orig;
  board_cell dest;
} board_cell_pair;

typedef struct {
  board_cell_pair move [2 * BOARD_DIM * BOARD_DIM];
  board_cell insert [2 * BOARD_DIM * BOARD_DIM];
  board_cell add [2 * BOARD_DIM * BOARD_DIM];
  dim move_len, insert_len, add_len;
} board_change;

typedef struct {
  game_state state;
  game_board board;
  val score;
  board_change delta;
} game_store;

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

void delta_move(
	board_change* change, const val value,
  const dim ro, const dim co,
  const dim rd, const dim cd,
  const bool merge
);

void delta_add(
	board_change* change, const val value,
  const dim row, const dim col
);

void delta_insert(
	board_change* change, const val value,
  const dim row, const dim col
);

void delta_clear(
	board_change* change
);

val rows_add(
	game_board board, const direction dir,
  board_change* change
);

val columns_add(
	game_board board, const direction dir,
  board_change* change
);

bool rows_move(
	game_board board, const direction dir,
  board_change* change
);

bool columns_move(
	game_board board, const direction dir,
  board_change* change
);

val random_value();

dim random_cell(
	const dim count
);

bool board_insert(
	game_board board, board_change* change
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

void game_play_console();