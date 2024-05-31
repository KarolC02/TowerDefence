#ifndef SWARMTOWER_HPP
#define SWARMTOWER_HPP

#include "Tower.hpp"

class SwarmTower : public Tower {
public:
    SwarmTower(sf::Vector2f position);
    void fireBullet(const Enemy* targetEnemy, float bulletSpeed) override;
    void update(float deltaTime, std::vector<Enemy>& enemies, const sf::RenderWindow& window) override;
};

#endif // SWARMTOWER_HPP
