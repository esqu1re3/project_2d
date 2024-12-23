#ifndef LEVEL_H
#define LEVEL_H

#include "globals.h"

char* levels_runtime_data[LEVEL_COUNT] = {nullptr};

bool is_colliding(Vector2 pos, char look_for, level &level) {
    Rectangle player_hitbox = {pos.x, pos.y, 1.0f, 1.0f};

    for (size_t row = 0; row < level.rows; ++row) {
        for (size_t column = 0; column < level.columns; ++column) {
            if (level.data[row * level.columns + column] == look_for) {
                Rectangle block_hitbox = {(float) column, (float) row, 1.0f, 1.0f};
                if (CheckCollisionRecs(player_hitbox, block_hitbox)) {
                    return true;
                }
            }
        }
    }
    return false;
}

char& get_collider(Vector2 pos, char look_for, level &level) {
    Rectangle player_hitbox = {pos.x, pos.y, 1.0f, 1.0f};

    for (size_t row = 0; row < level.rows; ++row) {
        for (size_t column = 0; column < level.columns; ++column) {
            if (level.data[row * level.columns + column] == look_for) {
                Rectangle block_hitbox = {(float) column, (float) row, 1.0f, 1.0f};
                if (CheckCollisionRecs(player_hitbox, block_hitbox)) {
                    return level.data[row * level.columns + column];
                }
            }
        }
    }

    return level.data[static_cast<int>(roundf(pos.y) * level.columns + roundf(pos.x))];
}

void init_all_levels() {
    for (int i = 0; i < LEVEL_COUNT; i++) {
        int rows = LEVELS[i].rows;
        int columns = LEVELS[i].columns;
        levels_runtime_data[i] = new char[rows * columns];


        for (int r = 0; r < rows; r++) {
            for (int c = 0; c < columns; c++) {
                levels_runtime_data[i][r * columns + c] =
                        LEVELS[i].data[r * columns + c];
            }
        }
    }
}

void load_level(int offset) {
    level_index += offset;

    if (level_index >= LEVEL_COUNT) {
        if (player_score == 100) {
            PlaySound(you_win_sound);
            create_victory_menu_background();
            level_index = 0;
            game_state = VICTORY_STATE;
            load_level(level_index);
            return;
        } else {
            PlaySound(game_over_sound);
            create_victory_menu_background();
            level_index = 0;
            game_state = GAME_OVER_STATE;
            load_level(level_index);
            return;
        }
    }


    current_level_data = levels_runtime_data[level_index];

    size_t rows = LEVELS[level_index].rows;
    size_t columns = LEVELS[level_index].columns;
    current_level = { rows, columns, current_level_data };

    spawn_player();
    derive_graphics_metrics_from_loaded_level();
}

void reset_all_levels() {
    for (int i = 0; i < LEVEL_COUNT; i++) {
        int rows = LEVELS[i].rows;
        int columns = LEVELS[i].columns;

        for (int r = 0; r < rows; r++) {
            for (int c = 0; c < columns; c++) {
                levels_runtime_data[i][r * columns + c] =
                        LEVELS[i].data[r * columns + c];
            }
        }
    }
}


void unload_all_levels() {
    for (int i = 0; i < LEVEL_COUNT; i++) {
        if (levels_runtime_data[i]) {
            delete[] levels_runtime_data[i];
            levels_runtime_data[i] = nullptr;
        }
    }
}

void unload_level() {
    delete[] current_level_data;
}

#endif // LEVEL_H
