#ifndef HEADER_LOGIC
  #define HEADER_LOGIC
  #include "2048.h"
#endif

#ifndef HEADER_DRAW
  #define HEADER_DRAW
  #include "draw.h"
#endif

#ifndef HEADER_BUTTONS
  #define HEADER_BUTTONS
  #include "buttons.h"
#endif

#define BUTTON_ACTION_COUNT 1

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

void button_click(const button_action action) {
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