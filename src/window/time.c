#include "./time.h"

void time_initialize(time_store* store) {
    store->last_time = 0;
    store->current_time = SDL_GetPerformanceCounter() / 1000000.0;
    store->delta_time = 0;
    store->fps = 0;
}

void time_update(time_store* store) {
    store->last_time = store->current_time;
    store->current_time = SDL_GetPerformanceCounter() / 1000000.0;
    store->delta_time = store->current_time - store->last_time;
    store->fps = 1000 / store->delta_time;
}