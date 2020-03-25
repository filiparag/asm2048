#include "buttons.h"

void inline button_click(const button_action action) {
  button_buffer[action] = true;
}

void button_handle_clicks(game_store* game) {
  for (Uint8 b = 0; b < BUTTON_ACTION_COUNT; ++b) {
    switch (b) {
      case ACTION_NEW_GAME:
        if (button_buffer[b])
          game_initialize(game);
        break;
    }
    button_buffer[b] = false;
  }
}