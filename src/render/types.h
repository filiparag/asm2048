#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#define VALUE_COLORS_COUNT 12

#define BOARD_SIZE 512
#define HEADER_SIZE 128
#define BOARD_PADDING 24

#define CELL_SIZE (BOARD_SIZE - BOARD_PADDING * (BOARD_DIM + 1)) / BOARD_DIM
#define CELL_PADDING CELL_SIZE / 6
#define CELL_BORDER_RAD 5

#define BTN_PADDING 5
#define BTN_BORDER_RAD 3
#define BTN_COLOR_DELTA 30
#define BTN_NAME_LENGTH 32

#define BUTTON_ACTION_COUNT 2

#define FONT_NAME "assets/fonts/ClearSans-Medium.ttf"
#define FONT_PT_SIZE 90

typedef uint32_t pos;

typedef enum {
  ALIGN_NONE,
  ALIGN_LEFT,
  ALIGN_MIDDLE,
  ALIGN_RIGHT
} text_align;

typedef enum {
  BTN_NORMAL,
  BTN_HOVERED,
  BTN_ACTIVE
} button_state;

typedef enum {
  ACTION_NEW_GAME,
  ACTION_UNDO_MOVE
} button_action;

typedef struct {
  char name[BTN_NAME_LENGTH];
  text_align align;
  pos x, y, w, h;
  SDL_Color color;
  button_action action;
} button;

extern const SDL_Color cell_colors[VALUE_COLORS_COUNT];
extern const button buttons[BUTTON_ACTION_COUNT];

TTF_Font* font;
Uint8 button_buffer_length;