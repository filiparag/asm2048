#include "game.h"

void time_update() {
  game_time.last = game_time.current;
  game_time.current = SDL_GetPerformanceCounter();
  game_time.delta = (
    (game_time.current - game_time.last) * 1000 /
    (double) SDL_GetPerformanceFrequency()
  );
}

void event_key_down(game_store* game, SDL_Keycode key) {
  switch (key) {
    case SDLK_UP:
      game_action(game, DIRECTION_UP);
      break;
    case SDLK_DOWN:
      game_action(game, DIRECTION_DOWN);
      break;
    case SDLK_LEFT:
      game_action(game, DIRECTION_LEFT);
      break;
    case SDLK_RIGHT:
      game_action(game, DIRECTION_RIGHT);
      break;
    case SDLK_n:
      game_initialize(game);
      // animate_clear();
      break;
    case SDLK_q:
      game->state = STATE_QUIT;
      break;
    /* <cheats> */
    case SDLK_d:
      game->board[0][0] *= 2;
      break;
    case SDLK_f:
      board_insert(game->board, &game->delta);
      break;
    /* </cheats> */
    default:
      break;
  }
}

void event_mouse_down(game_store* game, Uint8 btn) {
  switch (btn) {
    case SDL_BUTTON_LEFT:
      mouse.lmb = true;
      break;
    case SDL_BUTTON_RIGHT:
      mouse.rmb = true;
      break;
    default:
      break;
  }
}

void event_mouse_up(game_store* game, Uint8 btn) {
  switch (btn) {
    case SDL_BUTTON_LEFT:
      mouse.lmb = false;
      break;
    case SDL_BUTTON_RIGHT:
      mouse.rmb = false;
      break;
    default:
      break;
  }
}

void handle_input(game_store* game, bool input_enabled) {
  SDL_GetMouseState(&mouse.x, &mouse.y);
  if (!input_enabled)
    return;
  SDL_Event event;
  while(SDL_PollEvent(&event)) {
    switch(event.type) {
      case SDL_KEYDOWN:
        event_key_down(game, event.key.keysym.sym);
        break;
      case SDL_KEYUP:
        break;
      case SDL_MOUSEBUTTONDOWN:
        event_mouse_down(game, event.button.button);
        break;
      case SDL_MOUSEBUTTONUP:
        event_mouse_up(game, event.button.button);
        break;
      default:
        break;
    }
  }
}

void game_tick(game_store* game, bool input_enabled) {
  time_update();
  handle_input(game, input_enabled);
  if (input_enabled && !mouse.lmb)
    button_handle_clicks(game);
}