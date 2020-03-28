#pragma once

#include <stdint.h>
#include <stdbool.h>

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

typedef uint16_t val;
typedef uint8_t dim;
typedef int8_t sdim;

typedef val game_board[BOARD_DIM][BOARD_DIM];

typedef struct {
  dim row, col;
  val value;
} board_cell;

typedef struct {
  board_cell orig;
  board_cell dest;
} board_cell_pair;

// typedef struct {
//   board_cell_pair move[2 * BOARD_DIM * BOARD_DIM];
//   board_cell insert[2 * BOARD_DIM * BOARD_DIM];
//   board_cell add[2 * BOARD_DIM * BOARD_DIM];
//   dim move_len, insert_len, add_len;
// } board_change;

typedef struct {
  board_cell insert[BOARD_DIM * BOARD_DIM];
  board_cell add[BOARD_DIM * BOARD_DIM];
  board_cell_pair move[BOARD_DIM * BOARD_DIM];
  dim insert_len, add_len, move_len;
  bool add_moved[BOARD_DIM * BOARD_DIM];
} board_delta;

typedef struct {
  game_state state;
  game_board board;
  val score;
  board_delta delta;
} game_store;