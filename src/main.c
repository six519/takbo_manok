#include <gb/gb.h>
#include <stdio.h>
#include "graphics.h"

void check_input(int * game_state, int * can_jump) {
    switch (joypad()) {
        case J_START:
            if (*game_state == 0) {
                *game_state = 1;
                set_bkg_tiles(0, 0, 40, 18, gameMap);
            }
            break;

        case J_B:
        case J_A:
            if (*can_jump) {
                *can_jump = 0;
            }
        break;
        default:
            break;
    }
}

void init() {
    DISPLAY_ON;
    SPRITES_8x16;
    set_bkg_data(0, 19, backgroundTiles);
    set_bkg_tiles(0, 0, 40, 18, gameTitleMap);
    set_sprite_data(0, 12, manokTiles);
    HIDE_WIN;
    SHOW_SPRITES;
    SHOW_BKG;
}

void animate_manok(int x, int y, int * can_jump) {
    int additional_value = 2;
    int orig_y = y;
    if (*can_jump) {
        set_sprite_tile(0, 4);
        move_sprite(0, x, y);
        set_sprite_tile(1, 6);
        move_sprite(1, x + 8, y);
        delay(80);
        set_sprite_tile(0, 8);
        move_sprite(0, x, y);
        set_sprite_tile(1, 10);
        move_sprite(1, x + 8, y);
    } else {
        int a = 0;
        int b = 0;
        int reverse = 0;

        for (b=0; b<2; b++) {
            for (a=0; a<5; a++) {
                if (reverse) {
                    orig_y += additional_value;
                } else {
                    orig_y -= additional_value;
                }
                set_sprite_tile(0, 0);
                move_sprite(0, x, orig_y);
                set_sprite_tile(1, 2);
                move_sprite(1, x + 8, orig_y);
                delay(80);
                scroll_bkg(4, 0);
                if (a == 4) {
                    if(!reverse) {
                        reverse = 1;
                    } else {
                        *can_jump = 1;
                    }
                }
            }
        }
    }
}

void main() {
    int game_state = 0;
    int manok_x = 18;
    int manox_y = 97;
    int can_jump = 1;

    init();

    while(1) {
        check_input(&game_state, &can_jump);

        if (game_state == 1) {
            // main game
            animate_manok(manok_x, manox_y, &can_jump);
            scroll_bkg(4, 0);
        }
        wait_vbl_done();
    }
    
}
