#include <stdbool.h>
#include <time.h>

#ifndef FELPONGC_GAME_H
#define FELPONGC_GAME_H


int
create_round(unsigned char *mem_base, unsigned char *parlcd_mem_base, struct timespec loop_delay, int number_round, int
*num_win_round_blue, int *num_win_round_red);

void update_positions(unsigned char *mem_base, int *y_blue, int *y_red, int r);

void write_frame_buffer(unsigned char *parlcd_mem_base);

void sleep_for_delay(struct timespec loop_delay);

#endif //FELPONGC_GAME_H





