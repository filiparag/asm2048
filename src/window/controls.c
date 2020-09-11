#include "./window.h"

void control_initialize(window_store* store) {
    store->control.enabled = true;
    store->control.mouse = (input_pointer) {
        .x = 0,
        .y = 0,
        .lmb = false,
        .rmb = false
    };
    store->control.undo_count = 0;
    store->control.undo_current = 0;
}

bool control_read_events(window_store* store) {
    // if (!store->control.enabled)
    //     return false;
    SDL_GetMouseState(&store->control.mouse.x, &store->control.mouse.y);
    SDL_Event event;
    if (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_KEYDOWN:
                store->draw.anim_count = 0;
            case SDL_KEYUP:
                control_event_keyboard(event, store);
                break;
            case SDL_MOUSEBUTTONDOWN:
            case SDL_MOUSEBUTTONUP:
                control_event_mouse(event, store);
                break;
            case SDL_QUIT:
                store->game.state = QUIT;
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
                    control_undo_save(store);
                    if (game_make_move(&store->game, LEFT))
                        anim_append(&store->draw);
                    else
                        control_undo_discard(store);
                    break;
                case SDLK_RIGHT:
                    control_undo_save(store);
                    if (game_make_move(&store->game, RIGHT))
                        anim_append(&store->draw);
                    else
                        control_undo_discard(store);
                    break;
                case SDLK_UP:
                    control_undo_save(store);
                    if (game_make_move(&store->game, UP))
                        anim_append(&store->draw);
                    else
                        control_undo_discard(store);
                    break;
                case SDLK_DOWN:
                    control_undo_save(store);
                    if (game_make_move(&store->game, DOWN))
                        anim_append(&store->draw);
                    else
                        control_undo_discard(store);
                    break;
                case SDLK_n:
                    game_initialize(&store->game, store->game.rows, store->game.cols);
                    anim_append(&store->draw);
                    store->control.undo_count = 0;
                    store->control.undo_current = 0;
                    store->draw.buttons[UNDO].visible = false;
                    break;
                case SDLK_u:
                    control_undo_restore(store);
                    break;
                 case SDLK_1:
                    store->game.board[0][0] <<= 1;
                    break;
                case SDLK_q:
                    store->game.state = QUIT;
                    break;
            }
            break;
    }
    return true;
}

bool control_event_mouse(const SDL_Event event, window_store* store) {
    switch (event.type) {
        case SDL_MOUSEBUTTONDOWN:
            switch (event.button.button) {
                case SDL_BUTTON_LEFT:
                    store->control.mouse.lmb = true;
                    int x = store->control.mouse.x,
                        y = store->control.mouse.y;
                    if (y >= store->draw.buttons[0].y && y <= store->draw.buttons[0].y + store->draw.buttons[0].size) {
                        if (x >= store->draw.buttons[NEW].x && x <= store->draw.buttons[NEW].x + store->draw.buttons[NEW].size) {
                            game_initialize(&store->game, store->game.rows, store->game.cols);
                            anim_append(&store->draw);
                            store->draw.buttons[UNDO].visible = false;
                        }
                        if (store->draw.buttons[UNDO].visible && x >= store->draw.buttons[UNDO].x && x <= store->draw.buttons[UNDO].x + store->draw.buttons[UNDO].size)
                            control_undo_restore(store);
                    }
                    break;
                case SDL_BUTTON_RIGHT:
                    store->control.mouse.rmb = true;
                    break;
            }
            break;
        case SDL_MOUSEBUTTONUP:
            switch (event.button.button) {
                case SDL_BUTTON_LEFT:
                    store->control.mouse.lmb = false;
                    break;
                case SDL_BUTTON_RIGHT:
                    store->control.mouse.rmb = false;
                    break;
            }
            break;
    }
    return true;
}

void control_undo_save(window_store* store) {
    memcpy(&store->control.undo[store->control.undo_current], &store->game, sizeof(store->game));
    ++store->control.undo_current;
    store->control.undo_current %= UNDO_BUFFER;
    if (store->control.undo_count < UNDO_BUFFER)
        ++store->control.undo_count;
    store->draw.buttons[UNDO].visible = store->control.undo_count > 0;
}

void control_undo_discard(window_store* store) {
    if (store->control.undo_count == 0)
        return;
    --store->control.undo_current;
    store->control.undo_current %= UNDO_BUFFER;
    --store->control.undo_count;
    store->draw.buttons[UNDO].visible = store->control.undo_count > 0;
}

void control_undo_restore(window_store* store) {
    if (store->control.undo_count == 0)
        return;
    --store->control.undo_current;
    store->control.undo_current %= UNDO_BUFFER;
    --store->control.undo_count;
    memcpy(&store->game, &store->control.undo[store->control.undo_current], sizeof(store->game));
    store->draw.buttons[UNDO].visible = store->control.undo_count > 0;
}
