#include "raylib.h"
#include "globals.h"
#include "level.h"
#include "player.h"
#include "graphics.h"
#include "assets.h"
#include "utilities.h"

void update_game() {
    game_frame++;

    if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) {
        move_player_horizontally(MOVEMENT_SPEED);
    }

    if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) {
        move_player_horizontally(-MOVEMENT_SPEED);
    }

    // Calculating collisions to decide whether the player is allowed to jump:
    // don't want them to suction cup to the ceiling or jump midair
    is_player_on_ground = is_colliding({player_pos.x, player_pos.y + 0.1f}, WALL);

    if ((IsKeyDown(KEY_UP) || IsKeyDown(KEY_W) || IsKeyDown(KEY_SPACE)) && is_player_on_ground) {
        player_y_velocity = -JUMP_STRENGTH;
        PlaySound(jump_sound);
    }

    update_player();
}

void draw_game() {
    // TODO
    ClearBackground(BLACK);
    draw_level();
    draw_game_overlay();
}


int main() {
    SetConfigFlags(FLAG_FULLSCREEN_MODE);
    InitWindow(2560, 1600, "Platformer");
    SetTargetFPS(60);
    SetExitKey(KEY_NULL);

    load_fonts();
    load_images();
    load_sounds();
    init_all_levels();
    load_level(0);

    while (!WindowShouldClose()) {
        UpdateMusicStream(background_music);
        BeginDrawing();
        ClearBackground(BLACK);
        if (IsKeyPressed(KEY_Q)) {
            CloseWindow();
        }

        switch (game_state) {
            case MENU_STATE:
                draw_menu();
                if (IsKeyPressed(KEY_ENTER)) {
                    game_state = GAME_STATE;
                }
                break;

            case GAME_STATE:
                update_game();
                draw_game();
                if (IsKeyPressed(KEY_ESCAPE)) {
                    game_state = PAUSE_STATE;
                }
                break;

            case PAUSE_STATE:
                draw_pause_menu();
                if (IsKeyPressed(KEY_ESCAPE)) {
                    game_state = GAME_STATE;
                }
                break;

            case VICTORY_STATE:
                draw_victory_menu();
                if (IsKeyPressed(KEY_ENTER)) {
                    reset_all_levels();
                    player_score = 0;
                    level_index = 0;
                    player_current_hearts = PLAYER_MAX_HEARTS;
                    load_level(0);
                    game_state = MENU_STATE;
                }
                break;

            case GAME_OVER_STATE:
                draw_game_over_menu();
                if (IsKeyPressed(KEY_ENTER)) {
                    reset_all_levels();
                    player_score = 0;
                    level_index = 0;
                    player_current_hearts = PLAYER_MAX_HEARTS;
                    load_level(0);
                    game_state = MENU_STATE;
                }
                break;
        }

        EndDrawing();
    }

    unload_level();
    unload_all_levels();
    unload_sounds();
    unload_images();
    unload_fonts();
    CloseAudioDevice();
    CloseWindow();

    return 0;
}
