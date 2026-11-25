#include "Map.h"
#include "Config.h"
#include <algorithm>

namespace
{
constexpr float TILE = Config::TILE;
constexpr float FLOOR_TOP = 14.f * TILE;
constexpr float COIN_RADIUS = 9.f;
constexpr float PIPE_LIP_EXTRA = 4.f;

sf::FloatRect getViewRect(const sf::RenderWindow& window)
{
    const sf::View& v = window.getView();
    return {v.getCenter().x - v.getSize().x / 2.f,
            v.getCenter().y - v.getSize().y / 2.f,
            v.getSize().x, v.getSize().y};
}
}

Map::Map()
{
    flagPole_.setSize({5.f, 190.f});
    flagPole_.setFillColor(sf::Color(235, 235, 220));
    flagPole_.setPosition(worldWidth_ - 120.f, 258.f);

    flag_.setPointCount(3);
    flag_.setPoint(0, {0.f, 0.f});
    flag_.setPoint(1, {52.f, 17.f});
    flag_.setPoint(2, {0.f, 34.f});
    flag_.setFillColor(sf::Color(40, 190, 80));
    flag_.setPosition(worldWidth_ - 115.f, 260.f);

    background_.setSize({worldWidth_, worldHeight_});
    background_.setFillColor(sf::Color(115, 195, 245));

    buildLevel();
}

void Map::reset()
{
    buildLevel();
}

void Map::buildLevel()
{
    solids_.clear();
    floors_.clear();
    platforms_.clear();
    pipes_.clear();
    coins_.clear();

    // Nền đất: mỗi đoạn là MỘT hình chữ nhật liền (không có khe giữa các ô).
    // Hai hố ở ô 25-27 và 43-45 để tính năng "rơi khỏi bản đồ" có ý nghĩa.
    const int floorSegments[][2] = {{0, 25}, {28, 43}, {46, 60}};
    for (const auto& s : floorSegments)
    {
        const sf::FloatRect r(s[0] * TILE, FLOOR_TOP, (s[1] - s[0]) * TILE,
                              worldHeight_ - FLOOR_TOP);
        floors_.push_back(r);
        solids_.push_back(r);
    }

    // Bục lơ lửng {ô x bắt đầu, hàng, số ô}.
    const int platforms[][3] = {
        {6, 10, 4}, {13, 8, 4}, {21, 10, 5},
        {30, 8, 4}, {39, 10, 5}, {48, 8, 4}
    };
    for (const auto& p : platforms)
    {
        const sf::FloatRect r(p[0] * TILE, p[1] * TILE, p[2] * TILE, TILE);
        platforms_.push_back(r);
        solids_.push_back(r);
    }

    // Ống nước.
    const sf::FloatRect pipes[] = {
        {520.f, 352.f, 64.f, 96.f},
        {1050.f, 320.f, 64.f, 128.f},
        {1510.f, 352.f, 64.f, 96.f}
    };
    for (const auto& r : pipes)
    {
        pipes_.push_back(r);
        solids_.push_back(r);
    }

    // Đồng xu.
    const sf::Vector2f coinPositions[] = {
        {230.f, 290.f}, {270.f, 290.f}, {430.f, 250.f},
        {700.f, 260.f}, {740.f, 260.f}, {848.f, 360.f},
        {900.f, 300.f}, {1200.f, 250.f}, {1240.f, 250.f},
        {1424.f, 360.f}, {1450.f, 290.f}
    };
    coins_.assign(std::begin(coinPositions), std::end(coinPositions));
}

