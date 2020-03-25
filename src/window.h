#pragma once

#include <SDL2/SDL.h>
#include <stdbool.h>
#include "draw.h"

SDL_Window* win;
SDL_Renderer* ren;

bool window_init();
void window_render();
void window_close();