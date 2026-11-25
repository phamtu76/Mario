#pragma once
#include "Enemy.h"

// Koopa: đi bộ -> bị giẫm thì thụt vào mai -> bị đá thì mai trượt đi và hạ gục enemy khác
class Koopa : public Enemy {
public:
    Koopa(float x, float y);
    void update(float dt) override;
    void draw(sf::RenderWindow& window) override;
    void onStomped() override;
    void onTouchedBySide(float playerCenterX) override;   // đá mai
    bool canHurtPlayer() const override;
    int  getScore() const override { return 200; }

    bool isShellMoving() const { return state == EnemyState::ShellMoving; }

private:
    void kickShell(int dir);
    static constexpr float WIDTH = 40.f, HEIGHT = 56.f, SHELL_HEIGHT = 36.f;
    static constexpr float WALK_SPEED = 50.f, SHELL_SPEED = 350.f;
    static constexpr float KICK_COOLDOWN = 0.25f;  // tránh vừa đá đã tự trúng Mario
    float kickCooldown;
};
