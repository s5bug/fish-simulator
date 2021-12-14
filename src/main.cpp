#include "raylib.h"

#include "./world.h"
#include "./fish.h"

int main() {
    int screenWidth = 800;
    int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "Fish Simulator");

    SetTargetFPS(60);

    world world;
    world.add(std::make_unique<player_fish>(0.0, 0.0, 0.0, RED, 15));

    while(!WindowShouldClose()) {
        // Update
        world.update();

        // Draw
        BeginDrawing();

        ClearBackground(RAYWHITE);

        world.draw();

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
