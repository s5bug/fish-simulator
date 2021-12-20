#ifndef FISH_SIMULATOR_ENTITY_H
#define FISH_SIMULATOR_ENTITY_H

#include "raylib.h"

class entity_quad_tree; // "./entity_quad_map.h"
class world; // "./world.h"

class entity_visitor;

class entity {
private:
    double last_known_x;
    double last_known_y;
    entity_quad_tree* owner;
    friend entity_quad_tree;

public:
    entity() : owner(nullptr) {}
    virtual ~entity() = default;

    virtual void update(world& world) = 0;
    virtual void draw(Camera2D& camera) = 0;
    virtual void draw_ui() {};

    virtual void host(world& world, const entity_visitor& visitor);
    virtual const entity_visitor& interactions() = 0;

public:
    [[nodiscard]] double get_x() const;
    void set_x(double new_x);
    [[nodiscard]] double get_y() const;
    void set_y(double new_y);
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
