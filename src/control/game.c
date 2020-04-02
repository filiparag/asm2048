#include "game.h"

void time_update() {
  game_time.last = game_time.current;
  game_time.current = SDL_GetPerformanceCounter();
  game_time.delta = (
    (game_time.current - game_time.last) * 1000 /
    (double) SDL_GetPerformanceFrequency()
  );
}

void board_copy(game_board source, game_board target) {
  for (dim r = 0; r < BOARD_DIM; ++r)
    for (dim c = 0; c < BOARD_DIM; ++c)
      target[r][c] = source[r][c];
}

void board_zeros(game_board board) {
  for (dim r = 0; r < BOARD_DIM; ++r)
    for (dim c = 0; c < BOARD_DIM; ++c)
      board[r][c] = 0;
}

void event_key_down(SDL_Keycode key) {
  switch (key) {
    case SDLK_UP:
      board_copy(game.board, board_last);
      game_action(&game, DIRECTION_UP);
      break;
    case SDLK_DOWN:
      board_copy(game.board, board_last);
      game_action(&game, DIRECTION_DOWN);
      break;
    case SDLK_LEFT:
      board_copy(game.board, board_last);
      game_action(&game, DIRECTION_LEFT);
      break;
    case SDLK_RIGHT:
      board_copy(game.board, board_last);
      game_action(&game, DIRECTION_RIGHT);
      break;
    case SDLK_n:
      game_initialize(&game);
      break;
    case SDLK_q:
      game.state = STATE_QUIT;
      break;
    /* <cheats> */
    case SDLK_d:
      game.board[0][0] *= 2;
      break;
    case SDLK_f:
      board_insert(game.board, &game.delta);
      break;
    /* </cheats> */
    default:
      break;
  }
}

void event_mouse_down(Uint8 btn) {
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

void event_mouse_up(Uint8 btn) {
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

void handle_input(bool input_enabled) {
  SDL_GetMouseState(&mouse.x, &mouse.y);
  SDL_Event event;
  while(SDL_PollEvent(&event)) {
    if (!input_enabled)
      continue;
    switch(event.type) {
      case SDL_KEYDOWN:
        event_key_down(event.key.keysym.sym);
        break;
      case SDL_KEYUP:
        break;
      case SDL_MOUSEBUTTONDOWN:
        event_mouse_down(event.button.button);
        break;
      case SDL_MOUSEBUTTONUP:
        event_mouse_up(event.button.button);
        break;
      default:
        break;
    }
  }
}

void game_tick(bool input_enabled) {
  time_update();
  handle_input(input_enabled);
  if (input_enabled && !mouse.lmb)
    button_handle_clicks();
}

void game_render(SDL_Renderer* ren) {
  SDL_RenderClear(ren);
  draw_header(ren, game.state, game.score);
  draw_board(ren);
  draw_buttons(ren, mouse.x, mouse.y, mouse.lmb);
  if (!animate_board(ren, game_time.delta, game.board, board_last, &game.delta))
    draw_cells(ren, game.board, 1.0);
}

void game_init() {
  srand(time(NULL));
  time_update();
  mouse = (mouse_state) {
    .x = 0, .y = 0,
    .lmb = false, .rmb = false
  };
  game_initialize(&game);
  board_zeros(board_last);
  animate_clear();
}

bool game_end() {
  return game.state == STATE_QUIT;
}