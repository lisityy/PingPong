#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>

#include "utils.h"
#include "game.h"
#include "game_objects.h"
#include "checker_game.h"
#include "pages.h"

extern int diffNum;

bool check_win(int score_blue, int score_red, int *num_win_round_blue, int *num_win_round_red) {
    if (score_blue > 9) {
        *num_win_round_blue = *num_win_round_blue + 1;
        return true;
    } else if (score_red > 9) {
        *num_win_round_red = *num_win_round_red + 1;
        return true;
    }
    return false;
}

bool check_rebound_horizntal(int *dx, int *dy, int x_ball, int y_ball, int *score_blue, int *score_red) {
    srand(time(NULL));
    if (x_ball > 455) {
        *dx = -*dx;
        *dy = 0;
        *score_red = *score_red + 1;
        return true;
    } else if (x_ball < 16) {
        *dx = -*dx;
        *dy = 0;
        *score_blue = *score_blue + 1;
        return true;
    }
    return false;
}

void check_rebound_vertical(int *dy, int y_ball) {
    if (y_ball < 10) {
        *dy = -*dy;
    } else if (y_ball > 300) {
        *dy = -*dy;
    }
}

void check_rebound_rockets(int *dx, int *dy, int x_ball, int y_ball, int y_blue, int y_red, int y_blue_prev,
                           int y_red_prev, int radius_ball) {
    srand(time(NULL));
    int dx_size = 0;
    if(abs(*dx) == 15)
        dx_size = 5;
    else if(abs(*dx) == 18)
        dx_size = 6;
    if(radius_ball == 20 && abs(*dx) == 15)
        dx_size = 20;
    else if(radius_ball == 16 && abs(*dx) == 18)
        dx_size = 9;
    int random_vector = rand() % (4 * 2 + 1) - 4;
    if (x_ball > 425 - dx_size  && x_ball < 440 && y_ball <= y_blue + 70 && y_ball >= y_blue - 10) {
        printf("BLUE %d\n", x_ball);
        *dx = -*dx;
        *dy = (y_blue - y_blue_prev) * 0.5 + random_vector;
    } else if (x_ball < 50 + dx_size && x_ball > 35 && y_ball <= y_red + 70 && y_ball >= y_red - 10) {
        printf("RED %d\n", x_ball);
        *dx = -*dx;
        *dy = (y_red - y_red_prev) * 0.5 + random_vector;
    }
}