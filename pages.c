#define _POSIX_C_SOURCE 200112L
#define HIGH 320
#define WIDTH 480
#define ERROR 100

#include <time.h>
#include <string.h>

#include "font_types.h"
#include "mzapo_parlcd.h"
#include "mzapo_regs.h"
#include "drawing.h"
#include "utils.h"
#include "pages.h"
#include "game.h"

extern unsigned short *fb;
extern font_descriptor_t *fdes;
extern unsigned short colorBall;
extern int diffNum;
extern int levels;
extern int width_letter;

int main_page(unsigned char *mem_base, unsigned char *parlcd_mem_base) {
    struct timespec loop_delay;
    loop_delay.tv_sec = 0;
    loop_delay.tv_nsec = 150 * 1000 * 1000;


    short activeBut = 0; // 0 - play, 1 - set, 2 - exit
    int r_blue_before = 0;

    while (true) {
        int r = *(volatile uint32_t *) (mem_base + SPILED_REG_KNOBS_8BIT_o);
        if ((r & 0x2000000) != 0) {
            return ERROR;
        }
        int r_blue = r & 0xff;

        if ((r & 0x1000000) != 0) {
            return  activeBut;
        }
        activeBut = calculate_3_active_Button(r_blue_before, r_blue, activeBut);
        draw_menu();
        draw_circles_around(hsv2rgb_lcd(250, 315, 250), activeBut);
        write_frame_buffer(parlcd_mem_base);
        clock_nanosleep(CLOCK_MONOTONIC, 0, &loop_delay, NULL);
        r_blue_before = r_blue;
    }
}

void draw_menu() {
    unsigned short colorButt = hsv2rgb_lcd(0, 0, 255);

    draw_background(hsv2rgb_lcd(260, 0, 70));
    draw_big_word("FEL PONG", 20, 20, hsv2rgb_lcd(200, 240, 220));
    draw_word_middle("PLAY", 186, 166, colorButt);
    draw_word_middle("SETTINGS", 140, 206, colorButt);
    draw_word_middle("EXIT", 186, 246, colorButt);

}

void settings_page(unsigned char *mem_base, unsigned char *parlcd_mem_base) {

    struct timespec loop_delay;
    loop_delay.tv_sec = 0;
    loop_delay.tv_nsec = 150 * 1000 * 1000;


    short activeBut = 0; // 0 -boll, 1- levels, 2- diff, 3 - exit
    int r_blue_before = 0;
    int r_red_before = 0;


    char *difficulty[] = {"easy", "mid", "hard"}; // easy, hard, middle
    diffNum = 0;

    while (1) {
        int r = *(volatile uint32_t *) (mem_base + SPILED_REG_KNOBS_8BIT_o);
        if ((r & 0x2000000) != 0) {
            return;
        }
        int r_blue = (r) & 0xff;
        int r_red = (r >> 16) & 0xff;

        if (r_red != r_red_before) {
            if (activeBut == 0) {
                //ball
                colorBall = hsv2rgb_lcd(r_red, 315, 250);
            } else if (activeBut == 1) {
                levels = ((r_red / 5) % 5) + 1;
                //levels
            } else if (activeBut == 2) {
                //difficulty
                diffNum = ((r_red) / 5) % 3;
            }
        }
        activeBut = calculate_3_active_Button(r_blue_before, r_blue, activeBut);
        draw_settings(difficulty);
        draw_active_button_settings(activeBut);
        write_frame_buffer(parlcd_mem_base);
        clock_nanosleep(CLOCK_MONOTONIC, 0, &loop_delay, NULL);

        r_blue_before = r_blue;
        r_red_before = r_red;
    }
}

void draw_settings(char *difficulty[]) {
    unsigned short colorButt = hsv2rgb_lcd(0, 0, 255);

    draw_background(hsv2rgb_lcd(260, 0, 70));

    draw_rectangle(120, -10, 50, 370, colorButt);
    draw_word_middle("SETTINGS", 140, 0, colorButt);

    draw_word_middle("BALL", 70, 100, colorButt);
    draw_ball(270, 110, colorBall, 30);

    draw_word_middle("ROUNDS", 70, 165, colorButt);
    draw_char(250, 165, '<', colorButt, 3);
    draw_char(275, 165, '0' + levels, colorButt, 3);
    draw_char(300, 165, '>', colorButt, 3);

    draw_word_middle("LEVELS", 70, 230, colorButt);
    char st[20] = {0};
    strcat(st, "<");
    strcat(st, difficulty[diffNum]);
    strcat(st, ">");
    draw_word_middle(st, 250, 230, colorButt);

}


void game_start(unsigned char *mem_base, unsigned char *parlcd_mem_base) {

    struct timespec loop_delay;
    loop_delay.tv_sec = 0;
    loop_delay.tv_nsec = 1000 * 1000;

    int number_round = 0;
    int num_win_round_blue = 0;
    int num_win_round_red = 0;

    int result;
    while (true) {
        number_round = number_round + 1;
        prepare_for_round(parlcd_mem_base, loop_delay, number_round);
        result = create_round(mem_base, parlcd_mem_base, loop_delay, number_round, &num_win_round_blue,
                              &num_win_round_red);
        if (result == 1) {
            width_letter = 5;
            return;
        }
        if (num_win_round_blue > num_win_round_red) {
            rgb_use(0);
            rgb_use(2);
        } else if (num_win_round_blue < num_win_round_red) {
            rgb_use(0);
            rgb_use(1);
        }
        if (number_round == levels) {
            if (num_win_round_red > num_win_round_blue) {
                blinking(1);
            } else if (num_win_round_blue > num_win_round_red) {
                blinking(2);
            }
            draw_game_over(parlcd_mem_base, loop_delay, num_win_round_blue, num_win_round_red);
            rgb_use(0);
            return;
        }
    }
}

void draw_game_over(unsigned char *parlcd_mem_base, struct timespec loop_delay, int num_win_round_blue,
                    int num_win_round_red) {

    char num_win_round_blue_char = num_win_round_blue + '0';
    char num_win_round_red_char = num_win_round_red + '0';
    width_letter = 5;
    int i = 0;
    while (true) {
        i++;
        if (i > 50) {
            width_letter = 1;
            return;
        }
        draw_empty_board();
        char win_game_info1[] = "GAME OVER\0";
        draw_word_middle(win_game_info1, 110, 80, hsv2rgb_lcd(210, 240, 220));
        draw_char(110, 120, num_win_round_red_char, hsv2rgb_lcd(0, 0, 255), 10);
        draw_char(290, 120, num_win_round_blue_char, hsv2rgb_lcd(0, 0, 255), 10);
        draw_char(185, 110, ':', hsv2rgb_lcd(0, 0, 255), 10);
        for (int ptr = 0; ptr < 480 * 320; ptr++) {
            parlcd_write_data(parlcd_mem_base, fb[ptr]);
        }
        clock_nanosleep(CLOCK_MONOTONIC, 0, &loop_delay, NULL);

    }

}