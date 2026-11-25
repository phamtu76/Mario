// File TEST RIÊNG cho module của thành viên 2 (không dùng khi ghép vào Game).
// Điều khiển ô vuông xanh: mũi tên trái/phải để di chuyển, Space để nhảy.
// Thử: giẫm lên Goomba/Koopa, đá mai Koopa, nhặt Coin, ăn Mushroom.
#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include <iostream>
#include <algorithm>
#include "Goomba.h"
#include "Koopa.h"
#include "Coin.h"
#include "Mushroom.h"

int main() {
    sf::RenderWindow window(sf::VideoMode(900, 500), "Test Enemy + Item");
    window.setFramerateLimit(60);
    const float groundY = 440.f;

    std::vector<std::unique_ptr<Enemy>> enemies;
    std::vector<std::unique_ptr<Item>>  items;
    enemies.push_back(std::make_unique<Goomba>(500.f, 300.f));
    enemies.push_back(std::make_unique<Koopa>(700.f, 300.f));
    for (auto& e : enemies) { e->setGroundLevel(groundY); e->setPatrolRange(300.f, 880.f); }
    for (int i = 0; i < 5; ++i) items.push_back(std::make_unique<Coin>(250.f + i * 50.f, 350.f));
    auto mush = std::make_unique<Mushroom>(150.f, 300.f);
    mush->setGroundLevel(groundY);
    items.push_back(std::move(mush));

    // "Mario" giả
    sf::RectangleShape mario({32.f, 48.f});
    mario.setFillColor(sf::Color::Blue);
    sf::Vector2f pos(50.f, groundY - 48.f), vel(0.f, 0.f);
    int score = 0, lives = 3;

    sf::Clock clock;
    while (window.isOpen()) {
        sf::Event ev;
        while (window.pollEvent(ev)) if (ev.type == sf::Event::Closed) window.close();
        float dt = clock.restart().asSeconds();

        vel.x = 0.f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))  vel.x = -200.f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) vel.x =  200.f;
        bool grounded = pos.y + 48.f >= groundY;
        if (grounded && sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) vel.y = -520.f;
        vel.y += 1200.f * dt;
        pos += vel * dt;
        if (pos.y + 48.f > groundY) { pos.y = groundY - 48.f; vel.y = 0.f; }
        mario.setPosition(pos);
        sf::FloatRect mb(pos.x, pos.y, 32.f, 48.f);

        // ---- Enemy ----
        for (auto& e : enemies) {
            e->update(dt);
            if (!e->isAlive() && e->getState() != EnemyState::Stomped) continue;
            if (!mb.intersects(e->getBounds())) continue;
            bool stomp = vel.y > 0.f && (mb.top + mb.height) < (e->getBounds().top + e->getBounds().height * 0.6f);
            if (stomp) {
                e->onStomped(); score += e->getScore(); vel.y = -300.f;
            } else if (e->canHurtPlayer()) {
                --lives; pos = {50.f, groundY - 48.f}; std::cout << "Mario bi thuong! Mang: " << lives << "\n";
            } else {
                e->onTouchedBySide(mb.left + mb.width / 2.f);
            }
        }
        // mai rùa đang trượt hạ gục enemy khác
        for (auto& a : enemies) {
            Koopa* k = dynamic_cast<Koopa*>(a.get());
            if (!k || !k->isShellMoving()) continue;
            for (auto& b : enemies)
                if (b.get() != a.get() && b->getState() == EnemyState::Walking && k->getBounds().intersects(b->getBounds())) {
                    b->onHitByShell(); score += b->getScore();
                }
        }
        // ---- Item ----
        for (auto& it : items) {
            it->update(dt);
            if (!it->isCollected() && mb.intersects(it->getBounds())) {
                Mushroom* m = dynamic_cast<Mushroom*>(it.get());
                if (m && m->isEmerging()) continue;
                score += it->getScore();
                it->onCollected();
                if (it->getType() == ItemType::Mushroom) std::cout << "Mario an nam -> lon len!\n";
            }
        }
        enemies.erase(std::remove_if(enemies.begin(), enemies.end(), [](auto& e){ return e->isRemovable(); }), enemies.end());
        items.erase(std::remove_if(items.begin(), items.end(), [](auto& i){ return i->isRemovable(); }), items.end());

        window.setTitle("Score: " + std::to_string(score) + "  Lives: " + std::to_string(lives));
        window.clear(sf::Color(110, 180, 255));
        sf::RectangleShape ground({900.f, 60.f}); ground.setPosition(0.f, groundY); ground.setFillColor(sf::Color(120, 80, 40));
        window.draw(ground);
        for (auto& e : enemies) e->draw(window);
        for (auto& i : items)   i->draw(window);
        window.draw(mario);
        window.display();
    }
}
