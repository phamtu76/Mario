#include "UI.h"
#include "Config.h"
#include <cmath>

UI::UI()
{
    // Ưu tiên font trong dự án, sau đó thử font hệ thống để chữ vẫn hiện
    // khi chưa có assets/font.ttf.
    const char* candidates[] = {
        "assets/font.ttf",
        "C:/Windows/Fonts/arial.ttf",
        "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf",
        "/System/Library/Fonts/Supplemental/Arial.ttf"
    };

    for (const char* path : candidates)
    {
        if (font_.loadFromFile(path))
        {
            fontLoaded_ = true;
            break;
        }
    }
}

void UI::drawText(sf::RenderWindow& window, const std::string& text,
                  unsigned int size, float x, float y, sf::Color color) const
{
    if (!fontLoaded_)
        return;

    sf::Text label(text, font_, size);
    label.setFillColor(color);
    label.setPosition(x, y);
    window.draw(label);
}

void UI::drawCentered(sf::RenderWindow& window, const std::string& text,
                      unsigned int size, float y, sf::Color color) const
{
    if (!fontLoaded_)
        return;

    sf::Text label(text, font_, size);
    label.setFillColor(color);
    const sf::FloatRect b = label.getLocalBounds();
    const float x = (static_cast<float>(Config::WINDOW_WIDTH) - b.width) / 2.f
                    - b.left;
    label.setPosition(std::floor(x), y);
    window.draw(label);
}

void UI::draw(sf::RenderWindow& window, GameState state,
              int score, int lives) const
{
    const sf::View oldView = window.getView();
    window.setView(window.getDefaultView());

    if (state == GameState::Playing || state == GameState::Paused)
    {
        drawText(window, "SCORE: " + std::to_string(score), 22, 20.f, 15.f);
        drawText(window, "LIVES: " + std::to_string(lives), 22, 20.f, 45.f);
    }

    if (state != GameState::Playing)
    {
        sf::RectangleShape shade({static_cast<float>(Config::WINDOW_WIDTH),
                                  static_cast<float>(Config::WINDOW_HEIGHT)});
        shade.setFillColor(sf::Color(0, 0, 0, 155));
        window.draw(shade);

        std::string title;
        std::string subtitle;

        switch (state)
        {
            case GameState::Menu:
                title = "MARIO 2D";
                subtitle = "Press ENTER to start";
                break;
            case GameState::Paused:
                title = "PAUSED";
                subtitle = "Press P or ESC to continue";
                break;
            case GameState::GameOver:
                title = "GAME OVER";
                subtitle = "Press R to restart";
                break;
            case GameState::Win:
                title = "YOU WIN!";
                subtitle = "Press R to play again";
                break;
            default:
                break;
        }

        drawCentered(window, title, 48, 190.f, sf::Color::Yellow);
        drawCentered(window, subtitle, 24, 270.f);

        if (state == GameState::GameOver || state == GameState::Win)
            drawCentered(window, "Final score: " + std::to_string(score),
                         22, 320.f);

        if (state == GameState::Menu)
            drawCentered(window,
                         "A/D or Arrows: move   Space/W/Up: jump   P/Esc: pause",
                         18, 450.f, sf::Color(220, 220, 220));
    }

    window.setView(oldView);
}
