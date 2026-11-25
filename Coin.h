#pragma once
#include "Item.h"

// Coin: đứng yên, nhấp nhô nhẹ; nhặt được +100 điểm
class Coin : public Item {
public:
    Coin(float x, float y);
    void update(float dt) override;
    void draw(sf::RenderWindow& window) override;
    ItemType getType() const override { return ItemType::Coin; }
    int getScore() const override { return 100; }
private:
    static constexpr float SIZE = 28.f;
    float baseY;
    float time;
};
