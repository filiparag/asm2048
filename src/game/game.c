#include "./game.h"
#include "./asm.h"

#ifndef USE_ASM
void game_board_clear(game_store* store) {
    for (dim i = 0; i < store->rows; ++i)
        for (dim j = 0; j < store->cols; ++j)
            store->board[i][j] = 0;
}
#else
void game_board_clear(game_store* store) {
    game_board_clear_asm(store->board, store->rows, store->cols);
}
#endif

#ifndef USE_ASM
bool game_board_out_of_moves(game_store* store) {
    dim moves = 0;
    for (dim i = 0; i < store->rows; ++i)
        for (dim j = 0; j < store->cols; ++j) {
            if (store->board[i][j] == 0)
                ++moves;
            if (i  + 1 < store->rows && store->board[i][j] == store->board[i + 1][j])
                ++moves;
            if (j + 1 < store->cols && store->board[i][j] == store->board[i][j + 1])
                ++moves;
        }
    return moves == 0;
}
#else
bool game_board_out_of_moves(game_store* store) {
    return game_board_out_of_moves_asm(store->board, store->rows, store->cols);
}
#endif

void game_action_add(game_store* store, const board_action_type action, const val value, const dim rt, const dim ct, const dim r1, const dim c1, const dim r2, const dim c2) {
    store->actions[store->actions_count] = (game_action) {
        .action = action,
        .value = value,
        .row_target = rt,
        .col_target = ct,
        .row_el1 = r1,
        .col_el1 = c1,
        .row_el2 = r2,
        .col_el2 = c2
    };
    store->actions_count++;
}

bool game_action_undo_last(game_store* store) {
    if (store->actions_count == 0)
        return false;
    store->actions_count--;
    return true;
}

void board_action_insert(game_store* store, const val value, const dim rt, const dim ct) {
    game_action_add(store, INSERT, value, rt, ct, -1, -1, -1, -1);
}

void board_action_move(game_store* store, const val value, const dim rt, const dim ct, const dim r1, const dim c1) {
    game_action_add(store, MOVE, value, rt, ct, r1, c1, -1, -1);
}

void board_action_add(game_store* store, const val value, const dim rt, const dim ct, const dim r1, const dim c1, const dim r2, const dim c2) {
    game_action_add(store, ADD, value, rt, ct, r1, c1, r2, c2);
}

bool game_board_insert_random(game_store* store) {
    dim count = 0;
    dim empty[DIM_MAX*DIM_MAX][2];
    for (dim i = 0; i < store->rows; ++i)
        for (dim j = 0; j < store->cols; ++j)
            if (store->board[i][j] == 0) {
                empty[count][0] = i;
                empty[count][1] = j;
                ++count;
            }
    if (count == 0)
        return false;
    dim random_cell = rand() % count;
    val random_value = (store->base << rand() % INSERT_POW_MAX);
    store->board[empty[random_cell][0]][empty[random_cell][1]] = random_value;
    board_action_insert(store, random_value, empty[random_cell][0], empty[random_cell][1]);
    return true;
}

void game_board_move_direction(const game_move move, const dim rows, const dim cols, board_move* m) {
    switch (move) {
        case LEFT:
            m->row_start = 0;
            m->row_end = rows;
            m->row_inc = 1;
            m->col_start = 0;
            m->col_end = cols;
            m->col_inc = 1;
            break;
        case RIGHT:
            m->row_start = 0;
            m->row_end = rows;
            m->row_inc = 1;
            m->col_start = cols - 1;
            m->col_end = -1;
            m->col_inc = -1;
            break;
        case UP:
            m->row_start = 0;
            m->row_end = rows;
            m->row_inc = 1;
            m->col_start = 0;
            m->col_end = cols;
            m->col_inc = 1;
            break;
        case DOWN:
            m->row_start = rows - 1;
            m->row_end = -1;
            m->row_inc = -1;
            m->col_start = 0;
            m->col_end = cols;
            m->col_inc = 1;
            break;
    }
}

#ifndef USE_ASM
bool game_board_move_next(const game_move move, const board_move* m, dim* i, dim* j) {
    switch (move) {
        case LEFT:
        case RIGHT:
            if (*j + m->col_inc != m->col_end)
                *j += m->col_inc;
            else
                return false;
            break;
        case UP:
        case DOWN:
             if (*i + m->row_inc != m->row_end)
                *i += m->row_inc;
            else
                return false;
            break;
    }
    return true;
}
#else
bool game_board_move_next(const game_move move, const board_move* m, dim* i, dim* j) {
    return game_board_move_next_asm(move, m, i, j);
}
#endif

