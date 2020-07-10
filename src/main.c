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
    set_bkg_data(0, 19, backgroundTiles);
    set_bkg_tiles(0, 0, 40, 18, gameTitleMap);
    HIDE_WIN;
    SHOW_SPRITES;
    SHOW_BKG;
}

void main() {
    int game_state = 0;

    init();

    while(1) {
        check_input(&game_state);

        if (game_state == 1) {
            // main game
            scroll_bkg(1, 0);
        }
        wait_vbl_done();
    }
    
}
