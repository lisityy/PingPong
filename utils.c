#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <math.h>

#include "utils.h"
#include "font_types.h"
#include "mzapo_parlcd.h"
#include "mzapo_phys.h"
#include "mzapo_regs.h"
#include "game_objects.h"

extern unsigned short *fb;
extern font_descriptor_t *fdes;
extern int width_letter;
//uint32_t val_line;

int char_width(int ch) {
    int width;
    if (!fdes->width) {
        width = fdes->maxwidth;
    } else {
        width = fdes->width[ch - fdes->firstchar];
    }
    return width;
}
void lcd_timer(uint32_t *lcd_value, unsigned char *mem_base, int i){
    if(i == 0){
        *lcd_value = 4294967295;
    }
    *lcd_value -= pow(2, 32 - 1 - i);
    *(volatile uint32_t*)(mem_base + SPILED_REG_LED_LINE_o) = *lcd_value;
}

void reset_timer(unsigned char *mem_base) {
    *(volatile uint32_t*)(mem_base + SPILED_REG_LED_LINE_o) = 0;
}

void rgb_use(int label) {
    unsigned char *mem_base;
    mem_base = map_phys_address(SPILED_REG_BASE_PHYS, SPILED_REG_SIZE, 0);
    if (label == 1) {
        *(volatile uint32_t *) (mem_base + SPILED_REG_LED_RGB1_o) = 0x0000ff;
    } else if (label == 2) {
        *(volatile uint32_t *) (mem_base + SPILED_REG_LED_RGB2_o) = 0x0000ff;
    } else if (label == 0) {
        *(volatile uint32_t *) (mem_base + SPILED_REG_LED_RGB2_o) = 0;
        *(volatile uint32_t *) (mem_base + SPILED_REG_LED_RGB1_o) = 0;
    }

}

void blinking(int label) {
    rgb_use(0);
    usleep(100000);
    rgb_use(label);
    usleep(100000);
    rgb_use(0);
    usleep(100000);
    rgb_use(label);
    usleep(100000);
    rgb_use(0);
    usleep(100000);
    rgb_use(label);
    usleep(100000);
    rgb_use(0);
}

int calculate_3_active_Button(int r_before, int r_now, int activeBut) {
    if (r_now > r_before) {
        if (activeBut == 0) return 1;
        else if (activeBut == 1) return 2;
    } else if (r_now < r_before) {
        if (activeBut == 1) return 0;
        else if (activeBut == 2) return 1;
    }
    return activeBut;
}

unsigned int hsv2rgb_lcd(int hue, int saturation, int value) {
    hue = (hue % 360);
    float f = ((hue % 60) / 60.0);
    int p = (value * (255 - saturation)) / 255;
    int q = (value * (255 - (saturation * f))) / 255;
    int t = (value * (255 - (saturation * (1.0 - f)))) / 255;
    unsigned int r, g, b;
    if (hue < 60) {
        r = value;
        g = t;
        b = p;
    } else if (hue < 120) {
        r = q;
        g = value;
        b = p;
    } else if (hue < 180) {
        r = p;
        g = value;
        b = t;
    } else if (hue < 240) {
        r = p;
        g = q;
        b = value;
    } else if (hue < 300) {
        r = t;
        g = p;
        b = value;
    } else {
        r = value;
        g = p;
        b = q;
    }
    r >>= 3;
    g >>= 2;
    b >>= 3;
    return (((r & 0x1f) << 11) | ((g & 0x3f) << 5) | (b & 0x1f));
}

void prepare_for_round(unsigned char *parlcd_mem_base, struct timespec loop_delay, int number_round) {
    char number_round_info[] = "ROUND";
    char number_round_char = number_round + '0';
    for (int i = 3; i >= 0; i--) {
        draw_empty_board();
        char i_char = i + '0';
        width_letter = 5;
        draw_char(195, 95, i_char, hsv2rgb_lcd(0, 0, 255), 12);
        draw_word_middle(number_round_info, 145, 40, hsv2rgb_lcd(210, 240, 220));
        draw_char(295, 40, number_round_char, hsv2rgb_lcd(210, 240, 220), 3);
        width_letter = 1;
        parlcd_write_cmd(parlcd_mem_base, 0x2c);
        for (int ptr = 0; ptr < 480 * 320; ptr++) {
            parlcd_write_data(parlcd_mem_base, fb[ptr]);
        }
        sleep(1);
        clock_nanosleep(CLOCK_MONOTONIC, 0, &loop_delay, NULL);
    }
}
