#include "Collision.h"
#include "Config.h"
#include <algorithm>

bool Collision::intersects(const sf::FloatRect& a, const sf::FloatRect& b)
{
    return a.intersects(b);
}

void Collision::resolveMarioMap(Mario& mario, const Map& map)
{
    sf::Vector2f position = mario.getPosition();
    sf::Vector2f velocity = mario.getVelocity();
    const float halfW = Config::PLAYER_WIDTH / 2.f;
    const float halfH = Config::PLAYER_HEIGHT / 2.f;
    bool onGround = false;

    // Đẩy Mario ra khỏi vật cản theo trục có độ xuyên nhỏ nhất.
    // Cách này tránh lỗi "bị đẩy ngang" khi vừa rơi xuống mép bục.
    // Lặp 2 lượt để xử lý trường hợp chạm nhiều vật cản cùng lúc.
    for (int pass = 0; pass < 2; ++pass)
    {
        for (const auto& solid : map.getSolids())
        {
            const sf::FloatRect bounds(position.x - halfW, position.y - halfH,
                                       halfW * 2.f, halfH * 2.f);
            sf::FloatRect overlap;
            if (!bounds.intersects(solid, overlap))
                continue;

            if (overlap.width < overlap.height)
            {
                const bool marioOnLeft =
                    position.x < solid.left + solid.width / 2.f;
                position.x += marioOnLeft ? -overlap.width : overlap.width;
                velocity.x = 0.f;
            }
            else
            {
                const bool marioAbove =
                    position.y < solid.top + solid.height / 2.f;
                if (marioAbove)
                {
                    position.y -= overlap.height;
                    if (velocity.y > 0.f)
                        velocity.y = 0.f;
                    onGround = true;
                }
                else
                {
                    position.y += overlap.height;
                    if (velocity.y < 0.f)
                        velocity.y = 0.f;
                }
            }
        }
    }

    // Không cho đi ra ngoài biên trái/phải của bản đồ.
    position.x = std::max(halfW,
                          std::min(position.x, map.getWorldWidth() - halfW));

    // Rơi khỏi thế giới -> chết.
    if (position.y > map.getWorldHeight() + 100.f)
    {
        mario.kill();
        return;
    }

    mario.setPosition(position);
    mario.setVelocity(velocity);
    mario.setOnGround(onGround);
}
