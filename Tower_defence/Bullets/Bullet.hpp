#ifndef BULLET_HPP
#define BULLET_HPP

#include <SFML/Graphics.hpp>
#include "Enemy.hpp"

class Bullet {
private:
    int damage;

public:
    bool shouldBeRemoved() const;
    sf::CircleShape shape;
    void updateDirection();
    const Enemy* targetEnemy;
    virtual ~Bullet() = default;
    sf::Vector2f getPosition() const; // Declare the getPosition method
    Bullet(const sf::Vector2f& startPosition,const Enemy* targetEnemy, float bulletSpeed, int damage);
    Bullet(const sf::Vector2f& startPosition,const Enemy* targetEnemy, float bulletSpeed, int damage, sf::Color color);
    virtual void update(float deltaTime);
    sf::Vector2f direction;
    virtual void draw(sf::RenderWindow& window) const;

    float speed;
    // Getters for encapsulated properties
    int getDamage() const { return damage; }
    const sf::CircleShape& getShape() const { return shape; }
    

    // If needed, setters or additional functionality can be added here
};

#endif // BULLET_HPP
