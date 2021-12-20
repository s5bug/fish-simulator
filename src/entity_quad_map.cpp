#include "./entity_quad_map.h"

const double entity_quad_map::REGION_SIZE = 256.0;

void entity_quad_map::add(std::unique_ptr<entity> entity, double x, double y) {
    int region_x = std::floor(x / REGION_SIZE);
    int region_y = std::floor(y / REGION_SIZE);

    region r { region_x, region_y };

    double offsX = std::fmod((std::fmod(x, REGION_SIZE) + REGION_SIZE), REGION_SIZE);
    double offsY = std::fmod((std::fmod(y, REGION_SIZE) + REGION_SIZE), REGION_SIZE);

    if(map.find(r) == map.end())
        map[r] = std::make_unique<entity_quad_tree>(this, region_x * REGION_SIZE, region_y * REGION_SIZE, REGION_SIZE);

    map[r]->add(std::move(entity), offsX, offsY);
}

void entity_quad_map::visit_within(world &world, double x, double y, double distance, entity_visitor &visitor) {
    int center_region_x = std::floor(x / REGION_SIZE);
    int center_region_y = std::floor(y / REGION_SIZE);
    int region_distance = std::ceil(distance / REGION_SIZE);

    region r {};
    for(r.x = center_region_x - region_distance; r.x <= center_region_x + region_distance; r.x++) {
        for(r.y = center_region_y - region_distance; r.y <= center_region_y + region_distance; r.y++) {
            auto region_quad = map.find(r);
            if(region_quad != map.end()) {
                // TODO fix distance
                region_quad->second->visit_within(world, x, y, distance, visitor);
            }
        }
    }
}

void entity_quad_tree::add(std::unique_ptr<entity> entity, double xoffset, double yoffset) {
    switch(children.index()) {
        case 0: {
            std::vector<std::pair<std::unique_ptr<class entity>, entity_quad_offset>>& cv = std::get<0>(children);

            entity_quad_offset offs { xoffset, yoffset };

            entity->owner = this;
            entity->last_known_x = origin_x + xoffset;
            entity->last_known_y = origin_y + yoffset;
            cv.emplace_back(std::move(entity), offs);
        } break;
        case 1: {
            std::unique_ptr<std::array<entity_quad_tree, 4>>& ca = std::get<1>(children);

            // TODO implement this
        } break;
    }
    entity_count++;
}

void entity_quad_tree::visit_within(world &world, double xoffset, double yoffset, double distance, entity_visitor &visitor) {
    switch(children.index()) {
        case 0: { // vector of children
            std::vector<std::pair<std::unique_ptr<class entity>, entity_quad_offset>>& cv = std::get<0>(children);
            for (std::size_t i = 0; i < cv.size(); i++) {
                // TODO check distance
                cv[i].first->host(world, visitor);
            }
        } break;
        case 1: {
            std::unique_ptr<std::array<entity_quad_tree, 4>>& ca = std::get<1>(children);
            for (auto i = ca->begin(); i < ca->end(); i++) {
                // TODO fix xoffset and yoffset, check distance
                i->visit_within(world, xoffset, yoffset, distance, visitor);
            }
        } break;
    }
}

void entity_quad_tree::set_x_of(const entity& entity, double new_x) {
    switch(children.index()) {
        case 0: {
            std::vector<std::pair<std::unique_ptr<class entity>, entity_quad_offset>>& cv = std::get<0>(children);

            for(auto i = cv.begin(); i < cv.end(); i++) {
                if(std::addressof(entity) == i->first.get()) {
                    if((new_x / quad_size) != (origin_x / quad_size)) {
                        std::unique_ptr<class entity> ent = std::move(i->first);
                        double y = origin_y + i->second.y;
                        cv.erase(i);
                        map->add(std::move(ent), new_x, y);
                    } else {
                        i->second.x = origin_x - new_x;
                    }
                    return;
                }
            }

            throw std::exception("Entity does not exist anymore.");
        } break;
        case 1: {
            std::unique_ptr<std::array<entity_quad_tree, 4>>& ca = std::get<1>(children);

            // TODO implement this
        } break;
    }
}

void entity_quad_tree::set_y_of(const entity& entity, double new_y) {
    switch(children.index()) {
        case 0: {
            std::vector<std::pair<std::unique_ptr<class entity>, entity_quad_offset>>& cv = std::get<0>(children);

            for(auto i = cv.begin(); i < cv.end(); i++) {
                if(std::addressof(entity) == i->first.get()) {
                    if((new_y / quad_size) != (origin_y / quad_size)) {
                        std::unique_ptr<class entity> ent = std::move(i->first);
                        double x = origin_x + i->second.x;
                        cv.erase(i);
                        map->add(std::move(ent), x, new_y);
                    } else {
                        i->second.y = origin_y - new_y;
                    }
                    return;
                }
            }

            throw std::exception("Entity does not exist anymore.");
        } break;
        case 1: {
            std::unique_ptr<std::array<entity_quad_tree, 4>>& ca = std::get<1>(children);

            // TODO implement this
        } break;
    }
}
