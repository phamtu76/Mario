#pragma once

#include <SFML/Graphics.hpp>
#include <string>

enum class GameState
{
    Menu,
    Playing,
    Paused,
    GameOver,
    Win
};

class UI
{
public:
    UI();

    void draw(sf::RenderWindow& window, GameState state,
              int score, int lives) const;

private:
    sf::Font font_;
    bool fontLoaded_{false};

    void drawText(sf::RenderWindow& window, const std::string& text,
                  unsigned int size, float x, float y,
                  sf::Color color = sf::Color::White) const;
    void drawCentered(sf::RenderWindow& window, const std::string& text,
                      unsigned int size, float y,
                      sf::Color color = sf::Color::White) const;
};
