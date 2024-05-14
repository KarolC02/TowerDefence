#include "FrostBullet.hpp"

// Constructor with initialization list corrected
FrostBullet::FrostBullet(sf::Vector2f startPos, sf::Vector2f targetPos, float spd, int dmg, float slowFactor, float areaEffectRadius, float slowDuration)
    : Bullet(startPos, targetPos, spd, dmg), slowFactor(slowFactor), areaEffectRadius(areaEffectRadius), slowDuration(slowDuration) {}

float FrostBullet::getSlowFactor() const {
    return slowFactor;
}

float FrostBullet::getAreaEffectRadius() const {
    return areaEffectRadius;
}

float FrostBullet::getSlowDuration() const {
    return slowDuration;
}

void FrostBullet::update(float deltaTime) {
    // Custom update logic if necessary
    Bullet::update(deltaTime);
}

void FrostBullet::draw(sf::RenderWindow& window) const {
    // Custom draw logic if necessary
    Bullet::draw(window);
}
