#include "./ai_fish.h"

void ai_fish::host(world& world, const entity_visitor& visitor) {
    try {
        const auto& afv = dynamic_cast<const ai_fish_visitor&>(visitor);
        afv.visit_ai_fish(world, *this);
    } catch (std::bad_cast&) {
        fish::host(world, visitor);
    }
}

void ai_fish::update(world& world) {

}

void ai_fish_interactions::visit_entity(world& world, entity& entity) const {

}

void ai_fish_interactions::visit_fish(world& world, fish& fish) const {

}

void ai_fish_interactions::visit_ai_fish(world& world, ai_fish& ai_fish) const {

}

void ai_fish_interactions::visit_player_fish(world& world, player_fish& player_fish) const {

}
