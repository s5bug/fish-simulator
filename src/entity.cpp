#include "./entity.h"

void entity::host(world &world, const entity_visitor &visitor) {
    visitor.visit_entity(world, *this);
}
