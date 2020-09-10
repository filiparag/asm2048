#include "./game.h"

void game_board_clear(game_board board, const dim rows, const dim cols) {
    for (dim i = 0; i < rows; ++i)
        for (dim j = 0; j < cols; ++j)
            board[i][j] = 0;
}

bool game_board_full(game_board board, const dim rows, const dim cols) {
    dim count = rows*cols;
    for (dim i = 0; i < rows; ++i)
        for (dim j = 0; j < cols; ++j)
            if (board[i][j] > 0)
                --count;
    return !count;
}

bool game_board_insert_random(game_board board, const dim rows, const dim cols) {
    dim count = 0;
    dim empty[DIM_MAX*DIM_MAX][2];
    for (dim i = 0; i < rows; ++i)
        for (dim j = 0; j < cols; ++j)
            if (board[i][j] == 0) {
                empty[count][0] = i;
                empty[count][1] = j;
                ++count;
            }
    if (count == 0)
        return false;
    dim random_cell = rand() % count;
    val random_value = 2 << rand() % INSERT_POW_MAX;
    // val random_value = 8;
    board[empty[random_cell][0]][empty[random_cell][1]] = random_value;
    printf("Insrt: %1u at %2i,%2i\n", random_value, empty[random_cell][0] + 1, empty[random_cell][1] + 1);
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

bool game_board_move(game_board board, const dim rows, const dim cols, const game_move move, val* score) {
    bool moved = false;
    board_move m;
    game_board_move_direction(move, rows, cols, &m);
    for(dim i = m.row_start; i != m.row_end; i += m.row_inc)
        for(dim j = m.col_start; j != m.col_end; j += m.col_inc) {
            dim in = i, jn = j;
            if (board[in][jn] == 0) {
                while (game_board_move_next(move, &m, &in, &jn) && board[in][jn] == 0);
                if (board[in][jn] != 0) {
                    board[i][j] = board[in][jn];
                    board[in][jn] = 0;
                    moved = true;
                    // move first non empty to current location
                }
            }
            while (game_board_move_next(move, &m, &in, &jn) && board[in][jn] == 0);
            /*if (board[i][j] == 0 && board[in][jn] != 0) {
                board[i][j] = board[in][jn];
                board[in][jn] = 0;
                moved = true;
            } else */
            if (board[i][j] != 0 && board[i][j] == board[in][jn]) {
                board[i][j] <<= 1;
                board[in][jn] = 0;
                *score += board[i][j];
                moved = true;
                // add two cells
            }
        }
    return moved;
}

void game_initialize(game_store* store, const dim rows, const dim cols) {
    store->score = 0;
    store->state = PLAYING;
    store->rows = rows;
    store->cols = cols;
    game_board_clear(store->board, rows, cols);
}

bool game_make_move(game_store* store, const game_move move) {
    if (store->state != PLAYING && store->state != WON)
        return false;
    if (game_board_move(store->board, store->rows, store->cols, move, &(store->score))) {
        if (store->state != WON && store->score >= WIN_SCORE)
            store->state = WON;
        if (!game_board_insert_random(store->board, store->rows, store->cols)) {
            if (store->score < WIN_SCORE)
                store->state = LOST;
            return false;
        }
        return true;
    } else
        return false;
}

void game_print(game_store* store) {
    printf("Score: %10u\n", store->score);
    printf("State: %10u\n", store->state);
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
}