#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <math.h>
#include <stdbool.h>

#include "../game/game.h"
#include "../window/time.h"
#include "../window/controls.h"

#define BOARD_SIZE 512
#define HEADER_SIZE 98
#define BOARD_PADDING 24
#define BOARD_DIM_X 4
#define BOARD_DIM_Y 4
#define BOARD_DIM (((BOARD_DIM_X)<(BOARD_DIM_Y))?(BOARD_DIM_X):(BOARD_DIM_Y))

#define CELL_SIZE (BOARD_SIZE - BOARD_PADDING * (BOARD_DIM + 1)) / BOARD_DIM
#define CELL_PADDING CELL_SIZE / 6
#define CELL_BORDER_RAD 5

#define BTN_COUNT 2
#define BTN_SIZE 50

#define FONT_NAME "assets/fonts/ClearSans-Medium.ttf"
#define FONT_PT_SIZE 90

#define IMAGE_NEW "assets/images/new.bmp"
#define IMAGE_UNDO "assets/images/undo.bmp"
#define IMAGE_SHARE "assets/images/share.bmp"

#define COLOR_COUNT 11

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

typedef enum {
    NEW = 0,
    UNDO = 1,
    SHARE = 2
} button_name;

typedef struct {
    bool visible;
    pix x, y, size;
    SDL_Texture* image;
} button;
typedef struct {
    SDL_Renderer* render;
    TTF_Font* font;
    game_store* game;
    input_pointer* mouse;
    time_store* time;
    button buttons[BTN_COUNT];
} draw_store;

void draw_initialize(draw_store* store, SDL_Renderer* render, game_store* game, input_pointer* mouse, time_store* time);
void draw_close(draw_store* store);
void font_open(draw_store* store);
void font_close(draw_store* store);
void image_load(draw_store* store);
void image_unload(draw_store* store);
void draw(draw_store* store);

void color_add(SDL_Color *base, const SDL_Color delta);
void dim_to_pix(const dim row, const dim col, pix *x, pix *y);
void pix_to_dim(const pix x, const pix y, dim *row, dim *col);
void color_sub(SDL_Color *base, const SDL_Color delta);
void color_foreground(const SDL_Color bg, SDL_Color *fg);