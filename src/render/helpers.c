#include "animate.h"

void debug(SDL_Renderer* ren,
           const double v, const pos p) {
  static char v_string[24];
  sprintf(v_string, "%4.2f", v);
  draw_text(
    ren, v_string, BOARD_SIZE / 2, BOARD_PADDING + 25 * p,
    0, 25, (SDL_Color){0,0,0}, ALIGN_LEFT
  );
}

double transition(const double curr, const double max,
                  const double strenght,
                  const transition_type type) {
  if (type == TRANSITION_SIGMOID)
    return 1 / (
      1 + exp(-(strenght * (curr / max - 0.5)))
    );
  else if (type == TRANSITION_SQUARE)
    return 1 - (
      0.4 * curr * strenght * (curr - max)
    ) / pow(max, 2);
  else
    return 1;
}

void board_unchanged(game_board new, game_board old) {
  for (dim r = 0; r < BOARD_DIM; ++r)
    for (dim c = 0; c < BOARD_DIM; ++c) {
      if (
        new[r][c] != 0 &&
        new[r][c] == old[r][c]
      )
        board_render[r][c] = new[r][c];
      else
        board_render[r][c] = 0;
      board_move_keep[r][c] = false;
    }
}

void board_snapshot(game_board board) {
  for (dim r = 0; r < BOARD_DIM; ++r)
    for (dim c = 0; c < BOARD_DIM; ++c) {
      board_render[r][c] = board[r][c];
      board_move_keep[r][c] = false;
    }
}