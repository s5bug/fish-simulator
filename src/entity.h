#ifndef FISH_SIMULATOR_ENTITY_H
#define FISH_SIMULATOR_ENTITY_H

#include "raylib.h"

class world; // "./world.h"

class entity_visitor;

class entity {
private:
    double x;
    double y;

public:
    entity(double x, double y) : x(x), y(y) {}
    virtual ~entity() = default;

    virtual void update(world& world) = 0;
    virtual void draw(Camera2D& camera) = 0;
    virtual void draw_ui() {};

    virtual void host(world& world, const entity_visitor& visitor);
    virtual const entity_visitor& interactions() = 0;

public:
    [[nodiscard]] double get_x() const { return x; }
    void set_x(double new_x) { x = new_x; }
    [[nodiscard]] double get_y() const { return y; }
    void set_y(double new_y) { y = new_y; }
};

class entity_visitor {
public:
    virtual void visit_entity(world& world, entity& entity) const = 0;
};

class entity_interactions : public virtual entity_visitor {
public:
    void visit_entity(world &world, entity &entity) const override {}
};

#endif //FISH_SIMULATOR_ENTITY_H
