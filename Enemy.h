#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <cmath>

// Trạng thái chung của kẻ địch
enum class EnemyState { Walking, Stomped, ShellIdle, ShellMoving, Dead };

// ====================== LỚP TRỪU TƯỢNG Enemy ======================
// - Trừu tượng : có hàm thuần ảo update(), onStomped(), getScore() -> không tạo trực tiếp được Enemy
// - Đóng gói   : dữ liệu để protected/private, truy cập qua getter/setter
// - Đa hình    : Goomba, Koopa ghi đè update(), draw(), onStomped()...
class Enemy {
public:
    Enemy(float x, float y, float width, float height, float speed,
          const std::string& texturePath, sf::Color fallbackColor);
    virtual ~Enemy() = default;

    // ---- Hành vi ảo (lớp con ghi đè) ----
    virtual void update(float dt) = 0;                 // cập nhật di chuyển / trạng thái
    virtual void draw(sf::RenderWindow& window);       // vẽ enemy
    virtual void onStomped() = 0;                      // bị Mario giẫm lên đầu
    virtual void onHitByShell();                       // bị mai rùa đang trượt đụng trúng
    virtual void onTouchedBySide(float playerCenterX); // Mario chạm từ bên hông (Koopa dùng để đá mai)
    virtual bool canHurtPlayer() const;                // chạm vào Mario có gây sát thương không
    virtual int  getScore() const = 0;                 // điểm khi bị tiêu diệt

    // ---- Dành cho Game/Collision (thành viên 3) ----
    sf::FloatRect getBounds() const;
    sf::Vector2f  getPosition() const { return position; }
    sf::Vector2f  getVelocity() const { return velocity; }
    void setPosition(float x, float y) { position = {x, y}; }
    void setVelocity(float vx, float vy) { velocity = {vx, vy}; }
    void setOnGround(bool value) { onGround = value; }   // Collision gọi khi enemy đứng trên nền
    void reverseDirection();                              // Collision gọi khi enemy đụng tường/ống
    void setGroundLevel(float y) { groundLevel = y; hasGroundLevel = true; } // dùng khi test riêng
    void setPatrolRange(float left, float right) { patrolLeft = left; patrolRight = right; hasPatrol = true; }

    EnemyState getState() const { return state; }
    int  getDirection() const { return direction; }
    bool isAlive() const { return state != EnemyState::Dead && state != EnemyState::Stomped; }
    bool isRemovable() const { return removable; }       // Game xóa enemy khỏi danh sách khi true

protected:
    void applyPhysics(float dt);        // trọng lực + di chuyển + giới hạn tuần tra
    void updateSpritePosition();

    // dữ liệu dùng chung cho lớp con
    sf::Vector2f position;     // góc trên-trái
    sf::Vector2f velocity;
    float width, height;
    float speed;
    int   direction;           // -1: trái, +1: phải
    bool  onGround;
    bool  removable;
    float stateTimer;          // đếm thời gian cho các trạng thái tạm thời
    EnemyState state;

    sf::Sprite    sprite;
    sf::RectangleShape fallbackShape;   // vẽ khi không tải được ảnh
    bool hasTexture;

    static constexpr float GRAVITY = 1200.f;

private:
    float groundLevel;  bool hasGroundLevel;
    float patrolLeft, patrolRight; bool hasPatrol;
};
