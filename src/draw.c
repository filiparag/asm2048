#ifndef HEADER_LOGIC
  #define HEADER_LOGIC
  #include "2048.h"
#endif

#ifndef HEADER_DRAW
  #define HEADER_DRAW
  #include "draw.h"
#endif

#include <math.h>
#include <SDL2/SDL_ttf.h>

#define BOARD_SIZE 512
#define HEADER_SIZE 128
#define BOARD_PADDING 24
#define CELL_SIZE (BOARD_SIZE - BOARD_PADDING * (BOARD_DIM + 1)) / BOARD_DIM
#define CELL_PADDING CELL_SIZE / 6

#define FONT_NAME "fonts/ClearSans-Medium.ttf"
#define FONT_PT_SIZE 90

const SDL_Color cell_colors[] = {
  {.r = 201, .g = 187, .b = 178},
  {.r = 239, .g = 229, .b = 216},
  {.r = 236, .g = 224, .b = 201},
  {.r = 243, .g = 177, .b = 120},
  {.r = 244, .g = 149, .b = 99},
  {.r = 247, .g = 124, .b = 93},
  {.r = 246, .g = 93,  .b = 59},
  {.r = 237, .g = 206, .b = 113},
  {.r = 117, .g = 167, .b = 241},
  {.r = 69,  .g = 133, .b = 242},
  {.r = 237, .g = 207, .b = 114},
  {.r = 237, .g = 197, .b = 63}
};

TTF_Font* font = NULL;

void inline font_open() {
  if (font == NULL)
    font = TTF_OpenFont(FONT_NAME, FONT_PT_SIZE);
}

void inline font_close() {
  if (font != NULL)
    TTF_CloseFont(font);
}

text_raster* text_write(SDL_Renderer *ren, char text[], SDL_Color color) {
  font_open();
  text_raster* new_raster = (text_raster*) malloc(sizeof(text_raster));
  SDL_Surface* surface =  TTF_RenderText_Blended(font, text, color);
  new_raster->texture = SDL_CreateTextureFromSurface(ren, surface);
  SDL_FreeSurface(surface);
  TTF_SizeText(font, text, &new_raster->width, &new_raster->height);
  return new_raster;
}

void inline text_destroy(text_raster* raster) {
  SDL_DestroyTexture(raster->texture);
  free(raster);
}

void draw_text(SDL_Renderer *ren, char text[], Uint16 x, Uint16 y,
               Uint16 max_w, Uint16 max_h, SDL_Color color, bool center) {
  text_raster* raster = text_write(ren, text, color);
  SDL_Rect rect = (SDL_Rect){
    .x = x, .y = y, .w = raster->width, .h = raster->height
  };
  if (max_w != 0 && rect.w > max_w) {
    rect.h *= max_w / (double)rect.w;
    rect.w = max_w;
  } else if (max_h != 0 && rect.h > max_h) {
    rect.w *= max_h / (double)rect.h;
    rect.h = max_h;
  }
  if (center) {
    rect.x = x + (max_w - rect.w) / 2.0;
    rect.y = y + (max_h - rect.h) / 2.0;
  }
  SDL_RenderCopy(ren, raster->texture, NULL, &rect);
  text_destroy(raster);
}


void draw_cell_text(SDL_Renderer *ren, char text[], Uint16 x, Uint16 y, SDL_Color bg) {
  SDL_Color color;
  if (bg.r + bg.g + bg.b > 540)
    color = (SDL_Color) {.r = 119, .g = 110, .b = 101};
  else
    color = (SDL_Color) {.r = 249, .g = 246, .b = 242};
  draw_text(
    ren, text, x + CELL_PADDING, y + CELL_PADDING,
    CELL_SIZE - 2 * CELL_PADDING, CELL_SIZE - 2 * CELL_PADDING,
    color, true
  );
}

void draw_cell(SDL_Renderer *ren, val value, dim row, dim col) {
  const val ind = log2(value);
  SDL_SetRenderDrawColor(
    ren,
    cell_colors[ind].r,
    cell_colors[ind].g,
    cell_colors[ind].b,
    cell_colors[ind].a
  );
  const Uint16 x = BOARD_PADDING + (BOARD_PADDING + CELL_SIZE) * col;
  const Uint16 y = HEADER_SIZE + BOARD_PADDING + (BOARD_PADDING + CELL_SIZE) * row;
  SDL_Rect cell_rect = {
		x, y, CELL_SIZE, CELL_SIZE
	};
  SDL_RenderFillRect(ren, &cell_rect);
  if (value != 0) {
    static char value_string[8];
    sprintf(value_string, "%i", value);
    draw_cell_text(ren, value_string, x, y, cell_colors[ind]);
  }
}

void draw_header(SDL_Renderer *ren, game_state state, val score) {
  SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
  SDL_Rect header_rect = {
		0, 0, BOARD_SIZE, HEADER_SIZE
	};
  SDL_RenderFillRect(ren, &header_rect);
  static char score_string[8];
  sprintf(score_string, "%i", score);
  draw_text(ren, score_string, 20, 20, 0, 50, (SDL_Color) {.r = 119, .g = 110, .b = 101}, false);
  draw_text(ren, (state == PLAYING) ? "Playing" : "Lost", 20, 70, 0, 50, (SDL_Color) {.r = 119, .g = 110, .b = 101}, false);
}

void draw_board(SDL_Renderer *ren, game_board board) {
  SDL_SetRenderDrawColor(ren, 187,173,160, 255);
  SDL_Rect board_rect = {
		0, HEADER_SIZE, BOARD_SIZE, BOARD_SIZE
	};
  SDL_RenderFillRect(ren, &board_rect);
  for (dim r = 0; r < BOARD_DIM; ++r)
    for (dim c = 0; c < BOARD_DIM; ++c)
      draw_cell(ren, board[r][c], r, c);
}