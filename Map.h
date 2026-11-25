#pragma once

#include <SFML/Graphics.hpp>
#include <vector>

class Map
{
public:
    Map();

    void reset();  // dựng lại màn chơi (coin, v.v.)
    void draw(sf::RenderWindow& window) const;

    const std::vector<sf::FloatRect>& getSolids() const;
    sf::Vector2f getSpawnPoint() const;
    float getWorldWidth() const;
    float getWorldHeight() const;
    bool collectCoin(const sf::FloatRect& playerBounds);
    bool isGoalReached(const sf::FloatRect& playerBounds) const;

private:
    void buildLevel();

    // Dùng cho va chạm.
    std::vector<sf::FloatRect> solids_;

    // Dùng cho vẽ.
    std::vector<sf::FloatRect> floors_;
    std::vector<sf::FloatRect> platforms_;
    std::vector<sf::FloatRect> pipes_;
    std::vector<sf::Vector2f> coins_;

    sf::RectangleShape flagPole_;
    sf::ConvexShape flag_;
    sf::RectangleShape background_;

    sf::Vector2f spawnPoint_{100.f, 390.f};
    float worldWidth_{1920.f};
    float worldHeight_{544.f};
};
