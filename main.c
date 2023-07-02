/*******************************************************************
  Project main function template for MicroZed based MZ_APO board
  designed by Petr Porazil at PiKRON

  Project by Pimenova Olga and Robert Bakhtigariev
 
 *******************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "font_types.h"
#include "mzapo_parlcd.h"
#include "mzapo_phys.h"
#include "mzapo_regs.h"
#include "pages.h"
#include "utils.h"

unsigned short *fb;
font_descriptor_t *fdes;
unsigned short colorBall;
int diffNum;
int levels;
int width_letter;

void set_default_value();

int main(int argc, char *argv[]) {

    unsigned char *parlcd_mem_base, *mem_base;
    int ptr;

    set_default_value();
    parlcd_mem_base = map_phys_address(PARLCD_REG_BASE_PHYS, PARLCD_REG_SIZE, 0);
    if (parlcd_mem_base == NULL)
        exit(1);

    mem_base = map_phys_address(SPILED_REG_BASE_PHYS, SPILED_REG_SIZE, 0);
    if (mem_base == NULL)
        exit(1);

    parlcd_hx8357_init(parlcd_mem_base);

    printf("Start game\n");
    while (1) {
        *(volatile uint32_t*)(mem_base + SPILED_REG_LED_LINE_o) = 0;
        int ret = main_page(mem_base, parlcd_mem_base);
        if (ret == 0) {
            //PLAY
            game_start(mem_base, parlcd_mem_base);
        } else if (ret == 1) {
            //SETTINGS
            settings_page(mem_base, parlcd_mem_base);
        } else if (ret == 2) {
            //EXIT
            printf("Thank you for playing!\n");
            rgb_use(0);
            break;
        }

    }

    parlcd_write_cmd(parlcd_mem_base, 0x2c);
    for (ptr = 0; ptr < 480 * 320; ptr++) {
        parlcd_write_data(parlcd_mem_base, 0);
    }

    return 0;
}

void set_default_value(){
    fb = (unsigned short *) malloc(320 * 480 * 2);
    fdes = &font_rom8x16;
    width_letter = 5;
    levels = 3;
    diffNum = 1;
    rgb_use(0);
    colorBall = hsv2rgb_lcd(0, 315, 250);
}

