#include "./player_fish.h"

void player_fish::host(world& world, const entity_visitor& visitor) {
    try {
        const auto& pfv = dynamic_cast<const player_fish_visitor&>(visitor);
        pfv.visit_player_fish(world, *this);
    } catch (std::bad_cast&) {
        fish::host(world, visitor);
    }
}

void player_fish::change_speed() {
    double mouseWheelMovement = GetMouseWheelMove();
    if(mouseWheelMovement < 0.0) {
        speed_selection -= 1;
        if(speed_selection < 1) speed_selection = 1;
    } else if(mouseWheelMovement > 0.0) {
        speed_selection += 1;
        if(speed_selection > max_speed) speed_selection = max_speed;
    }
}

void player_fish::update_velocity(double mouseX, double mouseY) {
    double targetSpeed = std::sqrt((double) speed_selection);

    if(IsMouseButtonDown(MOUSE_RIGHT_BUTTON) || !IsCursorOnScreen()) {
        targetSpeed = 0;
    }

    double targetPxS = (targetSpeed * 60.0) / 50.0;

    double diffX = mouseX - get_x();
    double diffY = mouseY - get_y();
    double distance = std::sqrt((diffX * diffX) + (diffY * diffY));

    double targetHeading = std::atan2(diffX, -diffY);

    double targetVelocity;
    if(distance >= 20.0) {
        targetVelocity = targetPxS;
    } else if(distance >= 3.0) {
        targetVelocity = targetPxS * ((distance - 3.0) / 17.0);
    } else {
        targetVelocity = 0;
    }

    current_velocity += (targetVelocity - current_velocity) / 10.0;

    if(std::abs(heading - targetHeading) < (5.0 * PI / 180.0)) {
        heading = targetHeading;
    } else {
        if(heading < targetHeading) {
            if(std::abs(heading - targetHeading) <= PI) heading += (5.0 * PI / 180.0);
            else heading -= (5.0 * PI / 180.0);
        } else {
            if(std::abs(heading - targetHeading) <= PI) heading -= (5.0 * PI / 180.0);
            else heading += (5.0 * PI / 180.0);
        }

        if(heading > PI) heading -= (2.0 * PI);
        else if(heading < -PI) heading += (2.0 * PI);
    }
}

void player_fish::move() {
    double moveX = current_velocity * std::sin(heading);
    double moveY = current_velocity * -std::cos(heading);

    set_x(get_x() + moveX);
    set_y(get_y() + moveY);
}

void player_fish::update(world &world) {
    world.target_camera(-get_x() + (GetScreenWidth() / 2.0), -get_y() + ((GetScreenHeight() - 16.0) / 2.0));

    Vector2 mouseWorldCoords = GetScreenToWorld2D(
            {static_cast<float>(GetMouseX()), static_cast<float>(GetMouseY())},
            world.get_camera()
    );

    change_speed();
    update_velocity(mouseWorldCoords.x, mouseWorldCoords.y);
    move();
}

void player_fish::draw_ui() {
    double speedbarHeight = (GetScreenHeight() * 4.0) / 5.0;
    double speedbarWidth = 8.0;

    double speedbarX = GetScreenWidth() - (speedbarWidth * 2);
    double speedbarY = (GetScreenHeight() - speedbarHeight) / 2.0;

    double fillHeight = ((speedbarHeight * speed_selection) / max_speed);
    double yOffset = speedbarHeight - fillHeight;

    DrawRectangle(
            (int) speedbarX, (int) (speedbarY + yOffset),
            (int) speedbarWidth, (int) fillHeight,
            color
    );
    DrawRectangleLines(
            (int) speedbarX, (int) speedbarY,
            (int) speedbarWidth, (int) speedbarHeight,
            BLACK
    );
}

void player_fish_interactions::visit_entity(world &world, entity &entity) const {}
void player_fish_interactions::visit_fish(world &world, fish &fish) const {}
void player_fish_interactions::visit_player_fish(world &world, player_fish &player_fish) const {}
