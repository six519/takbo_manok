#include <gb/gb.h>
#include <stdio.h>
#include <stdlib.h>
#include "graphics.h"

void play_sound_jump() {
    NR10_REG = 0x16; 
    NR11_REG = 0x40;
    NR12_REG = 0x73;
    NR13_REG = 0x00; 
    NR14_REG = 0xC3;
}

void play_sound_start() {
    NR10_REG = 0x15;
    NR11_REG = 0x96;
    NR12_REG = 0x73;
    NR13_REG = 0xBB;
    NR14_REG = 0x85;
}

void play_sound_crash() {
    NR10_REG = 0x4F;
    NR11_REG = 0x96;
    NR12_REG = 0xB7;
    NR13_REG = 0xBB;
    NR14_REG = 0x85;
}

void check_input(int * game_state, int * can_jump) {
    switch (joypad()) {
        case J_START:
            if (*game_state == 0) {
                play_sound_start();
                *game_state = 1;
                set_bkg_tiles(0, 0, 40, 18, gameMap);
            }
            break;

        case J_B:
        case J_A:
            if (*game_state == 1) {
                if (*can_jump) {
                    play_sound_jump();
                    *can_jump = 0;
                }
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
    set_sprite_data(0, 56, manokTiles);
    HIDE_WIN;
    SHOW_SPRITES;
    SHOW_BKG;
    NR52_REG = 0x80;
    NR50_REG = 0x77;
    NR51_REG = 0xFF;
}

void show_score(int score) {
	char buffer[6];
    int i = 0;
    int multiplier = 7;
    int sprite_index = 11;
	itoa(score, buffer);

    set_sprite_tile(2, 12);
    move_sprite(2, 9, 17);
    set_sprite_tile(3, 14);
    move_sprite(3, 9 * 2, 17);
    set_sprite_tile(4, 16);
    move_sprite(4, 9 * 3, 17);
    set_sprite_tile(5, 18);
    move_sprite(5, 9 * 4, 17);
    set_sprite_tile(6, 20);
    move_sprite(6, 9 * 5, 17);
    set_sprite_tile(7, 22);
    move_sprite(7, 9 * 6, 17);

    for (i = 0; i < 6; i++) {
        switch (buffer[i]) {
        case '0':
            set_sprite_tile(sprite_index, 28);
            move_sprite(sprite_index, 9 * multiplier, 17);
        break;
        case '1':
            set_sprite_tile(sprite_index, 30);
            move_sprite(sprite_index, 9 * multiplier, 17);
        break;
        case '2':
            set_sprite_tile(sprite_index, 32);
            move_sprite(sprite_index, 9 * multiplier, 17);
        break;
        case '3':
            set_sprite_tile(sprite_index, 34);
            move_sprite(sprite_index, 9 * multiplier, 17);
        break;
        case '4':
            set_sprite_tile(sprite_index, 36);
            move_sprite(sprite_index, 9 * multiplier, 17);
        break;
        case '5':
            set_sprite_tile(sprite_index, 38);
            move_sprite(sprite_index, 9 * multiplier, 17);
        break;
        case '6':
            set_sprite_tile(sprite_index, 40);
            move_sprite(sprite_index, 9 * multiplier, 17);
        break;
        case '7':
            set_sprite_tile(sprite_index, 42);
            move_sprite(sprite_index, 9 * multiplier, 17);
        break;
        case '8':
            set_sprite_tile(sprite_index, 44);
            move_sprite(sprite_index, 9 * multiplier, 17);
        break;
        case '9':
            set_sprite_tile(sprite_index, 46);
            move_sprite(sprite_index, 9 * multiplier, 17);
        break;
        default:
            break;
        }
        sprite_index += 1;
        multiplier += 1;
    }
}

void show_lives(int lives) {
    int x = 0;
    int sprite_index = 8;
    int total_width = 9;
    int current_width = total_width;

    //clear first
    for(x =0; x < 3; x++) {
        set_sprite_tile(sprite_index, 26);
        move_sprite(sprite_index, current_width, 17 + 12);
        sprite_index += 1;
        current_width += total_width;        
    }

    sprite_index = 8;
    current_width = total_width;

    for (x = 0; x < lives; x++) {
        set_sprite_tile(sprite_index, 24);
        move_sprite(sprite_index, current_width, 17 + 12);
        sprite_index += 1;
        current_width += total_width;
    }
}

void spawn_obstacle(int sprite_index, int tile_index, int * ox, int sprite_y) {
    set_sprite_tile(sprite_index, tile_index);
    move_sprite(sprite_index, *ox, sprite_y);
    *ox -= 4;
    if (*ox == 0) {
        *ox = 168;
    }
}

int collided(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2) {
    int ret = 0;

    if (x1 < x2 + w2 && x1 + w1 > x2 && y1 < y2 + h2 && y1 + h1 > y2) {
        return 1;
    }

    return ret;
}

void handle_things(int * obstacle_1, int * obstacle_2, int score_to_change, int x, int y, int orig_y, int * lives, int * remove_lives) {
    show_score(score_to_change);
    spawn_obstacle(17, 48, obstacle_1, y);
    spawn_obstacle(18, 50, obstacle_2, y);
    if (collided(x, orig_y, 6, 10, *obstacle_1, y, 6, 10) || collided(x, orig_y, 6, 10, *obstacle_2, y, 6, 10)) {
        if (*remove_lives) {
            play_sound_crash();
            *lives -= 1;
            *remove_lives = 0;
        }
    }
}

void animate_manok(int x, int y, int * can_jump, int * score_to_change, int * obstacle_1, int * obstacle_2, int * lives, int * remove_lives) {
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
        handle_things(obstacle_1, obstacle_2, *score_to_change, x, y, orig_y, lives, remove_lives);
    } else {
        int a = 0;
        int b = 0;
        int reverse = 0;

        for (b=0; b<2; b++) {
            for (a=0; a<10; a++) {
                if (reverse) {
                    orig_y += additional_value;
                } else {
                    orig_y -= additional_value;
                }
                set_sprite_tile(0, 0);
                move_sprite(0, x, orig_y);
                set_sprite_tile(1, 2);
                move_sprite(1, x + 8, orig_y);
                *score_to_change += 1;
                handle_things(obstacle_1, obstacle_2, *score_to_change, x, y, orig_y, lives, remove_lives);
                delay(80);
                scroll_bkg(4, 0);
                if (a == 9) {
                    if(!reverse) {
                        reverse = 1;
                    } else {
                        *can_jump = 1;
                    }
                }
            }
        }
    }
    *score_to_change += 1;
}

void main() {
    int game_state = 0;
    int manok_x = 18;
    int manox_y = 97;
    int can_jump = 1;
    int score = 0;
    int lives = 3;
    int obstacle_x_1 = 168;
    int obstacle_x_2 = 264;
    int remove_lives = 1;
    int threshold = 0;

    init();

    while(1) {
        check_input(&game_state, &can_jump);

        if (game_state == 1) {
            // main game
            animate_manok(manok_x, manox_y, &can_jump, &score, &obstacle_x_1, &obstacle_x_2, &lives, &remove_lives);
            show_lives(lives);
            scroll_bkg(4, 0);
            
            if (!remove_lives) {
                threshold += 1;
                if (threshold == 5) {
                    threshold = 0;
                    remove_lives = 1;
                }
            }

            if (lives == 0) {
                game_state = 2;
            }
        }
        wait_vbl_done();
    }
    
}
