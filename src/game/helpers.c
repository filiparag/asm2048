#include "2048.h"

val random_value() {
  return 1 << (rand() % RND_POW_MAX + 1);
}

dim random_cell(const dim count) {
  return rand() % count;
}

dim index_first(const direction dir, const dim border) {
  if (dir == DIRECTION_UP || dir == DIRECTION_LEFT)
    return 0;
  else
    return BOARD_DIM - border;
}

dim index_last(const direction dir, const dim border) {
  if (dir == DIRECTION_UP || dir == DIRECTION_LEFT)
    return BOARD_DIM - border;
  else
    return 0;
}

sdim index_next(const direction dir) {
  if (dir == DIRECTION_UP || dir == DIRECTION_LEFT)
    return 1;
  else
    return -1;
}

void index_cell(const direction dir,
                const dim a, const dim b,
                dim* r, dim* c, dim* r_next, dim* c_next) {
  switch (dir) {
    case DIRECTION_UP:
      *r = b;
      *c = a;
      *r_next = b + 1;
      *c_next = a;
      break;
    case DIRECTION_DOWN:
      *r = b;
      *c = a;
      *r_next = b - 1;
      *c_next = a;
      break;
    case DIRECTION_LEFT:
      *r = a;
      *c = b;
      *r_next = a;
      *c_next = b + 1;
      break;
    case DIRECTION_RIGHT:
      *r = a;
      *c = b;
      *r_next = a;
      *c_next = b - 1;
      break;
  }
}

bool board_move_continue(const val cell,
                         const direction dir,
                         const dim r_next, const dim c_next) {
  if (cell != 0)
    return false;
  switch (dir) {
    case DIRECTION_UP:
      return r_next + 0 != BOARD_DIM - 0;
    case DIRECTION_DOWN:
      return r_next + 1 != 0;
    case DIRECTION_LEFT:
      return c_next + 0 != BOARD_DIM - 0;
    case DIRECTION_RIGHT:
      return c_next + 1 != 0;
    default:
      return false;
  }
}

bool board_move_next(const direction dir,
                     dim* r_next, dim* c_next) {
  switch (dir) {
    case DIRECTION_UP:
      *r_next += 1;
      break;
    case DIRECTION_DOWN:
      *r_next -= 1;
      break;
    case DIRECTION_LEFT:
      *c_next += 1;
      break;
    case DIRECTION_RIGHT:
      *c_next -= 1;
      break;
  }
  return *r_next < BOARD_DIM && 
         *c_next < BOARD_DIM;
}