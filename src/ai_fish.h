#ifndef FISH_SIMULATOR_AI_FISH_H
#define FISH_SIMULATOR_AI_FISH_H

#include <typeinfo>

#include "./fish.h"
#include "./player_fish.h"

class ai_fish;

class ai_fish_visitor : public virtual fish_visitor {
public:
    virtual void visit_ai_fish(world& world, ai_fish& ai_fish) const = 0;
};

class ai_fish_interactions final : public ai_fish_visitor, public player_fish_visitor {
private:
    ai_fish& self;

public:
    explicit ai_fish_interactions(ai_fish& self) : self(self) {}

    void visit_entity(world& world, entity& entity) const override;
    void visit_fish(world& world, fish& fish) const override;
    void visit_ai_fish(world& world, ai_fish& ai_fish) const override;
    void visit_player_fish(world& world, player_fish& player_fish) const override;
};

class ai_fish_boiding final : public player_fish_visitor {
private:
    ai_fish& self;

public:
    player_fish** player;
    std::vector<fish*>* other_fish;

public:
    explicit ai_fish_boiding(ai_fish& self, player_fish** player, std::vector<fish*>* other_fish) :
            self(self),
            player(player),
            other_fish(other_fish) {}

    void visit_entity(world& world, entity& entity) const override;
    void visit_fish(world& world, fish& fish) const override;
    void visit_player_fish(world& world, player_fish& player_fish) const override;
};

class ai_fish final : public fish {
private:
    ai_fish_interactions self;

public:
    double excitement;

public:
    ai_fish(double heading, Color color) :
            fish(color, heading),
            self(*this),
            excitement(0.0) {}
    ~ai_fish() override = default;

private:
    void adjust_current_velocity();
    void adjust_heading(world& world);
    void move();

public:
    void update(world& world) override;

    void host(world& world, const entity_visitor& visitor) override;
    const ai_fish_visitor& interactions() override { return self; };
};

#endif //FISH_SIMULATOR_AI_FISH_H
