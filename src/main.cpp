#include "raylib.h"

#include "./world.h"
#include "./player_fish.h"
#include "./ai_fish.h"

int main() {
    int screenWidth = 800;
    int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "Fish Simulator");

    SetTargetFPS(60);

    world world;
    world.add(std::make_unique<player_fish>(0.0, 0.0, 0.0, RED, 15));

    for(int i = 5; i < 10; i++) {
        for(int j = 5; j < 10; j++) {
            world.add(std::make_unique<ai_fish>(i * 50.0, j * 50.0, 0.0, BLUE));
        }
    }

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