bool game_board_move(game_store* store, const game_move move, val* cell_max) {
    bool moved = false;
    board_move m = {0};
    game_board_move_direction(move, store->rows, store->cols, &m);
    for(dim i = m.row_start; i != m.row_end; i += m.row_inc)
        for(dim j = m.col_start; j != m.col_end; j += m.col_inc) {
            dim i1 = i, j1 = j; // e1 - first nonzero element
            if (store->board[i1][j1] == 0)
                while (game_board_move_next(move, &m, &i1, &j1) && store->board[i1][j1] == 0);
            dim i2 = i1, j2 = j1; // e2 - first nonzero neighbour
            while (game_board_move_next(move, &m, &i2, &j2) && store->board[i2][j2] == 0);
            if (store->board[i1][j1] != 0 && (i1 != i2 || j1 != j2) && store->board[i1][j1] == store->board[i2][j2]) {
                val new_value = store->board[i1][j1] << 1;
                store->board[i1][j1] = 0;
                store->board[i2][j2] = 0;
                store->board[i][j] = new_value;
                store->score += new_value;
                moved = true;
                if (new_value > *cell_max)
                    *cell_max = new_value;
                board_action_add(store, store->board[i][j]/2, i, j, i1, j1, i2, j2);
            } else if ((i1 != i || j1 != j) && store->board[i1][j1] != 0) {
                store->board[i][j] = store->board[i1][j1];
                store->board[i1][j1] = 0;
                moved = true;
                board_action_move(store, store->board[i][j], i, j, i1, j1);
            }
        }
    return moved;
}

void game_initialize(game_store* store, const dim rows, const dim cols) {
    store->score = 0;
    store->base = INSERT_BASE;
    store->victory = WIN_CELL_MAX;
    store->state = PLAYING;
    store->rows = rows;
    store->cols = cols;
    store->actions_count = 0;
    game_board_clear(store);
    dim start_cells = rand() % START_CELLS_MAX + 1;
    start_cells = (start_cells < START_CELLS_MIN) ? START_CELLS_MIN : start_cells;
    for (dim i = 0; i < start_cells; ++i)
        game_board_insert_random(store);
}

bool game_make_move(game_store* store, const game_move move) {
    bool moved = false;
    if (store->state != PLAYING && store->state != WON)
        return false;
    store->actions_count = 0;
    val cell_max = 0;
    if (game_board_move(store, move, &cell_max)) {
        if (store->state != WON && cell_max >= store->victory)
            store->state = WON;
        game_board_insert_random(store);
        moved = true;
    }
    if (game_board_out_of_moves(store)) {
        if (store->state == WON)
            store->state = WON_END;
        else
            store->state = LOST;
    }
    return moved;
}

void game_print(game_store* store) {
    switch (store->state) {
        case PLAYING:
            printf("State: playing\n");
            break;
        case WON:
            printf("State: won\n");
            break;
        case LOST:
            printf("State: lost\n");
            break;
        case WON_END:
            printf("State: won (ended)\n");
            break;
        case QUIT:
            printf("State: quit\n");
            break;
    }
    printf("Score: %u\n", store->score);
    printf("Board:\n");
    for (dim i = 0; i < store->rows; ++i) {
        printf("|");
        for (dim j = 0; j < store->cols; ++j)
            if (store->board[i][j] != 0)
                printf("%6u |", store->board[i][j]);
            else
                printf("%6s |", "");
        printf("\n");
    }
    for (int i = 0; i < store->actions_count; ++i) {
        game_action* a = &store->actions[i];
        switch (a->action) {
            case INSERT:
                printf("Insert: %4u [%1i,%1i]\n", a->value, a->row_target + 1, a->col_target + 1);
                break;
            case MOVE:
                printf("Move:   %4u [%1i,%1i] > [%1i,%1i]\n", a->value, a->row_el1 + 1, a->col_el1 + 1, a->row_target + 1, a->col_target + 1);
                break;
            case ADD:
                printf("Add:    %4u [%1i,%1i] + [%1i,%1i] > [%1i,%1i]\n", a->value, a->row_el1 + 1, a->col_el1 + 1, a->row_el2 + 1, a->col_el2 + 1, a->row_target + 1, a->col_target + 1);
                break;
        }
    }
}