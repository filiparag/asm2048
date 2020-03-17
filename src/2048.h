#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define BOARD_DIM 4
#define RND_POW_MAX 2
#define INIT_CELL_MAX 5

typedef enum {
  LOST,
  PLAYING,
  WON,
  OUT_OF_MOVES
} game_state;

typedef enum {
  DIRECTION_UP,
  DIRECTION_DOWN,
  DIRECTION_LEFT,
  DIRECTION_RIGHT
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
  board_cell_pair add [2 * BOARD_DIM * BOARD_DIM];
  board_cell insert [2 * BOARD_DIM * BOARD_DIM];
  dim move_len, add_len, insert_len;
} board_change;

typedef struct {
  game_state state;
  game_board board;
  val score;
  board_change delta;
} game_store;