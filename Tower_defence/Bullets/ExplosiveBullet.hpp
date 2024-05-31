#ifndef EXPLOSIVEBULLET_HPP
#define EXPLOSIVEBULLET_HPP

#include "Bullet.hpp"

class ExplosiveBullet : public Bullet {
public:
    ExplosiveBullet(sf::Vector2f startPos,const Enemy* targetEnemy, float spd, int dmg, float explosionRadius);

    float getExplosionRadius() const;

private:
    float explosionRadius;
};

#endif // EXPLOSIVEBULLET_HPP
