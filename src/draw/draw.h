#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <math.h>
#include <stdbool.h>

#include "../game/game.h"
#include "../window/time.h"

#define BOARD_SIZE 512
#define HEADER_SIZE 296

typedef Sint16 pixel;
typedef Uint8 color;

typedef struct {
    SDL_Renderer* render;
    TTF_Font* font;
    game_store* game;
    time_store* time;
} draw_store;

void draw_initialize(draw_store* store, SDL_Renderer* render, game_store* game, time_store* time);
void draw(draw_store* store);
