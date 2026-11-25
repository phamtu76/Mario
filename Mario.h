#pragma once

#include <SFML/Graphics.hpp>

class Mario
{
public:
    Mario();

    void reset(const sf::Vector2f& spawn);
    void handleEvent(const sf::Event& event);
    void update(float deltaTime);
    void draw(sf::RenderWindow& window) const;

    sf::FloatRect getBounds() const;
    sf::Vector2f getPosition() const;
    sf::Vector2f getVelocity() const;
    void setPosition(const sf::Vector2f& position);
    void setVelocity(const sf::Vector2f& velocity);
    void setOnGround(bool value);
    bool isOnGround() const;
    bool isDead() const;
    void kill();

    int getLives() const;
    void loseLife();
    void addLife();
    void setInvincible(float seconds);

private:
    void updateAnimation();
    void updateVisual();

    sf::RectangleShape body_;
    sf::RectangleShape hat_;
    sf::RectangleShape face_;
    sf::Vector2f velocity_{0.f, 0.f};

    bool moveLeft_{false};
    bool moveRight_{false};
    bool jumpRequested_{false};
    bool jumpReleased_{false};
    bool onGround_{false};
    bool dead_{false};
    bool facingRight_{true};

    float moveSpeed_{230.f};
    float jumpSpeed_{-560.f};
    float gravity_{1450.f};
    float animationTimer_{0.f};
    float invincibleTimer_{0.f};
    float coyoteTimer_{0.f};      // cho phép nhảy ngay sau khi rời mép
    float jumpBufferTimer_{0.f};  // nhớ phím nhảy bấm sớm trước khi chạm đất

    int lives_{3};
};
