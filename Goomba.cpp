#include "Goomba.h"

Goomba::Goomba(float x, float y)
    : Enemy(x, y, WIDTH, HEIGHT, SPEED, "assets/goomba.png", sf::Color(150, 90, 40))
{
    velocity.x = direction * speed;
}

void Goomba::update(float dt) {
    switch (state) {
    case EnemyState::Walking:
        velocity.x = direction * speed;
        applyPhysics(dt);
        break;

    case EnemyState::Stomped:            // đứng yên, đợi rồi biến mất
        stateTimer += dt;
        if (stateTimer >= FLATTEN_TIME) removable = true;
        break;

    case EnemyState::Dead:               // bị mai rùa hất văng
        applyPhysics(dt);
        if (position.y > 2000.f) removable = true;
        break;

    default: break;
    }
}

void Goomba::onStomped() {
    if (state != EnemyState::Walking) return;
    state = EnemyState::Stomped;
    velocity = {0.f, 0.f};
    stateTimer = 0.f;
    // Bẹp xuống: giảm chiều cao, giữ nguyên đáy
    float newHeight = HEIGHT / 3.f;
    position.y += height - newHeight;
    height = newHeight;
    fallbackShape.setSize({width, height});
    if (hasTexture) {
        sf::Vector2f s = sprite.getScale();
        sprite.setScale(s.x, s.y / 3.f);
    }
}
