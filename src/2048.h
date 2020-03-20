#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define BOARD_DIM 4
#define RND_POW_MAX 2
#define INIT_CELL_MAX 5

typedef enum {
  PLAYING = 0,
  LOST = 1,
  WON = 2,
  OUT_OF_MOVES = 3
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