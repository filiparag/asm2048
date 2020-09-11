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
                    if (game_make_move(&store->game, LEFT)) {
                        anim_append(&store->draw);
                        store->draw.buttons[NEW].visible = true;
                        store->draw.buttons[SIZE].visible = false;
                    }
                    else
                        control_undo_discard(store);
                    break;
                case SDLK_RIGHT:
                    control_undo_save(store);
                    if (game_make_move(&store->game, RIGHT)) {
                        anim_append(&store->draw);
                        store->draw.buttons[NEW].visible = true;
                        store->draw.buttons[SIZE].visible = false;
                    } else
                        control_undo_discard(store);
                    break;
                case SDLK_UP:
                    control_undo_save(store);
                    if (game_make_move(&store->game, UP)) {
                        anim_append(&store->draw);
                        store->draw.buttons[NEW].visible = true;
                        store->draw.buttons[SIZE].visible = false;
                    } else
                        control_undo_discard(store);
                    break;
                case SDLK_DOWN:
                    control_undo_save(store);
                    if (game_make_move(&store->game, DOWN)) {
                        anim_append(&store->draw);
                        store->draw.buttons[NEW].visible = true;
                        store->draw.buttons[SIZE].visible = false;
                    } else
                        control_undo_discard(store);
                    break;
                case SDLK_n:
                    game_initialize(&store->game, store->game.rows, store->game.cols);
                    anim_append(&store->draw);
                    store->control.undo_count = 0;
                    store->control.undo_current = 0;
                    store->draw.buttons[UNDO].visible = false;
                    store->draw.buttons[NEW].visible = false;
                    store->draw.buttons[SIZE].visible = true;
                    break;
                case SDLK_u:
                    control_undo_restore(store);
                    break;
                case SDLK_1:
                    store->game.board[0][0] <<= 1;
                    break;
                case SDLK_PERIOD:
                    if (store->game.rows < DIM_MAX) {
                        ++store->game.rows;
                        ++store->game.cols;
                    }
                    game_initialize(&store->game, store->game.rows, store->game.cols);
                    draw_set_dimensions(&store->draw, store->draw.dim.board_size);
                    anim_append(&store->draw);
                    store->draw.buttons[UNDO].visible = false;
                    store->draw.buttons[SIZE].visible = true;
                    break;
                case SDLK_COMMA:
                    if (store->game.rows > DIM_MIN) {
                        --store->game.rows;
                        --store->game.cols;
                    }
                    game_initialize(&store->game, store->game.rows, store->game.cols);
                    draw_set_dimensions(&store->draw, store->draw.dim.board_size);
                    anim_append(&store->draw);
                    store->draw.buttons[UNDO].visible = false;
                    store->draw.buttons[SIZE].visible = true;
                    break;
                case SDLK_a:
                    draw_rescale(&store->draw, store->draw.dim.board_size * 1.1);
                    SDL_SetWindowSize(
                        store->window,
                        store->draw.dim.board_size,
                        store->draw.dim.header_size + store->draw.dim.board_size
                    );
                    break;
                case SDLK_z:
                    draw_rescale(&store->draw, store->draw.dim.board_size * 0.9);
                    SDL_SetWindowSize(
                        store->window,
                        store->draw.dim.board_size,
                        store->draw.dim.header_size + store->draw.dim.board_size
                    );
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
                        if (store->draw.buttons[NEW].visible && x >= store->draw.buttons[NEW].x && x <= store->draw.buttons[NEW].x + store->draw.buttons[NEW].size) {
                            game_initialize(&store->game, store->game.rows, store->game.cols);
                            anim_append(&store->draw);
                            store->draw.buttons[UNDO].visible = false;
                            store->draw.buttons[NEW].visible = true;
                            store->draw.buttons[SIZE].visible = true;
                        }
                        if (store->draw.buttons[SIZE].visible && x >= store->draw.buttons[SIZE].x && x <= store->draw.buttons[SIZE].x + store->draw.buttons[SIZE].size) {
                            if (store->game.rows < DIM_MAX) {
                                ++store->game.rows;
                                ++store->game.cols;
                            } else {
                                store->game.rows = DIM_MIN;
                                store->game.cols = DIM_MIN;
                            }
                            game_initialize(&store->game, store->game.rows, store->game.cols);
                            draw_set_dimensions(&store->draw, store->draw.dim.board_size);
                            anim_append(&store->draw);
                            store->draw.buttons[UNDO].visible = false;
                            store->draw.buttons[SIZE].visible = true;
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
