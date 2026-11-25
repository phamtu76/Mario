#include "Koopa.h"

Koopa::Koopa(float x, float y)
    : Enemy(x, y, WIDTH, HEIGHT, WALK_SPEED, "assets/koopa.png", sf::Color(40, 170, 60)),
      kickCooldown(0.f)
{
    velocity.x = direction * speed;
}

void Koopa::update(float dt) {
    if (kickCooldown > 0.f) kickCooldown -= dt;

    switch (state) {
    case EnemyState::Walking:
        velocity.x = direction * WALK_SPEED;
        applyPhysics(dt);
        break;

    case EnemyState::ShellIdle:
        velocity.x = 0.f;
        applyPhysics(dt);
        break;

    case EnemyState::ShellMoving:
        velocity.x = direction * SHELL_SPEED;
        applyPhysics(dt);
        break;

    case EnemyState::Dead:
        applyPhysics(dt);
        if (position.y > 2000.f) removable = true;
        break;

    default: break;
    }
}

void Koopa::draw(sf::RenderWindow& window) {
    Enemy::draw(window);
    // Mai đang trượt: viền sáng để dễ nhận biết khi chưa có sprite riêng
    if (!hasTexture && state == EnemyState::ShellMoving) {
        fallbackShape.setOutlineThickness(3.f);
        fallbackShape.setOutlineColor(sf::Color::Yellow);
    } else {
        fallbackShape.setOutlineThickness(0.f);
    }
}

void Koopa::onStomped() {
    switch (state) {
    case EnemyState::Walking: {                 // thụt vào mai
        state = EnemyState::ShellIdle;
        velocity.x = 0.f;
        float diff = height - SHELL_HEIGHT;
        position.y += diff;
        height = SHELL_HEIGHT;
        fallbackShape.setSize({width, height});
        if (hasTexture) { sf::Vector2f s = sprite.getScale(); sprite.setScale(s.x, s.y * SHELL_HEIGHT / HEIGHT); }
        fallbackShape.setFillColor(sf::Color(20, 110, 40));
        break;
    }
    case EnemyState::ShellMoving:               // giẫm lên mai đang trượt -> dừng lại
        state = EnemyState::ShellIdle;
        velocity.x = 0.f;
        break;
    case EnemyState::ShellIdle:                 // giẫm lên mai đứng yên -> đá đi
        kickShell(direction);
        break;
    default: break;
    }
}

void Koopa::onTouchedBySide(float playerCenterX) {
    if (state != EnemyState::ShellIdle || kickCooldown > 0.f) return;
    float myCenter = position.x + width / 2.f;
    kickShell(playerCenterX < myCenter ? +1 : -1);   // đá ra xa phía Mario
}

void Koopa::kickShell(int dir) {
    state = EnemyState::ShellMoving;
    direction = dir;
    velocity.x = dir * SHELL_SPEED;
    kickCooldown = KICK_COOLDOWN;
}

bool Koopa::canHurtPlayer() const {
    if (state == EnemyState::Walking) return true;
    if (state == EnemyState::ShellMoving) return kickCooldown <= 0.f;
    return false;   // mai đứng yên không gây sát thương
}