void Map::draw(sf::RenderWindow& window) const
{
    // Chỉ vẽ những gì nằm trong khung nhìn (culling).
    const sf::FloatRect view = getViewRect(window);

    window.draw(background_);

    // Đồi trang trí.
    sf::CircleShape hill(70.f);
    hill.setScale(1.8f, 0.8f);
    hill.setFillColor(sf::Color(90, 195, 115));
    for (int i = 0; i < 8; ++i)
    {
        const float x = i * 270.f + 40.f;
        if (!view.intersects(sf::FloatRect(x, 365.f, 252.f, 112.f)))
            continue;
        hill.setPosition(x, 365.f);
        window.draw(hill);
    }

    // Nền đất.
    sf::RectangleShape shape;
    shape.setOutlineThickness(1.f);
    for (const auto& r : floors_)
    {
        if (!view.intersects(r))
            continue;
        shape.setSize({r.width, r.height});
        shape.setPosition(r.left, r.top);
        shape.setFillColor(sf::Color(135, 85, 40));
        shape.setOutlineColor(sf::Color(85, 50, 25));
        window.draw(shape);

        sf::RectangleShape grass({r.width, 6.f});
        grass.setPosition(r.left, r.top);
        grass.setFillColor(sf::Color(55, 175, 65));
        window.draw(grass);
    }

    // Bục gạch (vẽ từng ô cho giống gạch).
    shape.setSize({TILE, TILE});
    shape.setFillColor(sf::Color(190, 105, 55));
    shape.setOutlineColor(sf::Color(115, 60, 30));
    for (const auto& r : platforms_)
    {
        if (!view.intersects(r))
            continue;
        const int count = static_cast<int>(r.width / TILE);
        for (int i = 0; i < count; ++i)
        {
            shape.setPosition(r.left + i * TILE, r.top);
            window.draw(shape);
        }
    }

    // Đồng xu.
    sf::CircleShape coin(COIN_RADIUS);
    coin.setOrigin(COIN_RADIUS, COIN_RADIUS);
    coin.setFillColor(sf::Color(255, 210, 20));
    coin.setOutlineThickness(2.f);
    coin.setOutlineColor(sf::Color(190, 130, 0));
    for (const auto& c : coins_)
    {
        if (c.x < view.left - 20.f || c.x > view.left + view.width + 20.f)
            continue;
        coin.setPosition(c);
        window.draw(coin);
    }

    // Ống nước.
    sf::RectangleShape pipeBody;
    pipeBody.setFillColor(sf::Color(30, 165, 65));
    pipeBody.setOutlineThickness(3.f);
    pipeBody.setOutlineColor(sf::Color(15, 95, 35));

    sf::RectangleShape lip({64.f + 2.f * PIPE_LIP_EXTRA, 16.f});
    lip.setFillColor(sf::Color(50, 205, 80));
    lip.setOutlineThickness(2.f);
    lip.setOutlineColor(sf::Color(15, 95, 35));

    for (const auto& r : pipes_)
    {
        if (!view.intersects(r))
            continue;
        pipeBody.setSize({r.width, r.height});
        pipeBody.setPosition(r.left, r.top);
        window.draw(pipeBody);

        lip.setPosition(r.left - PIPE_LIP_EXTRA, r.top);
        window.draw(lip);
    }

    // Cột cờ.
    window.draw(flagPole_);
    window.draw(flag_);
}

const std::vector<sf::FloatRect>& Map::getSolids() const { return solids_; }
sf::Vector2f Map::getSpawnPoint() const { return spawnPoint_; }
float Map::getWorldWidth() const { return worldWidth_; }
float Map::getWorldHeight() const { return worldHeight_; }

bool Map::collectCoin(const sf::FloatRect& playerBounds)
{
    const auto it = std::find_if(coins_.begin(), coins_.end(),
        [&playerBounds](const sf::Vector2f& p)
        {
            const sf::FloatRect coinBounds(p.x - COIN_RADIUS, p.y - COIN_RADIUS,
                                           COIN_RADIUS * 2.f, COIN_RADIUS * 2.f);
            return playerBounds.intersects(coinBounds);
        });

    if (it == coins_.end())
        return false;

    coins_.erase(it);
    return true;
}

bool Map::isGoalReached(const sf::FloatRect& playerBounds) const
{
    const sf::FloatRect goal(worldWidth_ - 145.f, 250.f, 55.f, 198.f);
    return playerBounds.intersects(goal);
}
