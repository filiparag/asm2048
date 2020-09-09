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

bool game_board_insert(game_board board, const dim rows, const dim cols) {
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
    board[empty[random_cell][0]][empty[random_cell][1]] = random_value;
    return true;
}

val game_board_move(game_board board, const dim rows, const dim cols, const game_move move) {
    val scored = 0;
    for(dim i = 0; i < rows; ++i)
        for(dim j = 0; j < cols; ++j)
            false;
    return 0;
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
    val scored = game_board_move(store->board, store->rows, store->cols, move);
    if (scored > 0) {
        store->score += scored;
        if (store->score >= WIN_SCORE)
            store->state = WON;
        if (!game_board_insert(store->board, store->rows, store->cols)) {
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
            printf("%6u |", store->board[i][j]);
        printf("\n");
    }
}