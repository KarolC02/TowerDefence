#include "Enemy.hpp"
#include <iostream>
#include "DEFINE.hpp"

Enemy::Enemy(sf::Vector2f startPosition, int initialHealth, int val, bool flying, bool spn, bool slw, bool fst)
    : position(startPosition), health(initialHealth), maxHealth(initialHealth), speed(ENEMY_SPEED), isDead(false), immune(true), gotPath(false), value(val), _originalSpeed(speed), flying(flying), isStunned(false), spawn(spn), slowImmune(slw), fast(fst) {
    
        if(fast) {
            speed *= 1.5;
            _originalSpeed = speed;
        }
    shape.setRadius(10);
    shape.setFillColor(sf::Color::Blue);
    shape.setOrigin(shape.getRadius(), shape.getRadius());
    shape.setPosition(position);

    // Initialize health bar background
    healthBarBackground.setSize(sf::Vector2f(20, 5));
    healthBarBackground.setFillColor(sf::Color(50, 50, 50)); // Dark gray color
    healthBarBackground.setOrigin(healthBarBackground.getSize().x / 2, healthBarBackground.getSize().y / 2);

    // Initialize health bar foreground
    healthBarForeground.setSize(sf::Vector2f(20, 5));
    healthBarForeground.setFillColor(sf::Color::Green); healthBarForeground.setOrigin(healthBarForeground.getSize().x / 2, healthBarForeground.getSize().y / 2);
    
    // std::cout << "Spawned enemy with health: " << health << std::endl;
}

void Enemy::update(float deltaTime) {
    if (isStunned && stunTimer.getElapsedTime().asSeconds() > STUN_TIME) {
        isStunned = false;
        speed = _originalSpeed;
    }
    if (position.x > LEFT_OFFSET) {
        immune = false;
    }
    
    if( !isStunned ){
        if( _slowClock.getElapsedTime().asSeconds() > 2.0f ){
            _slowedDown = false;
            speed = _originalSpeed;
        }
        
        if( flying == true ){
            position.x += speed * deltaTime;
        }
        
        // If the enemy is in a specified zone, keep moving right without following the path
        else if (position.x >= LEFT_OFFSET + GRID_CELL_SIZE * 27.5 &&
                 position.y >= TOP_OFFSET + GRID_CELL_SIZE * 9.5 &&
                 position.y <= TOP_OFFSET + ARENA_HEIGHT - 9.5 * GRID_CELL_SIZE) {
            position.x += speed * deltaTime;
        }
        
        else if (!immune && gotPath && !path.empty()) {
            sf::Vector2f targetPosition = {
                static_cast<float>(path[nextCellIndex].first * GRID_CELL_SIZE + LEFT_OFFSET + GRID_CELL_SIZE / 2),
                static_cast<float>(path[nextCellIndex].second * GRID_CELL_SIZE + TOP_OFFSET + GRID_CELL_SIZE / 2)
            };
            
            // Move towards the current target position in the path
            moveTowards(targetPosition, deltaTime);
            
            // Check if the enemy is close enough to the center of the target cell
            if (std::hypot(position.x - targetPosition.x, position.y - targetPosition.y) < speed * deltaTime) {
                position = targetPosition;  // Snap to the center of the cell
                nextCellIndex++;  // Move to the next cell in the path
                
                // If this was the last cell, the path is completed
                if (nextCellIndex >= path.size()) {
                    gotPath = false;  // Reset path flag
                    nextCellIndex = 0;  // Reset index
                }
            }
        } else {
            // Continue moving right if still immune
            position.x += speed * deltaTime;
        }
    }

    // Update the graphical representation
    shape.setPosition(position);
    updateHealthBar();

    // Check if the enemy is dead
    if (health <= 0) {
        isDead = true;
    }
}



void Enemy::stun(float duration) {
    isStunned = true;
    stunTimer.restart();
    speed = 0;  // Optionally set speed to zero or reduce it significantly
}


void Enemy::moveTowards(const sf::Vector2f& targetPosition, float deltaTime) {
    sf::Vector2f direction = targetPosition - position;
    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    if (length > 0) {
        direction /= length; // Normalize
        position += direction * speed * deltaTime; // Move towards the target
        shape.setPosition(position);
    }
}

void Enemy::updateHealthBar() {
    healthBarBackground.setPosition(position.x, position.y - shape.getRadius() - 10);
    healthBarForeground.setPosition(position.x, position.y - shape.getRadius() - 10);
    healthBarForeground.setPosition(position.x, position.y - shape.getRadius() - 10);
    float healthRatio = static_cast<float>(health) / maxHealth;
    healthBarForeground.setSize(sf::Vector2f(20 * healthRatio, 5));
}

void Enemy::draw(sf::RenderWindow& window) const {
    window.draw(shape);
    window.draw(healthBarBackground);
    window.draw(healthBarForeground);
}

// Setters
void Enemy::setHealth(int newHealth) {
    health = newHealth;
    updateHealthBar();
}

void Enemy::setIsDead(bool dead) {
    isDead = dead;
}

void Enemy::setPath( std::vector<std::pair<int, int>> path1){
    path = path1;
    nextCellIndex = 1; // Reset index to start of new path
    gotPath = true; // Indicate that the enemy has a path
}

void Enemy::damage(int hitPoints) {
    health -= hitPoints;
    updateHealthBar();
    if (health <= 0) {
        isDead = true;
        health = 0; // Ensure health does not go negative
    }
}

sf::Vector2i Enemy::getGridPosition() const {
    if (immune) {
        // Return an invalid grid position when the enemy is immune
        return sf::Vector2i(-1, -1);
    } else {
        int gridX = static_cast<int>((position.x - LEFT_OFFSET) / GRID_CELL_SIZE);
        int gridY = static_cast<int>((position.y - TOP_OFFSET) / GRID_CELL_SIZE);
        return sf::Vector2i(gridX, gridY);
    }
}

sf::Vector2f Enemy::getPosition() const {
    return position;
}

bool Enemy::isImmune() const{
    return immune;
}
sf::Vector2f Enemy::snapToGrid(const sf::Vector2f& position)const{
    sf::Vector2f snappedPos;
    snappedPos.x = std::floor((position.x - LEFT_OFFSET) / GRID_CELL_SIZE) * GRID_CELL_SIZE + LEFT_OFFSET;
    snappedPos.y = std::floor((position.y - TOP_OFFSET) / GRID_CELL_SIZE) * GRID_CELL_SIZE + TOP_OFFSET;
    return snappedPos;
}

void Enemy::applySlow(float factor, float duration) {
    if( !slowImmune ){
        if (!isImmune() && !_slowedDown ) {
            speed *= (1 - factor);  // Apply slowing factor to the speed
            _slowClock.restart();
            _slowedDown = true;
            // Here you should manage the duration of this effect possibly with a timer or event system
        }
    }
}
