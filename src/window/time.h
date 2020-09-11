#pragma once

#include <SDL2/SDL.h>
#include <stdint.h>

typedef struct {
    double last_time;
    double current_time;
    double delta_time;
    uint16_t fps;
} time_store;

void time_initialize(time_store* store);
void time_update(time_store* store);