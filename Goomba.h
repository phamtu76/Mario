#pragma once
#include "Enemy.h"

// Goomba: đi bộ qua lại, bị giẫm 1 phát là chết (bẹp rồi biến mất)
class Goomba : public Enemy {
public:
    Goomba(float x, float y);
    void update(float dt) override;
    void onStomped() override;
    int  getScore() const override { return 100; }
private:
    static constexpr float WIDTH = 40.f, HEIGHT = 40.f, SPEED = 60.f;
    static constexpr float FLATTEN_TIME = 0.5f;   // thời gian hiện xác bẹp
};
