#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <math.h>
#include "../game/2048.h"
#include "types.h"
#include "values.h"
#include "../control/buttons.h"

typedef struct {
  SDL_Texture* texture;
  int width, height;
} text_raster;

void font_open();

void font_close();

void color_add(
  SDL_Color* base, const SDL_Color delta
);

void dim_to_pos(
  const dim row, const dim col,
  pos* x, pos* y
);

void pos_to_dim(
  const pos x, const pos y,
  dim* row, dim* col
);

void color_sub(
  SDL_Color* base, const SDL_Color delta
);

void color_foreground(
  const SDL_Color bg, SDL_Color* fg
);

text_raster* text_write(
  SDL_Renderer *ren,
  const char text[], const SDL_Color color
);

void text_destroy(
  text_raster* raster
);

void draw_text(
  SDL_Renderer *ren, const char text[],
  const pos x, const pos y,
  const pos max_w, const pos max_h,
  const SDL_Color color, const text_align align
);

void draw_button(
  SDL_Renderer *ren,
  const button btn, const button_state state
);

void draw_cell_text(
  SDL_Renderer *ren, const char text[],
  const pos x, const pos y, const pos size,
  const pos padding, const SDL_Color bg
);

void draw_cell_box(
  SDL_Renderer *ren, const SDL_Color color,
  const pos x, const pos y, const pos size
);

void draw_cell_pos(
  SDL_Renderer *ren, const val value,
  const pos x, const pos y,
  const double scale
);

void draw_cell(
  SDL_Renderer *ren, const val value,
  const dim row, const dim col,
  const double scale
);

void draw_cells(
  SDL_Renderer *ren, game_board board, const double scale
);

void draw_header(
  SDL_Renderer *ren,
  const game_state state, const val score
);

void draw_buttons(
  SDL_Renderer *ren,
  pos mouse_x, pos mouse_y, bool mouse_down
);

void draw_board(
  SDL_Renderer *ren
);