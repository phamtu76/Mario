#pragma once

#include <SFML/Graphics.hpp>
#include "Mario.h"
#include "Map.h"
#include "UI.h"

class Game
{
public:
    Game();
    void run();

private:
    void processEvents();
    void update(float deltaTime);
    void render();
    void startGame();
    void restartGame();
    void updateCamera();
    void handleMarioDeath();

    sf::RenderWindow window_;
    sf::View camera_;
    sf::Clock clock_;

    Mario mario_;
    Map map_;
    UI ui_;

    GameState state_{GameState::Menu};
    int score_{0};
};
