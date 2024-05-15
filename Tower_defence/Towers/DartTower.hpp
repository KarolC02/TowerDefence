#ifndef DARTTOWER_HPP
#define DARTTOWER_HPP

#include "Tower.hpp"

class DartTower : public Tower {
public:
    DartTower(sf::Vector2f position);
    void fireBullet(const Enemy* targetEnemy, float bulletSpeed) override;
    
};

#endif // DARTTOWER_HPP
