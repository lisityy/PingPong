#include "drawing.h"
#include "utils.h"
#include "game.h"
#include "game_objects.h"
#include "checker_game.h"
#include "draw_game.h"

extern unsigned short *fb;

void draw_empty_board() {
    for (int ptr = 0; ptr < 320 * 480; ptr++) {
        unsigned int col = hsv2rgb_lcd(260, 0, 70);
        fb[ptr] = col;

    }
    for (int i = 0; i < 480; i++) {
        unsigned int col = hsv2rgb_lcd(0, 0, 255);
        if (i == 0 || i == 476) {
            for (int j = 0; j < 320; j++)
                draw_pixel_big(i, j, col, 4);
        } else {
            draw_pixel_big(i, 0, col, 4);
            draw_pixel_big(i, 316, col, 4);
        }
    }
}

void draw_board() {
    unsigned int col1 = hsv2rgb_lcd(260, 0, 70);
    unsigned int col2 = hsv2rgb_lcd(0, 0, 255);

    for (int ptr = 0; ptr < 320 * 480; ptr++) {
        fb[ptr] = col1;
    }

    for (int i = 4; i < 315; i++) {
        draw_pixel_big(240, i, col2, 3);
    }

    for (int i = 0; i < 160; i += 29) {
        draw_pixel_big(240, i, col1, 15);
    }

    for (int i = 320 - 13; i > 160; i -= 29) {
        draw_pixel_big(240, i, col1, 15);
    }

    draw_pixel_big(240, 160, col1, 15);

    for (int i = 0; i < 480; i++) {

        if (i == 0 || i == 476) {
            for (int j = 0; j < 320; j++)
                draw_pixel_big(i, j, col2, 4);
        } else {
            draw_pixel_big(i, 0, col2, 4);
            draw_pixel_big(i, 316, col2, 4);
        }
    }
    int x0 = 240;
    int y0 = 160;
    draw_empty_circle(x0, y0, 15, col2,3);
}