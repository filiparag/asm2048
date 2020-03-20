#ifndef HEADER_DRAW
  #define HEADER_DRAW
  #include "draw.h"
#endif

#include<math.h>

#define ANIMATION_TIME 200

#define BUFFER_SIZE 2 * BOARD_DIM * BOARD_DIM

#define SCALE_ORIG_INSERT 0.0
#define SCALE_ORIG_ADD 1.0
#define SCALE_TIME ANIMATION_TIME
#define SCALE_STRENGTH_INSERT 5
#define SCALE_STRENGTH_ADD 1

#define TRANSLATE_TIME ANIMATION_TIME
#define TRANSLATE_EASE_STRENGTH 5


typedef enum {
  INTERACTIVE, TRANSITION
} render_state;

typedef enum {
  TRANSITION_SIGMOID,
  TRANSITION_SQUARE
} transition_type;

typedef struct {
  val value;
  pos x, y;
  board_cell orig, dest;
  double elapsed_time;
} translation_cell;

typedef struct {
  val value;
  dim row, col;
  double scale;
  double elapsed_time;
} scale_cell;