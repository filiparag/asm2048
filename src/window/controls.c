#include "./window.h"

void control_initialize(window_store* store) {
    store->control.enabled = true;
    store->control.mouse = (input_pointer) {
        .x = 0,
        .y = 0,
        .lmb = false,
        .rmb = false
    };
}

bool control_read_events(window_store* store) {
    if (!store->control.enabled)
        return false;
    SDL_GetMouseState(&store->control.mouse.x, &store->control.mouse.y);
    SDL_Event event;
    if (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_KEYDOWN:
            case SDL_KEYUP:
                control_event_keyboard(event, store);
                break;
            case SDL_MOUSEBUTTONDOWN:
            case SDL_MOUSEBUTTONUP:
                break;
            default:
                break;
        }
        return true;
    } else {
        return false;
    }
}

bool control_event_keyboard(const SDL_Event event, window_store* store) {
    switch (event.type) {
        case SDL_KEYDOWN:
            switch (event.key.keysym.sym) {
                case SDLK_LEFT:
                    game_make_move(&store->game, LEFT);
                    break;
                case SDLK_RIGHT:
                    game_make_move(&store->game, RIGHT);
                    break;
                case SDLK_UP:
                    game_make_move(&store->game, UP);
                    break;
                case SDLK_DOWN:
                    game_make_move(&store->game, DOWN);
                    break;
                case SDLK_n:
                    game_initialize(&store->game, 4, 4);
                    break;
                case SDLK_q:
                    store->game.state = QUIT;
                    break;
            }
            break;
    }
    return true;
}