#pragma once

#include <SDL2/SDL.h>
#include "game.h"
#include "../game/2048.h"
#include "../render/types.h"

const button buttons[BUTTON_ACTION_COUNT];
bool button_buffer[BUTTON_ACTION_COUNT];
Uint8 button_buffer_length;

void button_click(
  const button_action action
);

void button_handle_clicks();