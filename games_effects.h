#include <stdbool.h>

#ifndef FELPONGC_GAMES_EFFECTS_H
#define FELPONGC_GAMES_EFFECTS_H

bool check_effect(int x_ball, int y_ball, int x_ef, int y_ef, int code_ef, int *dx, int *radius);

void reset_spawn_effect(bool *is_spawn_effect, bool *is_effect, int *code_ef, int *x_ef, int *y_ef,
                        clock_t *spawn_effect_time);

void reset_effect_check(bool *is_effect, int *code_ef, int *dx, int *radius_ball, clock_t *start_effect_time,
                        clock_t *current_time);

void spawn_effect_check(bool *is_spawn_effect, bool *is_effect, int *code_ef, int *x_ef, int *y_ef,
                        clock_t *spawn_effect_time, clock_t *current_time);

void draw_bomb(int x, int y);

void draw_effect(int code_ef, int x_ef, int y_ef);

void reset_effect(int code_ef, int *dx, int *radius);

void draw_speed2x(int x, int y);

void draw_snowflake(int x, int y);

void draw_lightning(int x, int y);

#endif //FELPONGC_GAMES_EFFECTS_H
