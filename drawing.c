#define _POSIX_C_SOURCE 200112L
#define HIGH 320
#define WIDTH 480

#include "font_types.h"
#include "drawing.h"
#include "game.h"
#include <math.h>
#include <stdio.h>

extern unsigned short *fb;
extern font_descriptor_t *fdes;
extern int width_letter;

void draw_circles_around(unsigned short color, int activeBut) {
    if (activeBut == 0) {

        draw_ball(155, 190, color, 4);
        draw_ball(320, 190, color, 4);
    } else if (activeBut == 1) {

        draw_ball(116, 230, color, 4);
        draw_ball(375, 230, color, 4);
    } else if (activeBut == 2) {

        draw_ball(167, 270, color, 4);
        draw_ball(310, 270, color, 4);
    }
}

void draw_background(unsigned short color) {
    for (int i = 0; i < 320 * 480; i++) {
        fb[i] = color;
    }
}

void draw_pixel(int x, int y, unsigned short color) {
    if (x >= 0 && x < WIDTH && y >= 0 && y < HIGH) {
        for (int k = 0; k < width_letter; k++) {
            fb[x + k + WIDTH * y] = color;
        }
    }
}

void draw_rectangle(int x, int y, int yEnd, int xEnd, unsigned short color) {
    int width = 4;
    for (int j = y; j < yEnd; j++) {
        for (int i = x; i < xEnd; i++) {
            if (i < x + width || i > xEnd - width || j < y + width || j > yEnd - width) {
                draw_pixel(i, j, color);
            }
        }
    }
}


void draw_char(int x, int y, char ch, unsigned short color, int scale) {
    int w = char_width(ch);
    const font_bits_t *ptr;
    if ((ch >= fdes->firstchar) && (ch - fdes->firstchar < fdes->size)) {
        if (fdes->offset) {
            ptr = &fdes->bits[fdes->offset[ch - fdes->firstchar]];
        } else {
            int bw = (fdes->maxwidth + 15) / 16;
            ptr = &fdes->bits[(ch - fdes->firstchar) * bw * fdes->height];
        }
        int i, j;
        for (i = 0; i < fdes->height; i++) {
            font_bits_t val = *ptr;
            for (j = 0; j < w; j++) {
                if ((val & 0x8000) != 0) {
                    draw_pixel_big(x + scale * j, y + scale * i, color, scale);
                }
                val <<= 1;
            }
            ptr++;
        }
    }
}


void draw_ball(int center_x, int center_y, unsigned short color, int radius) {
    int x = 0;
    int y = radius;
    int d = 3 - 2 * radius;

    while (x <= y) {
        for (int i = center_x - x; i <= center_x + x; i++) {
            draw_pixel(i, center_y + y, color);
            draw_pixel(i, center_y - y, color);
        }
        for (int i = center_x - y; i <= center_x + y; i++) {
            draw_pixel(i, center_y + x, color);
            draw_pixel(i, center_y - x, color);
        }
        if (d < 0) {
            d = d + 4 * x + 6;
        } else {
            d = d + 4 * (x - y) + 10;
            y--;
        }
        x++;
    }
}


void draw_pixel_big(int x, int y, unsigned short color, int scale) {
    int i, j;
    for (i = 0; i < scale; i++) {
        for (j = 0; j < scale; j++) {
            draw_pixel(x + i, y + j, color);
        }
    }
}

void draw_word_middle(char *word, int x, int y, unsigned short color) {
    int i = 0;
    int widthBefor = 0;
    int scale = 3;
    while (word[i] != '\0') {
        if (i == 0) {
            draw_char(x, y, word[i], color, scale);
            widthBefor = x;
        } else {
            widthBefor += 20 + char_width(word[i - 1]);
            draw_char(widthBefor, y, word[i], color, scale);
        }
        i++;
    }
}

void draw_little_word(char *word, int x, int y, unsigned short color) {
    int i = 0;
    int widthBefor = 0;
    int scale = 1;
    while (word[i] != '\0') {
        if (i == 0) {
            draw_char(x, y, word[i], color, scale);
            widthBefor = x;
        } else {
            widthBefor += 7 + char_width(word[i - 1]);
            draw_char(widthBefor, y, word[i], color, scale);
        }
        i++;
    }
}

void draw_big_word(char *word, int x, int y, unsigned short color) {
    int i = 0;
    int widthBefor = 0;
    int scale = 6;
    while (word[i] != '\0') {
        if (i == 0) {
            draw_char(x, y, word[i], color, scale);
            widthBefor = x;
        } else {
            widthBefor += 50 + char_width(word[i - 1]);
            draw_char(widthBefor, y, word[i], color, scale);
        }
        i++;
    }
}

void draw_empty_circle(int center_x, int center_y, int radius, unsigned short color, int scale) {
    int x = 0;
    int y = radius;
    int d = 3 - 2 * radius;

    while (x <= y) {
        draw_pixel_big(center_x + x, center_y + y, color, scale);
        draw_pixel_big(center_x + y, center_y + x, color, scale);
        draw_pixel_big(center_x - x, center_y + y, color, scale);
        draw_pixel_big(center_x - y, center_y + x, color, scale);
        draw_pixel_big(center_x + x, center_y - y, color, scale);
        draw_pixel_big(center_x + y, center_y - x, color, scale);
        draw_pixel_big(center_x - x, center_y - y, color, scale);
        draw_pixel_big(center_x - y, center_y - x, color, scale);

        if (d < 0) {
            d = d + 4 * x + 6;
        } else {
            d = d + 4 * (x - y) + 10;
            y--;
        }
        x++;
    }
}

void draw_active_button_settings(int activeBut){
    unsigned int colorCircle = hsv2rgb_lcd(250, 315, 250);
    if (activeBut == 0) {
        draw_ball(50, 115, colorCircle, 4);
    } else if (activeBut == 1) {
        draw_ball(50, 180, colorCircle, 4);
    } else if (activeBut == 2) {
        draw_ball(50, 250, colorCircle, 4);
    }
}
