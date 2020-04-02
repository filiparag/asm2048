#pragma once

#include <SDL2/SDL.h>
#include "../game/2048.h"
#include "../render/animate.h"
#include "buttons.h"

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

game_store game;

game_board board_last;

void time_update();

void board_copy(
  game_board source, game_board target
);

void board_zeros(
  game_board board
);

void event_key_down(
 SDL_Keycode key
);

void event_mouse_down(
  Uint8 btn
);

void event_mouse_up(
  Uint8 btn
);

void handle_input(
  bool input_enabled
);

void game_tick(
  bool input_enabled
);

void game_render(
  SDL_Renderer* ren
);

void game_init();

bool game_end();