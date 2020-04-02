#include "buttons.h"

void button_click(const button_action action) {
  button_buffer[action] = true;
}

void button_handle_clicks() {
  for (Uint8 b = 0; b < BUTTON_ACTION_COUNT; ++b) {
    switch (b) {
      case ACTION_NEW_GAME:
        if (button_buffer[b])
          game_init();
        break;
      case ACTION_UNDO_MOVE:
        if (button_buffer[b])
          board_copy(board_last, game.board);
        break;
    }
    button_buffer[b] = false;
  }
}