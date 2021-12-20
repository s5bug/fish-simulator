#include "./ai_fish.h"

void ai_fish::host(world& world, const entity_visitor& visitor) {
    try {
        const auto& afv = dynamic_cast<const ai_fish_visitor&>(visitor);
        afv.visit_ai_fish(world, *this);
    } catch (std::bad_cast&) {
        fish::host(world, visitor);
    }
}

void ai_fish::adjust_current_velocity() {
    current_velocity += (excitement - current_velocity) / 10.0;
}

void ai_fish::adjust_heading(world& world) {
    player_fish* player;
    std::vector<fish*> other_fish;
    ai_fish_boiding boiding(*this, &player, &other_fish);

    world.visit_entities_within(world, get_x(), get_y(), 50.0, boiding);
}

void ai_fish::move() {
    double moveX = current_velocity * std::sin(heading);
    double moveY = current_velocity * -std::cos(heading);

    set_x(get_x() + moveX);
    set_y(get_y() + moveY);
}

void ai_fish::update(world& world) {
    adjust_current_velocity();
    adjust_heading(world);
    move();

    excitement = (excitement * 9.0) / 10.0;
}

void ai_fish_interactions::visit_entity(world& world, entity& entity) const {

}

void ai_fish_interactions::visit_fish(world& world, fish& fish) const {

}

void ai_fish_interactions::visit_ai_fish(world& world, ai_fish& ai_fish) const {
    double distX = self.get_x() - ai_fish.get_x();
    double distY = self.get_y() - ai_fish.get_y();
    double distance = std::sqrt((distX * distX) + (distY * distY));
    if(distance != 0) self.excitement += (ai_fish.current_velocity - self.excitement) / distance;

    visit_fish(world, ai_fish);
}

void ai_fish_interactions::visit_player_fish(world& world, player_fish& player_fish) const {
    double distX = self.get_x() - player_fish.get_x();
    double distY = self.get_y() - player_fish.get_y();
    double distance = std::sqrt((distX * distX) + (distY * distY));
    self.excitement += (player_fish.current_velocity - self.excitement) / distance;

    visit_fish(world, player_fish);
}

void ai_fish_boiding::visit_entity(world& world, entity& entity) const {}

void ai_fish_boiding::visit_fish(world& world, fish& fish) const {
    other_fish->push_back(&fish);
}

void ai_fish_boiding::visit_player_fish(world& world, player_fish& player_fish) const {
    *player = &player_fish;
}
