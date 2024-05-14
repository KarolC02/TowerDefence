#include "Tower.hpp"
#include "DEFINE.hpp"
#include "ResourcePath.hpp"
#include <cmath>
#include <stdexcept>

static sf::Font towerFont;

Tower::Tower(sf::Vector2f position, sf::Color color, float atkSpeed, int rng, const std::string& typeSymbol, int dmg, int cst)
    : attackSpeed(atkSpeed), range(rng), damage(dmg), cost(cst), attackCooldown(0.0f), currentLevel(0) {
    if (!towerFont.loadFromFile(resourcePath() + "arial.ttf")) {
        throw std::runtime_error("Failed to load font!");
    }

    // Setup visual appearance as before
    shape.setSize(sf::Vector2f(TOWER_SIDE_LENGTH, TOWER_SIDE_LENGTH));
    shape.setFillColor(color);
    shape.setOrigin(TOWER_SIDE_LENGTH / 2, TOWER_SIDE_LENGTH / 2);
    shape.setPosition(position);
    shape.setOutlineThickness(-2.0f);
    shape.setOutlineColor(sf::Color::Black);

    text.setFont(towerFont);
    text.setString(typeSymbol);
    text.setCharacterSize(TOWER_SIDE_LENGTH / 2);
    text.setFillColor(sf::Color::White);
    sf::FloatRect textRect = text.getLocalBounds();
    text.setOrigin(textRect.left + textRect.width / 2, textRect.top + textRect.height / 2);
    text.setPosition(position);
}

// Implementation of new upgrade methods
bool Tower::canUpgrade() const {
    return currentLevel < upgrades.size();
}

void Tower::upgrade() {
    if (canUpgrade()) {
        info = upgrades[currentLevel].name;
        attackSpeed = upgrades[currentLevel].attackSpeed;
        range = upgrades[currentLevel].range;
        damage = upgrades[currentLevel].damage;
        cost += upgrades[currentLevel].cost;
        currentLevel++;
    }
}

std::string Tower::getNextUpgradeName() const {
    if (canUpgrade()) {
        return upgrades[currentLevel].name;
    }
    return "Max Level";
}

int Tower::getNextUpgradeCost() const {
    std::cout << "Current Level: " << currentLevel << ", Upgrades Size: " << upgrades.size() << std::endl;
    if (canUpgrade()) {
        printf("HELLO!!!");
        return upgrades[currentLevel ].cost;
    }
    return 0;
}


int Tower::getNextUpgradeDamage() const {
    printf("LOOOL\n");
    if (canUpgrade()) {
        return upgrades[currentLevel].damage;
    }
    return 0;
}

int Tower::getNextUpgradeSpeed() const {
    if (canUpgrade()) {
        return static_cast<int>(upgrades[currentLevel ].attackSpeed);
    }
    return 0;
}

int Tower::getNextUpgradeRange() const {
    if (canUpgrade()) {
        return upgrades[currentLevel ].range;
    }
    return 0;
}


void Tower::draw(sf::RenderWindow& window) const {
    window.draw(shape);
    window.draw(text);
    for (const auto& bullet : bullets) {
        bullet->draw(window);
    }
}

void Tower::fireBullet(const sf::Vector2f& targetPosition, float bulletSpeed) {
    if (canAttack()) {
        bullets.emplace_back(std::make_unique<Bullet>(shape.getPosition(), targetPosition, bulletSpeed, damage));
        resetCooldown();
    }
}

bool Tower::canAttack() const {
    return attackCooldown <= 0;
}

void Tower::resetCooldown() {
    attackCooldown = attackSpeed;
}

sf::Vector2f Tower::getPosition() const {
    return shape.getPosition();
}


// Inside the Tower class
void Tower::eraseOutOfScreenBullets(const sf::RenderWindow& window) {
    auto bulletIter = bullets.begin();
    while (bulletIter != bullets.end()) {
        sf::Vector2f pos = (*bulletIter)->getPosition();
        // Check if the bullet is out of the Arena bounds
        if (pos.x < LEFT_OFFSET || pos.y < TOP_OFFSET || pos.x > LEFT_OFFSET + ARENA_WIDTH || pos.y > TOP_OFFSET + ARENA_HEIGHT) {
            bulletIter = bullets.erase(bulletIter);
        } else {
            ++bulletIter;
        }
    }
}


void Tower::update(float deltaTime, const std::vector<Enemy>& enemies, const sf::RenderWindow& window) {
    // Cooldown timer decrement
    // std::cout << "There are currently " << bullets.size() << " bullets" << std::endl;
    if (attackCooldown > 0) {
        attackCooldown -= deltaTime;
    }

    for (const auto& enemy : enemies) {
        if (isInRange(enemy.getShape().getPosition()) && canAttack()) {
            fireBullet(enemy.getShape().getPosition(), BULLET_SPEED);
            break; // Stop checking after finding the first enemy in range
        }
    }

    // Update bullets and remove out-of-screen bullets
    auto bulletIter = bullets.begin();
    while (bulletIter != bullets.end()) {
        (*bulletIter)->update(deltaTime); // Update the bullet's position
        ++bulletIter;
    }
    
    eraseOutOfScreenBullets(window);
    
}

bool Tower::isInRange(const sf::Vector2f& enemyPosition) const {
    float dx = enemyPosition.x - shape.getPosition().x;
    float dy = enemyPosition.y - shape.getPosition().y;
    return std::sqrt(dx * dx + dy * dy) <= range;
}

void Tower::setPosition(sf::Vector2f pos){
    shape.setPosition(pos.x,pos.y);
    text.setPosition(pos.x,pos.y);
}

void Tower::setColor(sf::Color color){
    shape.setFillColor(color);
}

const std::vector<std::unique_ptr<Bullet>>& Tower::getBullets() const {
    return bullets;
}

// Non-const version: allows modification of the bullets vector
std::vector<std::unique_ptr<Bullet>>& Tower::getBullets() {
    return bullets;
}

int Tower::getCost() const{
    return cost;
}

int Tower::getDamage() const{
    return damage;
}

int Tower::getSpeed() const{
    return attackSpeed;
}

int Tower::getRange() const{
    return range;
}

std::string Tower::getInfo() const{
    return info;
}
