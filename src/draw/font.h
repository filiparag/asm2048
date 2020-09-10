#pragma once

#include <SDL2/SDL_ttf.h>

#include "./draw.h"

#define FONT_NAME "assets/fonts/ClearSans-Medium.ttf"
#define FONT_PT_SIZE 90

void font_open(draw_store* store);
void font_close(draw_store* store);