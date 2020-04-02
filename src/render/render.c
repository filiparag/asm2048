#include "animate.h"

void move_render(SDL_Renderer* ren) {
  for (dim i = 0; i < move_len; ++i)
    draw_cell_pos(
      ren,
      move[i].value,
      move[i].x,
      move[i].y,
      1
    );
}

void insert_render(SDL_Renderer* ren) {
  for (dim i = 0; i < insert_len; ++i)
    draw_cell(
      ren,
      insert[i].value,
      insert[i].row,
      insert[i].col,
      insert[i].scale
    );
}

void add_render(SDL_Renderer* ren) {
  for (dim i = 0; i < add_len; ++i)
    draw_cell(
      ren,
      add[i].value,
      add[i].row,
      add[i].col,
      add[i].scale
    );
}