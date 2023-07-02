#include <stdbool.h>

#ifndef FELPONGC_CHECKER_GAME_H
#define FELPONGC_CHECKER_GAME_H

bool check_win(int score_blue, int score_red, int *num_win_round_blue, int *num_win_round_red);

bool check_rebound_horizntal(int *dx, int *dy, int x_ball, int y_ball, int *score_blue, int *score_red);

void check_rebound_vertical(int *dy, int y_ball);

void check_rebound_rockets(int *dx, int *dy, int x_ball, int y_ball,int y_blue, int y_red, int y_blue_prev,
                           int y_red_prev, int radius_ball);

#endif //FELPONGC_CHECKER_GAME_H
