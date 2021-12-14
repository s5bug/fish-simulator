#ifndef FINAL_ENTITY_H
#define FINAL_ENTITY_H

class world;
#include "./world.h"

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

public:
    [[nodiscard]] double get_x() const { return x; }
    void set_x(double new_x) { x = new_x; }
    [[nodiscard]] double get_y() const { return y; }
    void set_y(double new_y) { y = new_y; }
};

#endif //FINAL_ENTITY_H
