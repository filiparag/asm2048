#ifndef HEADER_DRAW
  #define HEADER_DRAW
  #include "draw.h"
#endif

#include<math.h>

#define ANIMATION_TIME 1000

#define BUFFER_SIZE 2 * BOARD_DIM * BOARD_DIM

#define SCALE_ORIG 0.0
#define SCALE_TIME ANIMATION_TIME
#define SCALE_EASE_STRENGTH 5

#define TRANSLATE_TIME ANIMATION_TIME
#define TRANSLATE_EASE_STRENGTH 5


typedef enum {
  INTERACTIVE, TRANSITION
} render_state;

typedef enum {
  EASE_SIGMOID
} transition_type;

typedef struct {
  val value;
  pos x, y,
      orig_x, orig_y,
      dest_x, dest_y;
  board_cell orig, dest;
  double elapsed_time;
} translation_cell;

typedef struct {
  val value;
  dim row, col;
  double scale;
  double elapsed_time;
} scale_cell;