#pragma once

#include <SDL2/SDL.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../game/game.h"
#include "./time.h"
#include "../draw/draw.h"

typedef struct {
    int x, y;
    bool lmb, rmb;
} input_pointer;

typedef struct {
    bool enabled;
    input_pointer mouse;
} control_store;

typedef struct {
    SDL_Window* window;
    SDL_Renderer* render;
    control_store control;
    time_store time;
    game_store game;
    draw_store draw;
} window_store;

bool window_init(window_store* store);
void window_new_frame(window_store* store);
void window_close(window_store* store);

void control_initialize(window_store* store);
bool control_read_events(window_store* store);
bool control_event_keyboard(const SDL_Event event, window_store* store);