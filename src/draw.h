#ifndef _DRAW_
#define _DRAW_

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <math.h>
#include "2048.h"
#include "buttons.h"

#define FONT_NAME "assets/fonts/ClearSans-Medium.ttf"
#define FONT_PT_SIZE 90

#define BOARD_SIZE 512
#define HEADER_SIZE 128
#define BOARD_PADDING 24

#define CELL_SIZE (BOARD_SIZE - BOARD_PADDING * (BOARD_DIM + 1)) / BOARD_DIM
#define CELL_PADDING CELL_SIZE / 6
#define CELL_BORDER_RAD 5

typedef Uint32 pos;

typedef struct {
  SDL_Texture* texture;
  int width, height;
} text_raster;

typedef enum {
  ALIGN_NONE,
  ALIGN_LEFT,
  ALIGN_MIDDLE,
  ALIGN_RIGHT
} text_align;

const SDL_Color cell_colors[] = {
  {201, 187, 178, 255},
  {239, 229, 216, 255},
  {236, 224, 201, 255},
  {243, 177, 120, 255},
  {244, 149, 99,  255},
  {247, 124, 93,  255},
  {246, 93,  59,  255},
  {237, 206, 113, 255},
  {117, 167, 241, 255},
  {69,  133, 242, 255},
  {237, 207, 114, 255},
  {237, 197, 63,  255}
};

TTF_Font* font = NULL;

void inline font_open();

void inline font_close();

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

void inline text_destroy(
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

#endif