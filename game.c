#define START_BALL_X 238
#define START_BALL_Y 158
#define MILLION 1000000
#define WIDTH 480
#define HEIGHT 320
#define RESET_BALL -100
#define NUMBER_EFFECTS 3
#define RESTART_LCD_TIMER 32
#define FREQUENCY_LCD_TIMER 27


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


int
create_round(unsigned char *mem_base, unsigned char *parlcd_mem_base, struct timespec loop_delay, int number_round, int
*num_win_round_blue, int *num_win_round_red) {
    int r = *(volatile uint32_t *) (mem_base + SPILED_REG_KNOBS_8BIT_o);
    if ((r & 0x2000000) != 0) {
        return 1;
    }
    int y_blue = 0;
    int y_red = 0;
    int y_blue_prev = 0;
    int y_red_prev = 0;
    bool is_inside_blue = true;
    int x_ball = START_BALL_X;
    int y_ball = START_BALL_Y;
    int radius_ball;
    int dx;
    int dy;
    int frequency_spawn_effect;
    int frequency_durattion_effect;
    int score_blue = 0;
    int score_red = 0;
    int x_ef = -10;
    int y_ef = -10;
    int code_ef = -10;
    bool is_effect = false;
    bool is_spawn_effect = false;
    int lcd_i = 0;
    uint32_t val_line = 0xffffff;
    clock_t spawn_effect_time, start_effect_time, current_time, time_before;
    reset_timer(mem_base);
    start_effect_time = 0;
    spawn_effect_time = 0;
    current_time = 0;
    if(diffNum == 0) {
        radius_ball = 7;
        dx = 12;
        dy = 0;
        frequency_spawn_effect = -1;
        frequency_durattion_effect = -1;
    }
    else if(diffNum == 1) {
        radius_ball = 5;
        dx = 15;
        dy = 0;
        frequency_spawn_effect = 15;
        frequency_durattion_effect = 10;
    }
    else if(diffNum == 2) {
        radius_ball = 4;
        dx = 18;
        dy = 0;
        frequency_spawn_effect = 10;
        frequency_durattion_effect = 10;
    }
    while (true) {
        int r = *(volatile uint32_t *) (mem_base + SPILED_REG_KNOBS_8BIT_o);
        if ((r & 0x2000000) != 0) {
            return 1;
        }
        if (check_win(score_blue, score_red, num_win_round_blue, num_win_round_red)) {
            return 0;
        }
        current_time = clock();
        y_blue = (((r & 0xff) * 320) / 256 * 5) % 320;
        y_red = ((((r >> 16) & 0xff) * 320 * 5) / 256) % 320;
        draw_board();
        draw_rackets(y_blue, y_red, &is_inside_blue);
        draw_score(score_blue, score_red);
        if(current_time / MILLION - spawn_effect_time == frequency_spawn_effect && is_spawn_effect){
            is_spawn_effect = false;
        }
        if((current_time / MILLION - start_effect_time == frequency_durattion_effect) && is_effect){
            reset_effect(code_ef, &dx, &radius_ball);
            is_effect = false;
            reset_timer(mem_base);
        }
        if (is_effect && (current_time / 10000 - time_before / 10000) > FREQUENCY_LCD_TIMER) {
            if (lcd_i == RESTART_LCD_TIMER) {
                lcd_i = 0;
            } 
            else {
                lcd_timer(&val_line, mem_base, lcd_i);
                lcd_i += 1;
            }
            time_before = clock();
        }
        if(current_time / MILLION % frequency_spawn_effect == 0 && !is_spawn_effect && !is_effect && frequency_durattion_effect != -1) {
            srand(time(NULL));
            code_ef = rand() % NUMBER_EFFECTS;
            x_ef = rand() % 356 + 60; //Field boundaries
            y_ef = rand() % 261 + 25; 
            spawn_effect_time = clock() / MILLION;
            is_spawn_effect = true;
        }
        draw_effect(code_ef, x_ef, y_ef);
        if(check_rebound_horizntal(&dx, &dy, x_ball, y_ball, &score_blue, &score_red)) {
            x_ball = START_BALL_X;
            y_ball = START_BALL_Y;
        }
        check_rebound_vertical(&dy, y_ball);
        check_rebound_rockets(&dx, &dy, x_ball, y_ball, y_blue, y_red, y_blue_prev, y_red_prev, radius_ball);
        if (check_effect(x_ball, y_ball, x_ef, y_ef, code_ef, &dx, &radius_ball)) {
            x_ef = RESET_BALL;
            y_ef = RESET_BALL;
            start_effect_time = clock() / MILLION;
            is_effect = true;
        }
        x_ball += dx;
        y_ball += dy;
        draw_ball(x_ball, y_ball, colorBall, radius_ball);
        y_blue_prev = y_blue;
        y_red_prev = y_red;
        parlcd_write_cmd(parlcd_mem_base, 0x2c);
        for (int ptr = 0; ptr < WIDTH * HEIGHT; ptr++) {
            parlcd_write_data(parlcd_mem_base, fb[ptr]);
        }
        clock_nanosleep(CLOCK_MONOTONIC, 0, &loop_delay, NULL);
    }
}


void update_positions(unsigned char *mem_base, int *y_blue, int *y_red, int r) {
    *y_blue = (((r & 0xff) * 320) / 256 * 5) % 320;
    *y_red = ((((r >> 16) & 0xff) * 320) / 256 * 5) % 320;
}

void write_frame_buffer(unsigned char *parlcd_mem_base) {
    parlcd_write_cmd(parlcd_mem_base, 0x2c);
    for (int ptr = 0; ptr < WIDTH * HEIGHT; ptr++) {
        parlcd_write_data(parlcd_mem_base, fb[ptr]);
    }
}

void sleep_for_delay(struct timespec loop_delay) {
    clock_nanosleep(CLOCK_MONOTONIC, 0, &loop_delay, NULL);
}


