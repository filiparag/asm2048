#include "./draw.h"

void anim_initialize(draw_store* store) {
    store->anim_count = 0;
}

void anim_append(draw_store* store) {
    store->anim_count = 0;
    memcpy(&store->board_delta, &store->game->board, sizeof(game_board));
    for (dim i = 0; i < store->game->actions_count; ++i) {
        memcpy(&store->anim[store->anim_count].action, &store->game->actions[i], sizeof(game_action));
        store->anim[store->anim_count].start_time = store->time->current_time;
        store->anim[store->anim_count].end_time = store->time->current_time;
        switch(store->game->actions[i].action) {
            case INSERT:
                store->anim[store->anim_count].start_time += ANIMATION_TIME / 2;
                store->anim[store->anim_count].end_time += ANIMATION_TIME;
                store->board_delta[store->game->actions[i].row_target][store->game->actions[i].col_target] = 0;
                break;
            case MOVE:
                store->anim[store->anim_count].start_time += 0;
                store->anim[store->anim_count].end_time += ANIMATION_TIME / 2;
                store->board_delta[store->game->actions[i].row_el1][store->game->actions[i].col_el1] = 0;
                store->board_delta[store->game->actions[i].row_target][store->game->actions[i].col_target] = 0;
                break;
            case ADD:
                store->anim[store->anim_count].start_time += 0;
                store->anim[store->anim_count].end_time += ANIMATION_TIME;
                store->board_delta[store->game->actions[i].row_el1][store->game->actions[i].col_el1] = 0;
                store->board_delta[store->game->actions[i].row_el2][store->game->actions[i].col_el2] = 0;
                store->board_delta[store->game->actions[i].row_target][store->game->actions[i].col_target] = 0;
                break;
        }
        ++store->anim_count;
    }
    if (store->anim_count > 0)
        *store->controls = false;
}

double trans_sigmoid(const double value, const double strength) {
    double x = strength * (2.0 * value - 1.0);
    return 1.0 / (1.0 + exp(-x));
}

double trans_exponent(const double value, const double strength) {
    return pow(5 * strength, value) / (5 * strength);
}

double trans_square(const double value, const double strength) {
    return 4 * (-pow(value, 2)+value) * strength;
}

double anim_done(draw_store* store, const dim i) {
    if (store->time->current_time < store->anim[i].start_time)
        return 0;
    double duration = store->anim[i].end_time - store->anim[i].start_time;
    double done = abs(store->time->current_time - store->anim[i].start_time) / duration;
    if (done > 1)
        return 1;
    else
        return done;
}

void anim_insert(draw_store* store, const dim i) {
    game_action* action = &store->anim[i].action;
    double done = anim_done(store, i);
    if (done > 0.3)
        draw_cell_dim(store, action->value, action->row_target, action->col_target, done);
}

void anim_move(draw_store* store, const dim i) {
    game_action* action = &store->anim[i].action;
    pix xt, yt, x1, y1;
    dim_to_pix(action->row_target, action->col_target, &xt, &yt);
    dim_to_pix(action->row_el1, action->col_el1, &x1, &y1);
    double done = anim_done(store, i);
    pix x = x1 + (xt - x1) * done,
        y = y1 + (yt - y1) * done;
    draw_cell_pix(store, action->value, x, y, 1);

}

void anim_add(draw_store* store, const dim i) {
    game_action* action = &store->anim[i].action;
    pix xt, yt, x1, y1, x2, y2;
    dim_to_pix(action->row_target, action->col_target, &xt, &yt);
    dim_to_pix(action->row_el1, action->col_el1, &x1, &y1);
    dim_to_pix(action->row_el2, action->col_el2, &x2, &y2);
    double done = anim_done(store, i);
    pix xp1 = x1 + (xt - x1) * done * 2,
        yp1 = y1 + (yt - y1) * done * 2,
        xp2 = x2 + (xt - x2) * done * 2,
        yp2 = y2 + (yt - y2) * done * 2;
    if (x2 == xt && y2 == yt) {
        pix tmp = x2;
        x2 = x1;
        x1 = tmp;
        tmp = y2;
        y2 = y1;
        y1 = tmp;
    }
    if (done > 0.5) {
        draw_cell_pix(store, action->value*2, xt, yt, 1 + trans_square((done-0.5)*2,0.1));
    } else {
        draw_cell_pix(store, action->value, xp1, yp1, 1);
        draw_cell_pix(store, action->value, xp2, yp2, 1);
    }  
}

void anim_draw(draw_store* store) {
    uint16_t done = 0;
    for (dim i = 0; i < store->anim_count; ++i) {
        switch (store->anim[i].action.action) {
            case INSERT:
                anim_insert(store, i);
                break;
            case MOVE:
                anim_move(store, i);
                break;
            case ADD:
                anim_add(store, i);
                break;
        }
        if (store->time->current_time > store->anim[i].end_time)
            ++done;
    }
    if (done == store->anim_count) {
        store->anim_count = 0;
        *store->controls = true;
    }
}