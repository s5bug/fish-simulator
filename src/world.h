#ifndef FISH_SIMULATOR_WORLD_H
#define FISH_SIMULATOR_WORLD_H

#include <memory>
#include <vector>

#include "raylib.h"

class entity;
#include "./entity.h"

class world final {
private:
    Camera2D camera;
    std::vector<std::unique_ptr<entity>> entities;

public:
    world() : camera({{}, {}, 0.0, 1.0}), entities() {}

    void add(std::unique_ptr<entity> entity) {
        entities.push_back(std::move(entity));
    }

    void update() {
        for(auto i = entities.begin(); i < entities.end(); i++) {
            (*i)->update(*this);
        }
    }
    void draw() {
        BeginMode2D(camera);
        DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);
        for(auto i = entities.begin(); i < entities.end(); i++) {
            (*i)->draw(camera);
        }
        EndMode2D();
        for(auto i = entities.begin(); i < entities.end(); i++) {
            (*i)->draw_ui();
        }
    }

    void target_camera(double x, double y) {
        camera.offset.x = (float) x;
        camera.offset.y = (float) y;
    }

public:
    [[nodiscard]] const Camera2D& get_camera() const { return camera; }
};

#endif //FISH_SIMULATOR_WORLD_H
