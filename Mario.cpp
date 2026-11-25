#include "Mario.h"
#include "Config.h"
#include <algorithm>
#include <cmath>

namespace
{
constexpr float COYOTE_TIME = 0.08f;
constexpr float JUMP_BUFFER_TIME = 0.10f;
constexpr float JUMP_CUT_FACTOR = 0.4f;   // nhả phím sớm -> nhảy thấp hơn
constexpr float MAX_FALL_SPEED = 900.f;
constexpr float MAX_STEP = 0.05f;

const sf::Color BODY_COLOR(220, 45, 45);
const sf::Color BODY_COLOR_ALT(245, 85, 45);

bool isLeftKey(sf::Keyboard::Key k)
{
    return k == sf::Keyboard::Left || k == sf::Keyboard::A;
}

bool isRightKey(sf::Keyboard::Key k)
{
    return k == sf::Keyboard::Right || k == sf::Keyboard::D;
}

bool isJumpKey(sf::Keyboard::Key k)
{
    return k == sf::Keyboard::Space || k == sf::Keyboard::Up ||
           k == sf::Keyboard::W;
}
}

Mario::Mario()
{
    body_.setSize({Config::PLAYER_WIDTH, Config::PLAYER_HEIGHT});
    body_.setOrigin(Config::PLAYER_WIDTH / 2.f, Config::PLAYER_HEIGHT / 2.f);
    body_.setFillColor(BODY_COLOR);
    body_.setOutlineThickness(2.f);
    body_.setOutlineColor(sf::Color(90, 25, 25));

    hat_.setSize({24.f, 8.f});
    hat_.setOrigin(12.f, 4.f);
    hat_.setFillColor(sf::Color(190, 25, 25));

    face_.setSize({12.f, 13.f});
    face_.setOrigin(6.f, 6.5f);
    face_.setFillColor(sf::Color(255, 205, 155));

    reset({100.f, 300.f});
}

void Mario::reset(const sf::Vector2f& spawn)
{
    // Giữ nguyên trạng thái phím đang giữ (moveLeft_/moveRight_) để hồi sinh
    // xong vẫn đi tiếp được nếu người chơi còn đang giữ phím.
    body_.setPosition(spawn);
    velocity_ = {0.f, 0.f};
    jumpRequested_ = jumpReleased_ = false;
    onGround_ = false;
    dead_ = false;
    animationTimer_ = 0.f;
    invincibleTimer_ = 0.f;
    coyoteTimer_ = 0.f;
    jumpBufferTimer_ = 0.f;
    updateVisual();
}

void Mario::handleEvent(const sf::Event& event)
{
    switch (event.type)
    {
        case sf::Event::KeyPressed:
            if (isLeftKey(event.key.code))
                moveLeft_ = true;
            if (isRightKey(event.key.code))
                moveRight_ = true;
            if (isJumpKey(event.key.code))
                jumpRequested_ = true;
            break;

        case sf::Event::KeyReleased:
            if (isLeftKey(event.key.code))
                moveLeft_ = false;
            if (isRightKey(event.key.code))
                moveRight_ = false;
            if (isJumpKey(event.key.code))
                jumpReleased_ = true;
            break;

        case sf::Event::LostFocus:
            // Tránh kẹt phím khi cửa sổ mất focus.
            moveLeft_ = moveRight_ = jumpRequested_ = false;
            break;

        default:
            break;
    }
}

void Mario::update(float deltaTime)
{
    if (dead_)
        return;

    deltaTime = std::min(deltaTime, MAX_STEP);

    if (invincibleTimer_ > 0.f)
        invincibleTimer_ = std::max(0.f, invincibleTimer_ - deltaTime);

    // Di chuyển ngang.
    velocity_.x = 0.f;
    if (moveLeft_)
        velocity_.x -= moveSpeed_;
    if (moveRight_)
        velocity_.x += moveSpeed_;
    if (velocity_.x > 0.f)
        facingRight_ = true;
    else if (velocity_.x < 0.f)
        facingRight_ = false;

    // Nhảy: coyote time + jump buffer.
    coyoteTimer_ = onGround_ ? COYOTE_TIME
                             : std::max(0.f, coyoteTimer_ - deltaTime);
    if (jumpRequested_)
    {
        jumpBufferTimer_ = JUMP_BUFFER_TIME;
        jumpRequested_ = false;
    }
    else
    {
        jumpBufferTimer_ = std::max(0.f, jumpBufferTimer_ - deltaTime);
    }

    if (jumpBufferTimer_ > 0.f && coyoteTimer_ > 0.f)
    {
        velocity_.y = jumpSpeed_;
        onGround_ = false;
        coyoteTimer_ = 0.f;
        jumpBufferTimer_ = 0.f;
    }

    // Nhả phím nhảy khi đang bay lên -> cắt ngắn cú nhảy.
    if (jumpReleased_)
    {
        if (velocity_.y < 0.f)
            velocity_.y = std::max(velocity_.y, jumpSpeed_ * JUMP_CUT_FACTOR);
        jumpReleased_ = false;
    }

    velocity_.y = std::min(velocity_.y + gravity_ * deltaTime, MAX_FALL_SPEED);
    body_.move(velocity_ * deltaTime);

    animationTimer_ += deltaTime;
    updateAnimation();
    updateVisual();
}

void Mario::updateAnimation()
{
    // Placeholder: đổi màu thân khi chạy trên đất.
    if (std::abs(velocity_.x) > 1.f && onGround_)
    {
        const bool frame = (static_cast<int>(animationTimer_ * 10.f) % 2) == 0;
        body_.setFillColor(frame ? BODY_COLOR : BODY_COLOR_ALT);
    }
    else
    {
        body_.setFillColor(BODY_COLOR);
    }
}

void Mario::updateVisual()
{
    const sf::Vector2f p = body_.getPosition();
    hat_.setPosition(p.x, p.y - 18.f);
    face_.setPosition(p.x + (facingRight_ ? 5.f : -5.f), p.y - 3.f);
}

void Mario::draw(sf::RenderWindow& window) const
{
    // Nhấp nháy khi đang bất tử.
    if (invincibleTimer_ > 0.f &&
        static_cast<int>(invincibleTimer_ * 12.f) % 2 == 0)
        return;

    window.draw(body_);
    window.draw(hat_);
    window.draw(face_);
}

sf::FloatRect Mario::getBounds() const { return body_.getGlobalBounds(); }
sf::Vector2f Mario::getPosition() const { return body_.getPosition(); }
sf::Vector2f Mario::getVelocity() const { return velocity_; }

void Mario::setPosition(const sf::Vector2f& position)
{
    body_.setPosition(position);
    updateVisual();
}

void Mario::setVelocity(const sf::Vector2f& velocity) { velocity_ = velocity; }
void Mario::setOnGround(bool value) { onGround_ = value; }
bool Mario::isOnGround() const { return onGround_; }
bool Mario::isDead() const { return dead_; }
void Mario::kill() { dead_ = true; velocity_ = {0.f, 0.f}; }
int Mario::getLives() const { return lives_; }
void Mario::loseLife() { if (lives_ > 0) --lives_; }
void Mario::addLife() { ++lives_; }
void Mario::setInvincible(float seconds) { invincibleTimer_ = seconds; }
