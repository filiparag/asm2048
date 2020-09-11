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
#define HEADER_SIZE BOARD_SIZE / 5.22
#define BOARD_PADDING BOARD_SIZE / 21.3
#define BOARD_DIM_X 4
#define BOARD_DIM_Y 4
#define BOARD_DIM (((BOARD_DIM_X)<(BOARD_DIM_Y))?(BOARD_DIM_X):(BOARD_DIM_Y))

#define CELL_SIZE (BOARD_SIZE - BOARD_PADDING * (BOARD_DIM + 1)) / BOARD_DIM
#define CELL_PADDING CELL_SIZE / 6
#define CELL_BORDER_RAD 5

#define BTN_COUNT 3
#define BTN_SIZE HEADER_SIZE / 1.96

#define FONT_NAME "assets/fonts/ClearSans-Medium.ttf"
#define FONT_PT_SIZE BOARD_SIZE / 5.69

#define IMAGE_NEW "assets/images/new.bmp"
#define IMAGE_UNDO "assets/images/undo.bmp"
#define IMAGE_SIZE "assets/images/size.bmp"

#define COLOR_COUNT 11

#define ANIMATION_BUFER BOARD_DIM * BOARD_DIM * 3
#define ANIMATION_TIME 400

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
    SIZE = 1,
    UNDO = 2,
} button_name;

typedef struct {
    bool visible;
    pix x, y, size;
    SDL_Texture* image;
} button;

typedef struct {
    double start_time;
    double end_time;
    game_action action;
} animation_action;

typedef struct {
  pix board_size, header_size,
      board_padding, board_dim_max,
      cell_size, cell_padding,
      cell_border_rad, btn_size,
      font_size;
} dimensions;

typedef struct {
    SDL_Renderer* render;
    TTF_Font* font;
    game_store* game;
    input_pointer* mouse;
    time_store* time;
    bool* controls;
    dimensions dim;
    button buttons[BTN_COUNT];
    animation_action anim[ANIMATION_BUFER];
    uint16_t anim_count;
    game_board board_delta;
} draw_store;

void draw_initialize(draw_store* store, SDL_Renderer* render, game_store* game, input_pointer* mouse, time_store* time, bool* controls);
void draw_close(draw_store* store);
void font_open(draw_store* store);
void font_close(draw_store* store);
void image_load(draw_store* store);
void image_unload(draw_store* store);
void draw(draw_store* store);
void draw_set_dimensions(draw_store* store, const pix width);
void draw_rescale(draw_store* store, const pix width);
void draw_cell_pix(draw_store* store, const val value, const pix x, const pix y, const double scale);
void draw_cell_dim(draw_store* store, const val value, const dim row, const dim col, const double scale);
void draw_cell(draw_store* store, const dim row, const dim col, const double scale);

void anim_initialize(draw_store* store);
void anim_append(draw_store* store);
void anim_draw(draw_store* store);

void color_add(SDL_Color *base, const SDL_Color delta);
void dim_to_pix(const dimensions* dims, const dim row, const dim col, pix *x, pix *y);
void pix_to_dim(const dimensions* dims, const pix x, const pix y, dim *row, dim *col);
void color_sub(SDL_Color *base, const SDL_Color delta);
void color_foreground(const SDL_Color bg, SDL_Color *fg);