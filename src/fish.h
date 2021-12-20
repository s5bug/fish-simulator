#ifndef FISH_SIMULATOR_FISH_H
#define FISH_SIMULATOR_FISH_H

#include <cmath>

#include "raylib.h"

#include "./entity.h"

class fish;

class fish_visitor : public virtual entity_visitor {
public:
    virtual void visit_fish(world& world, fish& fish) const = 0;
};

class fish : public entity {
public:
    Color color;

    double heading;
    double current_velocity;

public:
    fish(double x, double y, Color color, double heading) :
            entity(x, y),
            color(color),
            heading(heading),
            current_velocity(0.0) {}
    ~fish() override = default;

    void draw(Camera2D& camera) override {
        DrawCircle((int) get_x(), (int) get_y(), 10.0, color);
    }

    void host(world& world, const entity_visitor& visitor) override;
    const fish_visitor& interactions() override = 0;
};

#endif //FISH_SIMULATOR_FISH_H
