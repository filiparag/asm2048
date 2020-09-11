#include "./draw.h"

void color_add(SDL_Color *base, const SDL_Color delta) {
    if (base->r + delta.r < UINT8_MAX)
        base->r += delta.r;
    else
        base->r = UINT8_MAX;
    if (base->g + delta.g < UINT8_MAX)
        base->g += delta.g;
    else
        base->g = UINT8_MAX;
    if (base->b + delta.b < UINT8_MAX)
        base->b += delta.b;
    else
        base->b = UINT8_MAX;
    if (base->a + delta.a < UINT8_MAX)
        base->a += delta.a;
    else
        base->a = UINT8_MAX;
}

void dim_to_pix(const dimensions* dims, const dim row, const dim col, pix *x, pix *y) {
    *x = dims->board_padding +
         (dims->board_padding + dims->cell_size) * col;
    *y = dims->header_size + dims->board_padding +
         (dims->board_padding + dims->cell_size) * row;
}

void pix_to_dim(const dimensions* dims, const pix x, const pix y, dim *row, dim *col) {
    *row = (y - dims->board_padding) /
               (dims->board_padding + dims->cell_size);
    *col = (x - dims->board_padding) /
           (dims->board_padding + dims->cell_size);
}

void color_sub(SDL_Color *base, const SDL_Color delta) {
    if (base->r - delta.r >= 0)
        base->r -= delta.r;
    else
        base->r = 0;
    if (base->g - delta.g >= 0)
        base->g -= delta.g;
    else
        base->g = 0;
    if (base->b - delta.b >= 0)
        base->b -= delta.b;
    else
        base->b = 0;
    if (base->a - delta.a >= 0)
        base->a -= delta.a;
    else
        base->a = 0;
}

void color_foreground(const SDL_Color bg, SDL_Color *fg) {
    if (bg.r + bg.g + bg.b > 540) {
        fg->r = 119;
        fg->g = 110;
        fg->b = 101;
    } else {
        fg->r = 249;
        fg->g = 246;
        fg->b = 242;
    }
    fg->a = 255;
}