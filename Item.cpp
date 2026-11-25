#include "Item.h"
#include <map>
#include <memory>

namespace {
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

Item::Item(float x, float y, float w, float h, const std::string& texturePath, sf::Color fallbackColor)
    : position(x, y), width(w), height(h), collected(false), removable(false), hasTexture(false)
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

void Item::updateSpritePosition() {
    sprite.setPosition(position);
    fallbackShape.setPosition(position);
}

void Item::draw(sf::RenderWindow& window) {
    updateSpritePosition();
    if (hasTexture) window.draw(sprite);
    else            window.draw(fallbackShape);
}

void Item::onCollected() { collected = true; }
