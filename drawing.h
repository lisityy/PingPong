//
// Created by hella on 10.05.23.
//

#ifndef FELPONG_DRAWING_H
#define FELPONG_DRAWING_H

/*
 * Draw circles around active button in main menu
 * */
void draw_circles_around(unsigned short color, int activeBut);

void draw_background(unsigned short color);

void draw_pixel(int x, int y, unsigned short color);

void draw_rectangle(int x, int y, int yEnd, int xEnd, unsigned short color);

void draw_char(int x, int y, char ch, unsigned short color, int scale);

void draw_ball(int center_x, int center_y, unsigned short color, int radius);

void draw_pixel_big(int x, int y, unsigned short color, int scale);

void draw_word_middle(char *word, int x, int y, unsigned short color);

void draw_little_word(char *word, int x, int y, unsigned short color);

void draw_big_word(char *word, int x, int y, unsigned short color);

void draw_empty_circle(int center_x, int center_y, int radius, unsigned short color, int scale);

/*
 * Draw circles around active button in settings
 * */
void draw_active_button_settings(int activeBut);

#endif //FELPONG_DRAWING_H
