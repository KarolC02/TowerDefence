#ifndef BASHTOWER_HPP
#define BASHTOWER_HPP

#include "Tower.hpp"
#include <random>

class BashTower : public Tower {
public:
    BashTower(sf::Vector2f position);
    void fireBullet(const sf::Vector2f& targetPosition, float bulletSpeed) override;
    void update(float deltaTime, const std::vector<Enemy>& enemies, const sf::RenderWindow& window) override;
    float getChanceToStun();
};

#endif // BASHTOWER_HPP
