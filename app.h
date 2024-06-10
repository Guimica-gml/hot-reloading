#ifndef APP_H_
#define APP_H_

#include <raylib.h>

typedef struct {
    Vector2 curr_player_pos;
    Vector2 target_player_pos;
} App_State;

typedef struct {
    App_State (*app_init_state)(void);
    void (*app_update_and_draw)(App_State *state);
} App_Export_Table;

#endif // APP_H_
