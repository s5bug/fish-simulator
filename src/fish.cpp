#include "./fish.h"

#include <typeinfo>

void fish::host(world& world, const entity_visitor& visitor) {
    try {
        const auto& fv = dynamic_cast<const fish_visitor&>(visitor);
        fv.visit_fish(world, *this);
    } catch (std::bad_cast&) {
        entity::host(world, visitor);
    }
}
