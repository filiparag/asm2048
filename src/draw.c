#ifndef HEADER_LOGIC
  #define HEADER_LOGIC
  #include "2048.h"
#endif

#include <math.h>
#include <SDL2/SDL_ttf.h>

#define BOARD_SIZE 512
#define BOARD_PADDING 24
#define CELL_SIZE (BOARD_SIZE - BOARD_PADDING * (BOARD_DIM + 1)) / BOARD_DIM
#define CELL_PADDING CELL_SIZE / 6

const SDL_Color colors[] = {
  {.r = 201, .g = 187, .b = 178},
  {.r = 239, .g = 229, .b = 216},
  {.r = 236, .g = 224, .b = 201},
  {.r = 243, .g = 177, .b = 120},
  {.r = 244, .g = 149, .b = 99},
  {.r = 247, .g = 124, .b = 93},
  {.r = 246, .g = 93,  .b = 59},
  {.r = 237, .g = 206, .b = 113}
};

void draw_cell_text(SDL_Renderer *ren, char text[], Uint16 x, Uint16 y, SDL_Color bg) {
  TTF_Font* Sans = TTF_OpenFont("fonts/ClearSans-Medium.ttf", 90); //this opens a font style and sets a size

  const SDL_Color dark = {119,110,101},
                  light = {249,246,242},
                  *color;
  if (bg.r + bg.g + bg.b > 540)
    color = &dark;
  else
    color = &light;

  SDL_Surface* surfaceMessage =  TTF_RenderText_Blended(Sans, text, *color); // as TTF_RenderText_Solid could only be used on SDL_Surface then you have to create the surface first

  SDL_Texture* Message = SDL_CreateTextureFromSurface(ren, surfaceMessage); //now you can convert it into a texture

  int text_orig_w, text_orig_h;
  TTF_SizeText(Sans, text, &text_orig_w, &text_orig_h);

  const Uint16 text_size_max = CELL_SIZE - (2 * CELL_PADDING);
  Uint16 text_w = text_orig_w,
         text_h = text_orig_h;

  if (text_w > text_size_max) {
    text_w = text_size_max;
    text_h *= text_w / (double)text_orig_w;
  } else if (text_h > text_size_max) {
    text_h = text_size_max;
    text_w *= text_h / (double)text_orig_h;
  }


  SDL_Rect Message_rect;
  Message_rect.x = x + CELL_PADDING + (text_size_max - text_w) / 2.0;
  Message_rect.y = y + CELL_PADDING + (text_size_max - text_h) / 2.0;;
  Message_rect.w = text_w;
  Message_rect.h = text_h;

  //Mind you that (0,0) is on the top left of the window/screen, think a rect as the text's box, that way it would be very simple to understance

  //Now since it's a texture, you have to put RenderCopy in your game loop area, the area where the whole code executes

  SDL_RenderCopy(ren, Message, NULL, &Message_rect);
}

void draw_cell(SDL_Renderer *ren, val value, dim row, dim col) {
  const val ind = log2(value);
  SDL_SetRenderDrawColor(
    ren,
    colors[ind].r,
    colors[ind].g,
    colors[ind].b,
    colors[ind].a
  );
  const Uint16 x = BOARD_PADDING + (BOARD_PADDING + CELL_SIZE) * col;
  const Uint16 y = BOARD_PADDING + (BOARD_PADDING + CELL_SIZE) * row;
  SDL_Rect srcrect = {
		x, y, CELL_SIZE, CELL_SIZE
	};
  SDL_RenderFillRect(ren, &srcrect);
  if (value != 0) {
    static char value_string[8];
    sprintf(value_string, "%i", value);
    draw_cell_text(ren, value_string, x, y, colors[ind]);
  }
}

void draw_board(SDL_Renderer *ren, game_board board) {
  for (dim r = 0; r < BOARD_DIM; ++r)
    for (dim c = 0; c < BOARD_DIM; ++c)
      draw_cell(ren, board[r][c], r, c);
}