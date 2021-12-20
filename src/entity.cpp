#include "./entity.h"
#include "./world.h"

void entity::host(world &world, const entity_visitor &visitor) {
    visitor.visit_entity(world, *this);
}

double entity::get_x() const {
    return last_known_x;
}

void entity::set_x(double new_x) {
    last_known_x = new_x;
    owner->set_x_of(*this, new_x);
}

double entity::get_y() const {
    return last_known_y;
}

void entity::set_y(double new_y) {
    last_known_y = new_y;
    owner->set_y_of(*this, new_y);
}
