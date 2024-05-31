#include "ExplosiveBullet.hpp"

// Constructor with initialization list corrected
ExplosiveBullet::ExplosiveBullet(sf::Vector2f startPos, const Enemy* targetEnemy, float spd, int dmg, float explosionRadius)
    : Bullet(startPos, targetEnemy, spd, dmg), explosionRadius(explosionRadius) {}

float ExplosiveBullet::getExplosionRadius() const {
    return explosionRadius;
}
