#pragma once

#include <SDL2/SDL.h>
#include "../game/2048.h"
#include "../render/animate.h"
#include "../render/buttons.h"

typedef struct {
  int x, y;
  bool lmb, rmb;
} mouse_state;

typedef struct {
  Uint32 last, current;
  double delta;
} time_state;

mouse_state mouse;

time_state game_time;

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

void game_init();