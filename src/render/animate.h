#pragma once

#include <math.h>
#include "types.h"
#include "draw.h"

#define ANIMATION_TIME 250

#define BUFFER_SIZE 2 * BOARD_DIM * BOARD_DIM

#define ANIM_ADD_DELAY ANIMATION_TIME / 2
#define ANIM_ADD_DURATION ANIMATION_TIME / 2
#define ANIM_ADD_STRENGTH 1.0
#define ANIM_ADD_SCALE 1.0

#define ANIM_INSERT_DELAY ANIMATION_TIME / 2
#define ANIM_INSERT_DURATION ANIMATION_TIME / 2
#define ANIM_INSERT_STRENGTH 5.0
#define ANIM_INSERT_SCALE 0.0

#define ANIM_MOVE_DELAY 0
#define ANIM_MOVE_DURATION ANIMATION_TIME / 2
#define ANIM_MOVE_STRENGTH 5.0
#define ANIM_MOVE_SCALE 0.0

typedef enum {
  INTERACTIVE, TRANSITION
} render_state;

typedef enum {
  TRANSITION_SIGMOID,
  TRANSITION_SQUARE
} transition_type;

typedef struct {
  val value;
  pos x, y;
  board_cell orig, dest;
  double elapsed_time;
} translation_cell;

typedef struct {
  val value;
  dim row, col;
  double scale;
  double elapsed_time;
} scale_cell;

translation_cell move[BUFFER_SIZE];
scale_cell insert[BUFFER_SIZE];
scale_cell add[BUFFER_SIZE];
dim move_len, add_len, insert_len;

game_board board_render;
bool board_move_keep[BOARD_DIM][BOARD_DIM];

void animate_clear();

void animate_flush_completed();

void debug(
	SDL_Renderer* ren,
  const double v, const pos p
);

double transition(
	const double curr, const double max,
  const double strenght,
  const transition_type type
);

void move_append(
	const board_cell_pair cells[],
 const dim len
);

void insert_append(
	const board_cell cells[],
   const dim len
);

void add_append(
	const board_cell cells[],
  const dim len
);

void move_update(
	const double delta_time
);

void insert_update(
	const double delta_time
);

void add_update(
	const double delta_time
);

void move_render(
	SDL_Renderer* ren
);

void insert_render(
	SDL_Renderer* ren
);

void add_render(
	SDL_Renderer* ren
);

void board_unchanged(
	game_board new, game_board old
);

void board_copy(
	game_board board
);

bool animate_board(
	SDL_Renderer* ren,
  const double delta_time,
  game_board board, game_board board_old,
  board_delta* delta
);