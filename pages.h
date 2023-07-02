#ifndef FELPONG_PAGES_H
#define FELPONG_PAGES_H

/*
 * Make main menu.
 * return : number of next menu (0 - play, 1 - settings, 2 - exit)
 * */
int main_page(unsigned char *mem_base, unsigned char *parlcd_mem_base);

void draw_menu();

/*
 * Make settings page.
 * Change ball's color, number of rounds, levels
 * */
void settings_page(unsigned char *mem_base, unsigned char *parlcd_mem_base);

void draw_settings(char *difficulty[]);

void game_start(unsigned char *mem_base, unsigned char *parlcd_mem_base);

void draw_game_over(unsigned char *parlcd_mem_base, struct timespec loop_delay, int num_win_round_blue,
                    int num_win_round_red);

#endif //FELPONG_PAGES_H
