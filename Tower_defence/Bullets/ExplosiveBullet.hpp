#ifndef EXPLOSIVE_BULLET_HPP
#define EXPLOSIVE_BULLET_HPP

#include "Bullet.hpp"

class ExplosiveBullet : public Bullet {
private:
    float explosionRadius; // Radius of the explosion impact

public:
    ExplosiveBullet(sf::Vector2f startPos, sf::Vector2f targetPos, float spd, int dmg, float explosionRadius);
    // void update(float deltaTime) override; // Optionally override update if behavior is different
    float getExplosionRadius() const;
    // void draw(sf::RenderWindow& window) const override; // Optionally override if visual is different
};

#endif // EXPLOSIVE_BULLET_HPP
