#ifndef FISH_SIMULATOR_PLAYER_FISH_H
#define FISH_SIMULATOR_PLAYER_FISH_H

#include <typeinfo>

#include "./fish.h"
#include "./world.h"

class player_fish;

class player_fish_visitor : public virtual fish_visitor {
public:
    virtual void visit_player_fish(world& world, player_fish& player_fish) const = 0;
};

class player_fish_interactions final : public player_fish_visitor {
private:
    player_fish& self;

public:
    explicit player_fish_interactions(player_fish& self) : self(self) {}

    void visit_entity(world& world, entity& entity) const override;
    void visit_fish(world& world, fish& fish) const override;
    void visit_player_fish(world& world, player_fish& player_fish) const override;
};

class player_fish final : public fish {
private:
    int max_speed;
    int speed_selection;

public:
    player_fish(double x, double y, double heading, Color color, int max_speed) :
            fish(x, y, color, heading),
            max_speed(max_speed),
            speed_selection(1),
            self(*this) {}
    ~player_fish() override = default;

private:
    void change_speed();
    void update_velocity(double mouseX, double mouseY);
    void move();

public:
    void update(world& world) override;
    void draw_ui() override;

private:
    player_fish_interactions self;

public:
    void host(world& world, const entity_visitor& visitor) override;
    const player_fish_visitor& interactions() override { return self; };
};

#endif //FISH_SIMULATOR_PLAYER_FISH_H
