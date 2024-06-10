#include "./app.h"

#include <stdio.h>
#include <stdlib.h>

#include <raymath.h>

#define GRID_SIZE 18
#define CELL_SIZE 32

#define max(a, b) (((a) > (b)) ? (a) : (b))
#define min(a, b) (((a) < (b)) ? (a) : (b))

#define clamp(v, minv, maxv) (min(maxv, max(minv, v)))

App_State app_init_state(void) {
    App_State init_state = {0};
    return init_state;
}

void app_update_and_draw(App_State *state) {
    if (!Vector2Equals(state->target_player_pos, state->curr_player_pos)) {
        state->curr_player_pos = Vector2MoveTowards(
            state->curr_player_pos,
            state->target_player_pos,
            0.15);
    } else {
        Vector2 mv;
        mv.x = IsKeyDown(KEY_RIGHT) - IsKeyDown(KEY_LEFT);
        mv.y = IsKeyDown(KEY_DOWN) - IsKeyDown(KEY_UP);

        state->target_player_pos = Vector2Add(state->target_player_pos, mv);
        state->target_player_pos.x = clamp(state->target_player_pos.x, 0, GRID_SIZE - 1);
        state->target_player_pos.y = clamp(state->target_player_pos.y, 0, GRID_SIZE - 1);
    }

    DrawRectangle(32, 0, CELL_SIZE, CELL_SIZE, GREEN);
    DrawRectangle(96, 0, CELL_SIZE, CELL_SIZE, YELLOW);
    DrawRectangle(128, 0, CELL_SIZE, CELL_SIZE, MAGENTA);

    DrawRectangle(
        state->curr_player_pos.x * CELL_SIZE,
        state->curr_player_pos.y * CELL_SIZE,
        CELL_SIZE, CELL_SIZE, RED);

    DrawLine(1, 0, 1, CELL_SIZE*GRID_SIZE, RAYWHITE);
    for (size_t i = 0; i < GRID_SIZE + 1; ++i) {
        DrawLine(0, CELL_SIZE*i, CELL_SIZE*GRID_SIZE, CELL_SIZE*i, RAYWHITE);
        DrawLine(CELL_SIZE*i, 0, CELL_SIZE*i, CELL_SIZE*GRID_SIZE, RAYWHITE);
    }
}

const App_Export_Table export_table = {
    app_init_state,
    app_update_and_draw,
};
