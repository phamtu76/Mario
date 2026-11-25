#include "Mushroom.h"

Mushroom::Mushroom(float x, float y)
    : Item(x, y, SIZE, SIZE, "assets/mushroom.png", sf::Color(220, 40, 40)),
      emerging(true), emergeTimer(0.f), startY(y), direction(1), onGround(false),
      velocity(0.f, 0.f), groundLevel(0.f), hasGroundLevel(false) {}

void Mushroom::update(float dt) {
    if (emerging) {                       // trồi lên khỏi khối gạch
        emergeTimer += dt;
        position.y = startY - SIZE * (emergeTimer / EMERGE_TIME);
        if (emergeTimer >= EMERGE_TIME) {
            position.y = startY - SIZE;
            emerging = false;
        }
        return;
    }
    velocity.x = direction * SPEED;
    velocity.y += GRAVITY * dt;
    position.x += velocity.x * dt;
    position.y += velocity.y * dt;

    if (hasGroundLevel && position.y + height >= groundLevel) {
        position.y = groundLevel - height;
        velocity.y = 0.f;
        onGround = true;
    }
    if (position.y > 2000.f) removable = true;   // rơi khỏi map
}
