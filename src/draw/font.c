#include "./font.h"

void font_open(draw_store* store) {
    if (store->font == NULL)
        store->font = TTF_OpenFont(FONT_NAME, FONT_PT_SIZE);
}

void font_close(draw_store* store) {
    if (store->font != NULL)
        TTF_CloseFont(store->font);
}