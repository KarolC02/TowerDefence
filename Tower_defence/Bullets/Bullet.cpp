#include "Bullet.hpp"
#include <cmath> // For std::sqrt

Bullet::Bullet(sf::Vector2f startPos, sf::Vector2f targetPos, float spd, int dmg)
    : speed(spd), damage(dmg) {
    shape.setRadius(5);
    shape.setFillColor(sf::Color::Blue);
    shape.setOrigin(5, 5);
    shape.setPosition(startPos);

    // Calculate direction vector
    sf::Vector2f dir = targetPos - startPos;
    float length = std::sqrt(dir.x * dir.x + dir.y * dir.y);
    direction = dir / length; // Normalize direction vector
}

void Bullet::update(float deltaTime) {
    // Calculate the new position based on the direction vector and speed
    sf::Vector2f movement = direction * speed * deltaTime;
    shape.move(movement);
}


void Bullet::draw(sf::RenderWindow& window) const {
    window.draw(shape);
}

sf::Vector2f Bullet::getPosition() const {
    return shape.getPosition();
}
