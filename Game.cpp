#include "Game.h"
#include "Collision.h"
#include "Config.h"
#include <algorithm>

Game::Game()
    : window_(sf::VideoMode(Config::WINDOW_WIDTH, Config::WINDOW_HEIGHT),
              "Mario 2D - C++ OOP",
              sf::Style::Titlebar | sf::Style::Close),
      camera_(sf::FloatRect(0.f, 0.f,
                            static_cast<float>(Config::WINDOW_WIDTH),
                            static_cast<float>(Config::WINDOW_HEIGHT)))
{
    window_.setVerticalSyncEnabled(true);
    window_.setKeyRepeatEnabled(false);
    mario_.reset(map_.getSpawnPoint());
    updateCamera();
}

void Game::run()
{
    float accumulator = 0.f;

    while (window_.isOpen())
    {
        processEvents();

        const float frameTime =
            std::min(clock_.restart().asSeconds(), Config::MAX_FRAME_TIME);

        // Vật lý chạy theo bước cố định để va chạm ổn định ở mọi FPS.
        if (state_ == GameState::Playing)
        {
            accumulator += frameTime;
            while (accumulator >= Config::FIXED_STEP &&
                   state_ == GameState::Playing)
            {
                update(Config::FIXED_STEP);
                accumulator -= Config::FIXED_STEP;
            }
        }
        else
        {
            accumulator = 0.f;
        }

        updateCamera();
        render();
    }
}

void Game::processEvents()
{
    sf::Event event{};
    while (window_.pollEvent(event))
    {
        switch (event.type)
        {
            case sf::Event::Closed:
                window_.close();
                break;

            case sf::Event::LostFocus:
                mario_.handleEvent(event);
                if (state_ == GameState::Playing)
                    state_ = GameState::Paused;
                break;

            case sf::Event::KeyReleased:
                // Luôn nhận phím nhả để không bị kẹt phím khi đang Pause.
                mario_.handleEvent(event);
                break;

            case sf::Event::KeyPressed:
                if (state_ == GameState::Playing)
                    mario_.handleEvent(event);

                if (event.key.code == sf::Keyboard::Enter &&
                    state_ == GameState::Menu)
                    startGame();

                if ((event.key.code == sf::Keyboard::P ||
                     event.key.code == sf::Keyboard::Escape) &&
                    (state_ == GameState::Playing ||
                     state_ == GameState::Paused))
                {
                    state_ = (state_ == GameState::Playing)
                                 ? GameState::Paused : GameState::Playing;
                }

                if (event.key.code == sf::Keyboard::R &&
                    (state_ == GameState::GameOver ||
                     state_ == GameState::Win))
                    restartGame();
                break;

            default:
                break;
        }
    }
}

void Game::startGame()
{
    state_ = GameState::Playing;
    updateCamera();
}

void Game::restartGame()
{
    score_ = 0;
    mario_ = Mario();
    mario_.reset(map_.getSpawnPoint());
    map_.reset();
    state_ = GameState::Playing;
    updateCamera();
}

void Game::handleMarioDeath()
{
    mario_.loseLife();
    if (mario_.getLives() <= 0)
    {
        state_ = GameState::GameOver;
        return;
    }

    mario_.reset(map_.getSpawnPoint());
    mario_.setInvincible(1.5f);
}

void Game::update(float deltaTime)
{
    if (state_ != GameState::Playing)
        return;

    mario_.update(deltaTime);
    Collision::resolveMarioMap(mario_, map_);

    // Có thể ăn nhiều xu trong cùng một bước.
    while (map_.collectCoin(mario_.getBounds()))
        score_ += 10;

    if (map_.isGoalReached(mario_.getBounds()))
    {
        state_ = GameState::Win;
        return;
    }

    if (mario_.isDead())
        handleMarioDeath();
}

void Game::updateCamera()
{
    const float halfWidth = camera_.getSize().x / 2.f;
    float x = mario_.getPosition().x;
    x = std::max(halfWidth, std::min(x, map_.getWorldWidth() - halfWidth));
    camera_.setCenter(x, map_.getWorldHeight() / 2.f);
}

void Game::render()
{
    window_.clear(sf::Color(115, 195, 245));

    window_.setView(camera_);
    map_.draw(window_);
    mario_.draw(window_);

    ui_.draw(window_, state_, score_, mario_.getLives());
    window_.display();
}
