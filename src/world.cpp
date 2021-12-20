#include "./world.h"

void update_visitor::visit_entity(world& world, entity &entity) const {
    host_visitor hv(entity);
    world.visit_entities_within(world, entity.get_x(), entity.get_y(), 1000.0, hv);
    entity.update(world);
}

void host_visitor::visit_entity(world& world, entity& entity) const {
    if(std::addressof(host) != std::addressof(entity))
        host.host(world, entity.interactions());
}

void draw_visitor::visit_entity(world& world, entity& entity) const {
    entity.draw(camera);
}

void draw_ui_visitor::visit_entity(world& world, entity& entity) const {
    entity.draw_ui();
}
