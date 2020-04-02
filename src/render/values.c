#include <SDL2/SDL_ttf.h>
#include "types.h"

const SDL_Color cell_colors[] = {
  {201, 187, 178, 255},
  {239, 229, 216, 255},
  {236, 224, 201, 255},
  {243, 177, 120, 255},
  {244, 149, 99,  255},
  {247, 124, 93,  255},
  {246, 93,  59,  255},
  {237, 206, 113, 255},
  {117, 167, 241, 255},
  {69,  133, 242, 255},
  {237, 207, 114, 255},
  {237, 197, 63,  255}
};

TTF_Font* font = NULL;

const button buttons[] = {
  {
    "New game",
    ALIGN_MIDDLE,
    BOARD_SIZE - BOARD_PADDING - 100, BOARD_PADDING,
    100, 30,
    {117, 167, 241},
    ACTION_NEW_GAME
  },
  {
    "Undo",
    ALIGN_MIDDLE,
    BOARD_SIZE - BOARD_PADDING - 100, BOARD_PADDING + 45,
    100, 30,
    {243, 177, 120},
    ACTION_UNDO_MOVE
  }
};

Uint8 button_buffer_length = 0;