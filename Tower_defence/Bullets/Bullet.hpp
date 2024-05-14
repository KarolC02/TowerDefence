#ifndef BULLET_HPP
#define BULLET_HPP

#include <SFML/Graphics.hpp>

class Bullet {
private:
    sf::CircleShape shape;
    sf::Vector2f direction;
    float speed;
    int damage;

public:
    virtual ~Bullet() = default;
    sf::Vector2f getPosition() const; // Declare the getPosition method
    Bullet(sf::Vector2f startPos, sf::Vector2f targetPos, float spd, int dmg);
    virtual void update(float deltaTime);

    virtual void draw(sf::RenderWindow& window) const;

    // Getters for encapsulated properties
    int getDamage() const { return damage; }
    const sf::CircleShape& getShape() const { return shape; }
    

    // If needed, setters or additional functionality can be added here
};

#endif // BULLET_HPP
