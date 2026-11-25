#pragma once

#include "Mario.h"
#include "Map.h"

class Collision
{
public:
    static void resolveMarioMap(Mario& mario, const Map& map);
    static bool intersects(const sf::FloatRect& a, const sf::FloatRect& b);
};
