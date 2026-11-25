#include "Enemy.h"
#include <map>
#include <memory>

namespace {
// Cache texture: mỗi file ảnh chỉ tải 1 lần dù có nhiều enemy
const sf::Texture* loadTexture(const std::string& path) {
    static std::map<std::string, std::unique_ptr<sf::Texture>> cache;
    auto it = cache.find(path);
    if (it != cache.end()) return it->second.get();
    auto tex = std::make_unique<sf::Texture>();
    if (!tex->loadFromFile(path)) { cache[path] = nullptr; return nullptr; }
    const sf::Texture* ptr = tex.get();
    cache[path] = std::move(tex);
    return ptr;
}
}

Enemy::Enemy(float x, float y, float w, float h, float spd,
             const std::string& texturePath, sf::Color fallbackColor)
    : position(x, y), velocity(0.f, 0.f), width(w), height(h), speed(spd),
      direction(-1), onGround(false), removable(false), stateTimer(0.f),
      state(EnemyState::Walking), hasTexture(false),
      groundLevel(0.f), hasGroundLevel(false),
      patrolLeft(0.f), patrolRight(0.f), hasPatrol(false)
{
    const sf::Texture* tex = loadTexture(texturePath);
    if (tex) {
        hasTexture = true;
        sprite.setTexture(*tex);
        sf::Vector2u sz = tex->getSize();
        sprite.setScale(w / static_cast<float>(sz.x), h / static_cast<float>(sz.y));
    }
    fallbackShape.setSize({w, h});
    fallbackShape.setFillColor(fallbackColor);
    updateSpritePosition();
}

void Enemy::draw(sf::RenderWindow& window) {
    updateSpritePosition();
    if (hasTexture) window.draw(sprite);
    else            window.draw(fallbackShape);
}

void Enemy::updateSpritePosition() {
    sprite.setPosition(position);
    fallbackShape.setPosition(position);
}

sf::FloatRect Enemy::getBounds() const {
    return sf::FloatRect(position.x, position.y, width, height);
}

void Enemy::reverseDirection() {
    direction = -direction;
    velocity.x = direction * std::abs(velocity.x);
}

bool Enemy::canHurtPlayer() const { return state == EnemyState::Walking; }

void Enemy::onHitByShell() {
    // Mặc định: bị hất văng lên rồi rơi khỏi màn hình
    state = EnemyState::Dead;
    velocity = {0.f, -350.f};
    stateTimer = 0.f;
}

void Enemy::onTouchedBySide(float) { /* mặc định không làm gì */ }

void Enemy::applyPhysics(float dt) {
    velocity.y += GRAVITY * dt;
    position.x += velocity.x * dt;
    position.y += velocity.y * dt;

    // Giới hạn tuần tra (dùng khi chưa ghép Collision)
    if (hasPatrol && state != EnemyState::Dead) {
        if (position.x < patrolLeft)               { position.x = patrolLeft;           reverseDirection(); }
        if (position.x + width > patrolRight)      { position.x = patrolRight - width;  reverseDirection(); }
    }
    // Nền giả (dùng khi test riêng)
    if (hasGroundLevel && state != EnemyState::Dead && position.y + height >= groundLevel) {
        position.y = groundLevel - height;
        velocity.y = 0.f;
        onGround = true;
    }
}
