#ifndef FISH_SIMULATOR_WORLD_H
#define FISH_SIMULATOR_WORLD_H

#include <memory>
#include <vector>

#include "raylib.h"

#include "./entity.h"
#include "./entity_quad_map.h"

class update_visitor final : public entity_visitor {
public:
    void visit_entity(world& world, entity& entity) const override;
};

class host_visitor final : public entity_visitor {
private:
    entity& host;

public:
    explicit host_visitor(entity& host) : host(host) {}

    void visit_entity(world& world, entity& entity) const override;
};

class draw_visitor final : public entity_visitor {
private:
    Camera2D& camera;

public:
    explicit draw_visitor(Camera2D& camera) : camera(camera) {}
    void visit_entity(world& world, entity& entity) const override;
};

class draw_ui_visitor final : public entity_visitor {
public:
    void visit_entity(world& world, entity& entity) const override;
};

class world final {
private:
    Camera2D camera;
    entity_quad_map entities;

public:
    world() : camera({{}, {}, 0.0, 1.0}), entities() {}

    void add(std::unique_ptr<entity> entity, double x, double y) {
        entities.add(std::move(entity), x, y);
    }

    void update() {
        update_visitor v;
        entities.visit_within(*this, -camera.offset.x, -camera.offset.y, 1000.0, v);
    }
    void draw() {
        BeginMode2D(camera);
        DrawText("Scroll to change speed", 280, 200, 20, LIGHTGRAY);
        draw_visitor dv(camera);
        entities.visit_within(*this, -camera.offset.x, -camera.offset.y, 1000.0, dv);
        EndMode2D();
        draw_ui_visitor uv;
        entities.visit_within(*this, -camera.offset.x, -camera.offset.y, 1000.0, uv);
    }

    void offset_camera(double x, double y) {
        camera.offset.x = (float) x;
        camera.offset.y = (float) y;
    }

    void visit_entities_within(world& world, double x, double y, double distance, entity_visitor& visitor) {
        entities.visit_within(world, x, y, distance, visitor);
    }

public:
    [[nodiscard]] const Camera2D& get_camera() const { return camera; }
};

#endif //FISH_SIMULATOR_WORLD_H
