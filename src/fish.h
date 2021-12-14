#ifndef FISH_SIMULATOR_FISH_H
#define FISH_SIMULATOR_FISH_H

#include <cmath>

#include "raylib.h"

#include "./entity.h"

class fish : public entity {
private:
    Color color;

public:
    fish(double x, double y, Color color) : entity(x, y), color(color) {}
    ~fish() override = default;

    void draw(Camera2D& camera) override {
        DrawCircle((int) get_x(), (int) get_y(), 10.0, get_color());
    }

public:
    [[nodiscard]] Color get_color() const { return color; }
    void set_color(Color new_color) { color = new_color; }
};

class player_fish final : public fish {
private:
    int max_speed;
    int speed_selection;

    double heading;
    double current_velocity;

public:
    player_fish(double x, double y, double heading, Color color, int max_speed) :
            fish(x, y, color),
            heading(heading),
            max_speed(max_speed),
            speed_selection(1),
            current_velocity(0.0) {}
    ~player_fish() override = default;

private:
    void change_speed() {
        double mouseWheelMovement = GetMouseWheelMove();
        if(mouseWheelMovement < 0.0) {
            speed_selection -= 1;
            if(speed_selection < 1) speed_selection = 1;
        } else if(mouseWheelMovement > 0.0) {
            speed_selection += 1;
            if(speed_selection > max_speed) speed_selection = max_speed;
        }
    }

    void update_velocity(double mouseX, double mouseY) {
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

    void move(double mouseX, double mouseY) {
        double moveX = current_velocity * std::sin(heading);
        double moveY = current_velocity * -std::cos(heading);

        set_x(get_x() + moveX);
        set_y(get_y() + moveY);
    }

public:
    void update(world& world) override {
        world.target_camera(-get_x() + (GetScreenWidth() / 2.0), -get_y() + ((GetScreenHeight() - 16.0) / 2.0));

        Vector2 mouseWorldCoords = GetScreenToWorld2D(
                {static_cast<float>(GetMouseX()), static_cast<float>(GetMouseY())},
                world.get_camera()
        );

        change_speed();
        update_velocity(mouseWorldCoords.x, mouseWorldCoords.y);
        move(mouseWorldCoords.x, mouseWorldCoords.y);
    }
    void draw_ui() override {
        double speedbarHeight = (GetScreenHeight() * 4.0) / 5.0;
        double speedbarWidth = 8.0;

        double speedbarX = GetScreenWidth() - (speedbarWidth * 2);
        double speedbarY = (GetScreenHeight() - speedbarHeight) / 2.0;

        double fillHeight = ((speedbarHeight * speed_selection) / max_speed);
        double yOffset = speedbarHeight - fillHeight;

        DrawRectangle(
                (int) speedbarX, (int) (speedbarY + yOffset),
                (int) speedbarWidth, (int) fillHeight,
                get_color()
        );
        DrawRectangleLines(
                (int) speedbarX, (int) speedbarY,
                (int) speedbarWidth, (int) speedbarHeight,
                BLACK
        );
    }
};

class ai_fish final : public fish {
public:
    ai_fish(double x, double y, Color color) : fish(x, y, color) {}
    ~ai_fish() override = default;

    void update(world& world) override {

    }
};

#endif //FISH_SIMULATOR_FISH_H
