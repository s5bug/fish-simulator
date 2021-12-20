#ifndef FISH_SIMULATOR_ENTITY_QUAD_MAP_H
#define FISH_SIMULATOR_ENTITY_QUAD_MAP_H

#include <array>
#include <memory>
#include <unordered_map>
#include <variant>
#include <vector>

#include "./entity.h"

class entity_quad_map;

struct entity_quad_offset {
    double x;
    double y;
};

class entity_quad_tree {
private:
    int level;
    entity_quad_map* map;
    entity_quad_tree* parent;
    int entity_count;
    std::variant<
            std::vector<std::pair<std::unique_ptr<entity>, entity_quad_offset>>,
            std::unique_ptr<std::array<entity_quad_tree, 4>>
    > children;
    double origin_x;
    double origin_y;
    double quad_size;

public:
    entity_quad_tree(entity_quad_map* map, double origin_x, double origin_y, double quad_size) :
            map(map),
            origin_x(origin_x),
            origin_y(origin_y),
            quad_size(quad_size),
            level(0),
            parent(nullptr),
            entity_count(0),
            children(std::vector<std::pair<std::unique_ptr<entity>, entity_quad_offset>>()) {}

    entity_quad_tree(entity_quad_map* map, entity_quad_tree* parent, double origin_x, double origin_y, double quad_size) :
            map(map),
            origin_x(origin_x),
            origin_y(origin_y),
            quad_size(quad_size),
            level(parent->level + 1),
            parent(parent),
            entity_count(0),
            children(std::vector<std::pair<std::unique_ptr<entity>, entity_quad_offset>>()) {}

    void add(std::unique_ptr<entity> entity, double xoffset, double yoffset);

    void visit_within(world& world, double xoffset, double yoffset, double distance, entity_visitor& visitor);

    void set_x_of(const entity& entity, double new_x);
    void set_y_of(const entity& entity, double new_y);
};

struct region {
    int x;
    int y;

public:
    bool operator==(const region& other) const noexcept {
        return x == other.x && y == other.y;
    }
};

template<>
struct std::hash<region> {
    std::size_t operator()(region const& r) const noexcept {
        std::size_t hx = std::hash<int>{}(r.x);
        std::size_t hy = std::hash<int>{}(r.y);
        return hx + (hy * 31);
    }
};

class entity_quad_map {
public:
    const static double REGION_SIZE;

private:
    std::unordered_map<region, std::unique_ptr<entity_quad_tree>> map;

public:
    entity_quad_map() : map() {}

    void add(std::unique_ptr<entity> entity, double x, double y);

    void visit_within(world& world, double x, double y, double distance, entity_visitor& visitor);
};

#endif //FISH_SIMULATOR_ENTITY_QUAD_MAP_H
