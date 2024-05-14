#include "ExplosiveBullet.hpp"

ExplosiveBullet::ExplosiveBullet(sf::Vector2f startPos, sf::Vector2f targetPos, float spd, int dmg, float explosionRadius)
    : Bullet(startPos, targetPos, spd, dmg), explosionRadius(explosionRadius) {}

//void ExplosiveBullet::update(float deltaTime) {
//    // Use the basic bullet update or add specific logic for moving explosion effects
//    Bullet::update(deltaTime);
//}

float ExplosiveBullet::getExplosionRadius() const {
    return explosionRadius;
}

//void ExplosiveBullet::draw(sf::RenderWindow& window) const {
//    // Optionally customize drawing if needed
//    Bullet::draw(window);
//}
