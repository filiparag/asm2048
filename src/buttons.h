#ifndef HEADER_DRAW
  #define HEADER_DRAW
  #include "draw.h"
#endif

#define BTN_PADDING 5
#define BTN_BORDER_RAD 3
#define BTN_COLOR_DELTA 30
#define BTN_NAME_LENGTH 32

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