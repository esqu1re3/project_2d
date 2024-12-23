#ifndef PLAYER_H
#define PLAYER_H

#include "globals.h"

float spawn_x = 0.0f;
float spawn_y = 0.0f;

void spawn_player() {
    for (size_t row = 0; row < current_level.rows; ++row) {
        for (size_t column = 0; column < current_level.columns; ++column) {
            char cell = current_level.data[row * current_level.columns + column];
            if (cell == PLAYER) {
                spawn_x = static_cast<float>(column);
                spawn_y = static_cast<float>(row);
                player_pos.x = static_cast<float>(column);
                player_pos.y = static_cast<float>(row);
                return;
            }
        }
    }
}

void respawn_player() {
    player_pos.x = spawn_x;
    player_pos.y = spawn_y;
    player_y_velocity = 0.0f;
}

void move_player_horizontally(float delta) {
    float next_x = player_pos.x + delta;
    if (!is_colliding({next_x, player_pos.y}, WALL)) {
        player_pos.x = next_x;
    } else {
        player_pos.x = roundf(player_pos.x);
    }
}

void update_player() {
    player_pos.y += player_y_velocity;
    player_y_velocity += GRAVITY_FORCE;

    is_player_on_ground = is_colliding({ player_pos.x, player_pos.y + 0.1f }, WALL);
    if (is_player_on_ground) {
        player_y_velocity = 0;
        player_pos.y = roundf(player_pos.y);
    }

    if (is_colliding(player_pos, COIN)) {
        get_collider(player_pos, COIN) = ' ';
        player_score += 10;
        PlaySound(coin_sound);
    }
    if (is_colliding(player_pos, EXIT)) {
        load_level(1);
        PlaySound(next_level_sound);
    }

    if (is_colliding(player_pos, BOMB)) {
        if (player_current_hearts == 1) {
            create_victory_menu_background();
            game_state = GAME_OVER_STATE;
            PlaySound(bomb_sound);
            PlaySound(game_over_sound);
            player_current_hearts = PLAYER_MAX_HEARTS;
        } else {
            player_current_hearts--;
            respawn_player();
            PlaySound(bomb_sound);
        }
    }
    if (is_colliding(player_pos, HEART)) {
        get_collider(player_pos, HEART) = AIR;

        if (player_current_hearts < PLAYER_MAX_HEARTS) {
            player_current_hearts++;
        }
        PlaySound(heart_pickup_sound);
    }
    if (is_colliding(player_pos, RERUN)) {
        level_index = 0;
        load_level(0);
        PlaySound(rerun_sound);
    }
}

#endif // PLAYER_H
