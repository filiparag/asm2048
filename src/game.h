#ifndef _GAME_
#define _GAME_

#include <SDL2/SDL.h>
#include "2048.h"
#include "buttons.h"

typedef struct {
  int x, y;
  bool lmb, rmb;
} mouse_state;

typedef struct {
  Uint32 last, current;
  double delta;
} time_state;

mouse_state mouse = {
  .x = 0, .y = 0,
  .lmb = false, .rmb = false
};

time_state game_time = {0};

void time_update();
void event_key_down(
  game_store* game, SDL_Keycode key
);

void event_mouse_down(
  game_store* game, Uint8 btn
);

void event_mouse_up(
  game_store* game, Uint8 btn
);

void handle_input(
  game_store* game, bool input_enabled
);

void game_tick(
  game_store* game, bool input_enabled
);

#endif