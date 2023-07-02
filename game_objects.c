#include <stdbool.h>
#define START_BALL_X 238
#define START_BALL_Y 158

#include "font_types.h"
#include "drawing.h"
#include "utils.h"
#include "game.h"
#include "game_objects.h"

extern unsigned short *fb;
extern font_descriptor_t *fdes;
extern unsigned short colorBall;
extern int diffNum;
extern int levels;
extern int width_letter;

void reset_ball_position(int *x_ball, int *y_ball) {
    *x_ball = START_BALL_X;
    *y_ball = START_BALL_Y;
}

void update_ball_position(int *x_ball, int *y_ball, int dx, int dy) {
    *x_ball += dx;
    *y_ball += dy;
}

void draw_rackets(int y_blue, int y_red, bool *is_inside_blue) {
    unsigned int col = hsv2rgb_lcd(0, 0, 255);
    for (int j = 0; j < 60; j++) {
        for (int i = 0; i < 10; i++) {
            draw_pixel_big(440, j + y_blue, col, 4);
        }
    }

    for (int j = 0; j < 60; j++) {
        for (int i = 0; i < 10; i++) {
            draw_pixel_big(40, j + y_red, col, 4);
        }
    }
}

void draw_score(int score_blue, int score_red) {
    char score_blue_char = score_blue + '0';
    char score_red_char = score_red + '0';
    draw_char(110, 80, score_red_char, hsv2rgb_lcd(260, 0, 45), 10);
    draw_char(290, 80, score_blue_char, hsv2rgb_lcd(260, 0, 45), 10);
}


#include "game_objects.h"
