#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <math.h>
#include <stdbool.h>

#include "../game/game.h"
#include "../window/time.h"

#define BOARD_SIZE 512
#define HEADER_SIZE 128
#define BOARD_PADDING 24
#define BOARD_DIM 4

#define CELL_SIZE (BOARD_SIZE - BOARD_PADDING * (BOARD_DIM + 1)) / BOARD_DIM
#define CELL_PADDING CELL_SIZE / 6
#define CELL_BORDER_RAD 5

#define BTN_PADDING 5
#define BTN_BORDER_RAD 3
#define BTN_COLOR_DELTA 30
#define BTN_NAME_LENGTH 32

#define FONT_NAME "assets/fonts/ClearSans-Medium.ttf"
#define FONT_PT_SIZE 90

typedef int16_t pix;
typedef uint8_t clr;

typedef enum {
  ALIGN_NONE,
  ALIGN_LEFT,
  ALIGN_MIDDLE,
  ALIGN_RIGHT
} text_align;

typedef struct {
  SDL_Texture* texture;
  int width, height;
} text_raster;

typedef struct {
    SDL_Renderer* render;
    TTF_Font* font;
    game_store* game;
    time_store* time;
} draw_store;

void font_open(draw_store* store);
void font_close(draw_store* store);

void draw_initialize(draw_store* store, SDL_Renderer* render, game_store* game, time_store* time);
void draw(draw_store* store);

void color_add(SDL_Color *base, const SDL_Color delta);
void dim_to_pix(const dim row, const dim col, pix *x, pix *y);
void pix_to_dim(const pix x, const pix y, dim *row, dim *col);
void color_sub(SDL_Color *base, const SDL_Color delta);
void color_foreground(const SDL_Color bg, SDL_Color *fg);