#include "2048.h"

void delta_clear(board_delta* delta) {
  for (dim i = 0; i < delta->add_len; ++i)
    delta->add_moved[i] = false;
  delta->move_len = 0;
  delta->add_len = 0;
  delta->insert_len = 0;
}

void delta_insert(board_delta* delta,
                  const val value,
                  const dim row, const dim col) {
  if (delta == NULL)
    return;
  delta->insert[delta->insert_len] = (board_cell) {
    .row = row, .col = col, .value = value
  };
  ++delta->insert_len;
}

void delta_add(board_delta* delta,
               const val value,
               const dim row, const dim col) {
  if (delta == NULL)
    return;
  delta->add[delta->add_len] = (board_cell) {
    .row = row, .col = col, .value = value
  };
  ++delta->add_len;
}

bool delta_add_move(board_delta* delta,
                    const dim ro, const dim co,
                    const dim rd, const dim cd) {
  for (dim i = 0; i < delta->add_len; ++i) {
    printf("Add %4i (%i %i)\n", delta->add[i].value, delta->add[i].row, delta->add[i].col);
    if (
      !delta->add_moved[i] &&
      delta->add[i].row == ro &&
      delta->add[i].col == co
    ) {
      delta->add[i].row = rd;
      delta->add[i].col = cd;
      delta->add_moved[i] = true;
      return true;
    }
  }
  return false;
}

void delta_move(board_delta* delta, game_board board,
                const dim ro, const dim co,
                const dim rd, const dim cd,
                const bool merge) {
  if (delta == NULL)
    return;
  bool merged = false;
  if (merge) {
    printf("Merge (%i %i) (%i %i)\n", ro, co, rd, cd);
    if (delta_add_move(delta, ro, co, rd, cd))
      return;
    for (dim i = 0; i < delta->move_len; ++i) {
      if (
        delta->move[i].dest.row == ro &&
        delta->move[i].dest.col == co &&
        delta->move[i].orig.value == board[ro][co]
      ) {
        delta->move[i].dest.row = rd;
        delta->move[i].dest.col = cd;
        delta->move[i].dest.value = board[rd][cd];
        merged = true;
        break;
      }
    }
  }
  if (!merge || !merged) {
    printf("Move %i (%i %i) (%i %i)\n", board[ro][co], ro, co, rd, cd);
    delta->move[delta->move_len] = (board_cell_pair) {
      .orig = (board_cell) {
        .row = ro, .col = co, .value = board[ro][co]
      },
      .dest = (board_cell) {
        .row = rd, .col = cd, .value = board[rd][cd]
      }
    };
    ++delta->move_len;
  }
}