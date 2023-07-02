#include <stdbool.h>

#ifndef FELPONGC_GAME_OBJECTS_H
#define FELPONGC_GAME_OBJECTS_H

void reset_ball_position(int *x_ball, int *y_ball);

void update_ball_position(int *x_ball, int *y_ball, int dx, int dy);

void draw_rackets(int y_blue, int y_red, bool *is_inside_blue);

void draw_score(int score_blue, int score_red);

#endif //FELPONGC_GAME_OBJECTS_H
