#pragma once
#include <SFML/Graphics.hpp>
#include <string>

enum class ItemType { Coin, Mushroom };

// ====================== LỚP TRỪU TƯỢNG Item ======================
// Coin và Mushroom kế thừa. Game chỉ cần làm việc với Item* (đa hình).
class Item {
public:
    Item(float x, float y, float width, float height,
         const std::string& texturePath, sf::Color fallbackColor);
    virtual ~Item() = default;

    virtual void update(float dt) = 0;
    virtual void draw(sf::RenderWindow& window);
    virtual ItemType getType() const = 0;
    virtual int  getScore() const = 0;       // điểm cộng khi nhặt
    virtual void onCollected();              // gọi khi Mario nhặt (mặc định: đánh dấu đã nhặt)

    sf::FloatRect getBounds() const { return sf::FloatRect(position.x, position.y, width, height); }
    sf::Vector2f  getPosition() const { return position; }
    void setPosition(float x, float y) { position = {x, y}; }
    bool isCollected() const { return collected; }
    bool isRemovable() const { return collected || removable; }  // Game xóa khỏi danh sách khi true

protected:
    void updateSpritePosition();

    sf::Vector2f position;
    float width, height;
    bool  collected;
    bool  removable;
    sf::Sprite sprite;
    sf::RectangleShape fallbackShape;
    bool hasTexture;
};
