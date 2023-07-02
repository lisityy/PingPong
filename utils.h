//
// Created by hella on 10.05.23.
//
#include <stdint.h>

#ifndef FELPONG_UTILS_H
#define FELPONG_UTILS_H

int char_width(int ch);
void lcd_timer(uint32_t *lcd_value, unsigned char *mem_base, int i);
void reset_timer(unsigned char *mem_base);
void blinking_lcd(unsigned char *mem_base);
void rgb_use(int label);
void blinking(int label);
int calculate_3_active_Button(int r_before, int r_now, int activeBut);
unsigned int hsv2rgb_lcd(int hue, int saturation, int value);
void prepare_for_round(unsigned char *parlcd_mem_base, struct timespec loop_delay, int number_round);


#endif //FELPONG_UTILS_H
