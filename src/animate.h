#include<math.h>
#include "draw.h"

#define ANIMATION_TIME 200

#define BUFFER_SIZE 2 * BOARD_DIM * BOARD_DIM

#define ANIM_ADD_DELAY ANIMATION_TIME / 2
#define ANIM_ADD_DURATION ANIMATION_TIME / 2
#define ANIM_ADD_STRENGTH 1.0
#define ANIM_ADD_SCALE 1.0

#define ANIM_INSERT_DELAY ANIMATION_TIME / 2
#define ANIM_INSERT_DURATION ANIMATION_TIME / 2
#define ANIM_INSERT_STRENGTH 5.0
#define ANIM_INSERT_SCALE 0.0

#define ANIM_MOVE_DELAY 0
#define ANIM_MOVE_DURATION ANIMATION_TIME / 2
#define ANIM_MOVE_STRENGTH 5.0
#define ANIM_MOVE_SCALE 0.0

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