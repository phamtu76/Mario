#include "Coin.h"
#include <cmath>

Coin::Coin(float x, float y)
    : Item(x, y, SIZE, SIZE, "assets/coin.png", sf::Color(255, 215, 0)), baseY(y), time(0.f) {}

void Coin::update(float dt) {
    time += dt;
    position.y = baseY + std::sin(time * 4.f) * 4.f;   // nhấp nhô lên xuống
}

void Coin::draw(sf::RenderWindow& window) {
    Item::draw(window);
}
