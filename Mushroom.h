#pragma once
#include "Item.h"

// Mushroom: trồi ra từ khối gạch, sau đó đi bộ; Mario ăn -> lớn lên (power-up), +1000 điểm
class Mushroom : public Item {
public:
    Mushroom(float x, float y);
    void update(float dt) override;
    ItemType getType() const override { return ItemType::Mushroom; }
    int getScore() const override { return 1000; }

    // Dành cho Collision (thành viên 3)
    void setOnGround(bool value) { onGround = value; }
    void reverseDirection() { direction = -direction; }
    void setVelocity(float vx, float vy) { velocity = {vx, vy}; }
    sf::Vector2f getVelocity() const { return velocity; }
    void setGroundLevel(float y) { groundLevel = y; hasGroundLevel = true; }  // dùng khi test riêng
    bool isEmerging() const { return emerging; }   // đang trồi ra -> chưa nhặt được, chưa va chạm

private:
    static constexpr float SIZE = 36.f, SPEED = 80.f, GRAVITY = 1200.f;
    static constexpr float EMERGE_TIME = 0.6f;
    bool  emerging;
    float emergeTimer;
    float startY;
    int   direction;
    bool  onGround;
    sf::Vector2f velocity;
    float groundLevel; bool hasGroundLevel;
};
