#ifndef FROSTTOWER_HPP
#define FROSTTOWER_HPP

#include "Tower.hpp"
#include "FrostBullet.hpp"

class FrostTower : public Tower {
public:
    FrostTower(sf::Vector2f position);
    void fireBullet(const Enemy* targetEnemy, float bulletSpeed) override;
//    void update(float deltaTime) override;
};

#endif // FROSTTOWER_HPP
