#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>
#include <assert.h>
#include <time.h>

#include <dlfcn.h>
#include <sys/stat.h>
#include <sys/file.h>

#include <raylib.h>

#include "./app.h"

#define APP_DLL_PATH "./build/app.so"

typedef struct {
    void *handle;
    time_t last_modified;
    App_Export_Table *export_table;
    App_State state;
} App;

bool app_should_reload(App *app) {
    struct stat dll_stat = {0};
    int err = stat(APP_DLL_PATH, &dll_stat);
    if (err != 0) {
        fprintf(
            stderr, "Error: could not stat '%s': %s\n",
            APP_DLL_PATH, strerror(errno));
        exit(1);
    }

    time_t last_modified = dll_stat.st_mtime;
    return last_modified != app->last_modified;
}

void app_reload(App *app) {
    if (app->handle != NULL) {
        dlclose(app->handle);
    }

    app->handle = dlopen(APP_DLL_PATH, RTLD_NOW);
    if (app->handle == NULL) {
        fprintf(stderr, "Error: %s\n", dlerror());
        exit(1);
    }

    app->export_table = dlsym(app->handle, "export_table");
    if (app->export_table == NULL) {
        fprintf(stderr, "Error: %s\n", dlerror());
        exit(1);
    }

    struct stat dll_stat = {0};
    int err = stat(APP_DLL_PATH, &dll_stat);
    if (err != 0) {
        fprintf(
            stderr, "Error: could not stat '%s': %s\n",
            APP_DLL_PATH, strerror(errno));
        exit(1);
    }
    app->last_modified = app->last_modified;
}

int main(void) {
    SetTraceLogLevel(LOG_WARNING);
    InitWindow(800, 640, "Hot Reloading");
    SetTargetFPS(60);

    App app = {0};
    app_reload(&app);
    app.state = app.export_table->app_init_state();

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);

        if (app_should_reload(&app)) {
            app_reload(&app);
        }
        app.export_table->app_update_and_draw(&app.state);

        EndDrawing();
    }

    dlclose(app.handle);
    CloseWindow();
    return 0;
}
