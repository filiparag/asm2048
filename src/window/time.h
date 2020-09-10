#pragma once

#include <SDL2/SDL.h>
#include <stdint.h>

typedef struct {
    uint32_t last_frame;
    uint32_t current_frame;
    uint32_t delta;
    double frame_time;
} time_store;

void time_initialize(time_store* store);
void time_update(time_store* store);