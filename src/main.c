#include <gb/gb.h>
#include <stdio.h>
#include "graphics.h"

void check_input(int * game_state) {
    switch (joypad()) {
        case J_START:
            if (*game_state == 0) {
                *game_state = 1;
                set_bkg_tiles(0, 0, 40, 18, gameMap);
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

void animate_manok(int x, int y) {
    set_sprite_tile(0, 4);
    move_sprite(0, x, y);
    set_sprite_tile(1, 6);
    move_sprite(1, x + 8, y);
    delay(80);
    set_sprite_tile(0, 8);
    move_sprite(0, x, y);
    set_sprite_tile(1, 10);
    move_sprite(1, x + 8, y);
}

void main() {
    int game_state = 0;
    int manok_x = 18;
    int manox_y = 97;

    init();

    while(1) {
        check_input(&game_state);

        if (game_state == 1) {
            // main game
            animate_manok(manok_x, manox_y);
            scroll_bkg(4, 0);
        }
        wait_vbl_done();
    }
    
}
