#define START_BALL_X 238
#define START_BALL_Y 158
#define MILLION 1000000

#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>

#include "font_types.h"
#include "mzapo_parlcd.h"
#include "mzapo_regs.h"
#include "drawing.h"
#include "utils.h"
#include "game.h"
#include "game_objects.h"
#include "checker_game.h"
#include "draw_game.h"
#include "games_effects.h"

extern unsigned short *fb;
extern font_descriptor_t *fdes;
extern unsigned short colorBall;
extern int diffNum;
extern int levels;
extern int width_letter;


bool check_effect(int x_ball, int y_ball, int x_ef, int y_ef, int code_ef, int *dx, int *radius) {
    if (x_ball >= (x_ef - 20) && x_ball <= (x_ef + 20) && y_ball >= (y_ef - 20) && y_ball <= (y_ef + 20)) {
        if (code_ef == 0) {
            if(*dx > 0)
                *dx = *dx + 7;
            else 
                *dx = *dx - 7;
            return true;
        } 
        else if (code_ef == 1) {
            *dx = *dx / 2;
            return true;
        } 
        else if (code_ef == 2) {
            *radius = *radius * 4;
            return true;
        }
    }
    return false;
}

void reset_spawn_effect(bool *is_spawn_effect, bool *is_effect, int *code_ef, int *x_ef, int *y_ef, clock_t *spawn_effect_time) {
    clock_t current_time = clock();
    if (current_time / MILLION - *spawn_effect_time == 15 && *is_spawn_effect) {
        printf("Reset spawn effect\n");
        *is_spawn_effect = false;
        *x_ef = -100;
        *y_ef = -100;
    }
}

void reset_effect_check(bool *is_effect, int *code_ef, int *dx, int *radius_ball, clock_t *start_effect_time, clock_t *current_time) {
    if ((*current_time / MILLION - *start_effect_time == 10) && *is_effect) {
        printf("Reset effect\n");
        reset_effect(*code_ef, dx, radius_ball);
        *is_effect = false;
    }
}

void spawn_effect_check(bool *is_spawn_effect, bool *is_effect, int *code_ef, int *x_ef, int *y_ef, clock_t *spawn_effect_time, clock_t *current_time) {
    if (*current_time / MILLION % 10 == 0 && !*is_spawn_effect && !*is_effect) {
        printf("Spawn effect\n");
        srand(time(NULL));
        *code_ef = rand() % 5;
        *x_ef = rand() % 356 + 60;
        *y_ef = rand() % 261 + 25;
        *spawn_effect_time = clock() / MILLION;
        *is_spawn_effect = true;
    }
}

void draw_bomb(int x, int y) {
    draw_ball(x, y, 0, 12);
    draw_pixel_big(x + 3, y - 14, 0, 5);
    for (int i = 0; i < 5; i++)
        draw_pixel_big(x + 5 + i, y - 15 - i, 0, 2);
    draw_pixel_big(x + 10, y - 19, hsv2rgb_lcd(45, 255, 255), 2);
    draw_pixel_big(x + 11, y - 19, hsv2rgb_lcd(45, 255, 255), 2);
    draw_pixel_big(x + 10, y - 20, hsv2rgb_lcd(45, 255, 255), 2);
    draw_pixel_big(x + 11, y - 20, hsv2rgb_lcd(45, 255, 255), 2);
}

void draw_effect(int code_ef, int x_ef, int y_ef) {
    if (code_ef == 0)
        draw_lightning(x_ef, y_ef);
    else if (code_ef == 1)
        draw_snowflake(x_ef, y_ef);
    else if (code_ef == 2)
        draw_speed2x(x_ef, y_ef);
    // else if(code_ef == 3)
    //     draw_bomb(x_ef, y_ef); // разобраться по позже.
}

void reset_effect(int code_ef, int *dx, int *radius){
    if (code_ef == 0) {
        if(*dx < 0)
            *dx = *dx + 7;
        else 
            *dx = *dx - 7;
    }
    if (code_ef == 1) {
        *dx = *dx * 2;
    }
    else if (code_ef == 2) {
        *radius = *radius / 4;
    }
}

void draw_speed2x(int x, int y) {
    draw_char(x, y, '2', hsv2rgb_lcd(45, 255, 255), 1);
    draw_char(x + 10, y, 'x', hsv2rgb_lcd(45, 255, 255), 1);
    x += 8;
    y += 6;
    draw_empty_circle(x, y, 14, hsv2rgb_lcd(45, 255, 255),2);
}

void draw_snowflake(int x, int y) {
    width_letter = 2;
    for (int i = 0; i < 26; i++) {
        draw_pixel(x + i, y + 12, hsv2rgb_lcd(180, 255, 255));
        draw_pixel(x + i, y + 13, hsv2rgb_lcd(180, 255, 255));
        draw_pixel(x + 12, y + i, hsv2rgb_lcd(180, 255, 255));
    }
    for (int i = 0; i < 7; i++) {
        draw_pixel(x - 1 + i, y + 18 - i, hsv2rgb_lcd(180, 255, 255));
        draw_pixel(x - 1 + i, y + 6 + i, hsv2rgb_lcd(180, 255, 255));
        draw_pixel(x + 25 - i, y + 18 - i, hsv2rgb_lcd(180, 255, 255));
        draw_pixel(x + 25 - i, y + 6 + i, hsv2rgb_lcd(180, 255, 255));
        draw_pixel(x + 14 + i, y + 19 + i, hsv2rgb_lcd(180, 255, 255));
        draw_pixel(x + 10 - i, y + 19 + i, hsv2rgb_lcd(180, 255, 255));
        draw_pixel(x + 14 + i, y + 5 - i, hsv2rgb_lcd(180, 255, 255));
        draw_pixel(x + 10 - i, y + 5 - i, hsv2rgb_lcd(180, 255, 255));
    }

    width_letter = 1;
}

void draw_lightning(int x, int y) {
    width_letter = 4;
    for (int i = 0; i < 13; i++)
        draw_pixel(x - i, y + i, hsv2rgb_lcd(60, 255, 255));
    for (int i = 0; i < 13; i++)
        draw_pixel(x - 13 + i, y + 13, hsv2rgb_lcd(60, 255, 255));
    for (int i = 0; i < 13; i++)
        draw_pixel(x - 2 - i, y + 14 + i, hsv2rgb_lcd(60, 255, 255));
    width_letter = 1;
}
