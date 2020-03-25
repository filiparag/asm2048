#ifndef _BUTTONS_
#define _BUTTONS_

#include <SDL2/SDL.h>
#include "draw.h"

#define BTN_PADDING 5
#define BTN_BORDER_RAD 3
#define BTN_COLOR_DELTA 30
#define BTN_NAME_LENGTH 32

#define BUTTON_ACTION_COUNT 1

typedef enum {
  BTN_NORMAL,
  BTN_HOVERED,
  BTN_ACTIVE
} button_state;

typedef enum {
  ACTION_NEW_GAME
} button_action;

typedef struct {
  char name[BTN_NAME_LENGTH];
  text_align align;
  pos x, y, w, h;
  SDL_Color color;
  button_action action;
} button;

const button buttons[] = {
  {
    "New game",
    ALIGN_MIDDLE,
    BOARD_SIZE - BOARD_PADDING - 100, BOARD_PADDING,
    100, 30,
    {117, 167, 241},
    ACTION_NEW_GAME
  }
};

bool button_buffer[BUTTON_ACTION_COUNT];
Uint8 button_buffer_length = 0;

void inline button_click(
  const button_action action
);

void button_handle_clicks(
  game_store* game
);

#endif