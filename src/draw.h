#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <math.h>

#define FONT_NAME "fonts/ClearSans-Medium.ttf"
#define FONT_PT_SIZE 90

#define BOARD_SIZE 512
#define HEADER_SIZE 128
#define BOARD_PADDING 24

#define CELL_SIZE (BOARD_SIZE - BOARD_PADDING * (BOARD_DIM + 1)) / BOARD_DIM
#define CELL_PADDING CELL_SIZE / 6
#define CELL_BORDER_RAD 5

typedef Uint32 pos;

typedef struct {
  SDL_Texture* texture;
  int width, height;
} text_raster;

typedef enum {
  ALIGN_NONE,
  ALIGN_LEFT,
  ALIGN_MIDDLE,
  ALIGN_RIGHT
} text_align;