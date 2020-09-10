#include "./time.h"

void time_initialize(time_store* store) {
    store->last_frame = 0;
    store->current_frame = SDL_GetPerformanceCounter();
    store->frame_time = 0;
    store->delta = 0;
}

void time_update(time_store* store) {
    store->last_frame = store->current_frame;
    store->current_frame = SDL_GetPerformanceCounter();
    store->frame_time = store->current_frame - store->last_frame;
    store->delta = (
        (store->frame_time) * 1000 / (double) SDL_GetPerformanceFrequency()
    );
}