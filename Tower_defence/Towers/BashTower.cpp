#include "BashTower.hpp"
#include "DEFINE.hpp"
#include <iostream>
#include <algorithm> // For std::remove_if

// Constructor definition
BashTower::BashTower(sf::Vector2f position)
    : Tower(position, BASH_TOWER_COLOR, SLOW, 3 * GRID_CELL_SIZE, "B", 10, 30) {
    info = "Bash Tower";
    upgrades = {
        TowerUpgrade("Bash Tower 2", SLOW, 2 * GRID_CELL_SIZE + 2 / 3 * GRID_CELL_SIZE, 20, 25),
        TowerUpgrade("Bash Tower 3", SLOW, 2 * GRID_CELL_SIZE + 2 / 3 * GRID_CELL_SIZE, 40, 50),
        TowerUpgrade("Bash Tower 4", SLOW, 2 * GRID_CELL_SIZE + 2 / 3 * GRID_CELL_SIZE, 80, 100),
        TowerUpgrade("Bash Tower 5", SLOW, 2 * GRID_CELL_SIZE + 2 / 3 * GRID_CELL_SIZE, 160, 185),
        TowerUpgrade("Quake Tower", SLOW, 2 * GRID_CELL_SIZE + 2 / 3 * GRID_CELL_SIZE, 320, 355)
    };

    // Initialize stomp effect
    stompEffect.setRadius(getRange());
    stompEffect.setFillColor(sf::Color(0, 0, 0, 100)); // Black with some transparency
    stompEffect.setOrigin(stompEffect.getRadius(), stompEffect.getRadius());
    stompEffectAlpha = 100;
    showStompEffect = false;
}

void BashTower::fireBullet(const Enemy* targetEnemy, float bulletSpeed) {
    std::cout << "BashTower firing bullet" << std::endl;
}

void BashTower::update(float deltaTime, std::vector<Enemy>& enemies, const sf::RenderWindow& window) {
    static std::random_device rd;  // Random device to seed the generator
    static std::mt19937 gen(rd()); // Mersenne Twister random number generator
    static std::uniform_real_distribution<float> dis(0.0f, 1.0f); // Uniform distribution [0, 1]

    if (attackCooldown > 0) {
        attackCooldown -= deltaTime;
    }

    if (canAttack()) {
        sf::Vector2f towerPos = getPosition();
        float attackRange = getRange();
        float stunChance = getChanceToStun();
        float randomValue = dis(gen);
        bool doWeStun = (randomValue <= stunChance);

        bool attacked = false;
        for (auto& enemy : enemies) {
            sf::Vector2f enemyPos = enemy.getPosition();
            float distance = std::hypot(towerPos.x - enemyPos.x, towerPos.y - enemyPos.y);

            if (distance <= attackRange) {
                enemy.damage(getDamage());
                attacked = true;
                // Check if the enemy should be stunned
                float randomValue = dis(gen);  // Generate a random number between 0 and 1
                if (doWeStun) {
                    enemy.stun(STUN_DURATION);  // Stun the enemy, where STUN_DURATION is the duration of the stun
                }
            }
        }

        if (attacked) {
            resetCooldown();  // Reset cooldown after attack
            showStompEffect = true;
            stompEffect.setPosition(towerPos);
            stompEffectAlpha = 100;

            if (PARTICLES_ON) {
                // Create particles at the tower's position and move them immediately
                createParticles(towerPos);
            }
        }
    }

    // Update stomp effect visibility
    if (showStompEffect) {
        stompEffectAlpha -= 100.0f * deltaTime; // Fade out effect
        if (stompEffectAlpha <= 0) {
            stompEffectAlpha = 0;
            showStompEffect = false;
        }
    }

    if (PARTICLES_ON) {
        // Update particles
        for (auto& particle : particles) {
            particle.update(deltaTime);
        }

        // Remove dead particles
        particles.erase(std::remove_if(particles.begin(), particles.end(),
                        [](const Particle& p) { return !p.isAlive(); }), particles.end());
    }
}

void BashTower::draw(sf::RenderWindow& window) {
    // Draw the tower
    window.draw(shape);
    window.draw(text);
    for (const auto& bullet : bullets) {
        bullet->draw(window);
    }

    // Draw the stomp effect
    if (showStompEffect) {
        stompEffect.setFillColor(sf::Color(0, 0, 0, static_cast<sf::Uint8>(stompEffectAlpha)));
        window.draw(stompEffect);
    }

    if (PARTICLES_ON) {
        // Draw particles
        for (const auto& particle : particles) {
            particle.draw(window);
        }
    }
}

float BashTower::getChanceToStun() const {
    return BASIC_STUN_CHANCE + STUN_CHANCE_PER_LEVEL * currentLevel;
}

void BashTower::createParticles(sf::Vector2f position) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_real_distribution<float> dis(-1.0f, 1.0f);

    for (int i = 0; i < 50; ++i) {
        std::cout << dis(gen) << std::endl;
        sf::Vector2f velocity( range / attackCooldown * dis(gen) * 3 , range / attackCooldown * dis(gen) * 3); // Random velocity
        particles.emplace_back(position, velocity, attackCooldown/4);
    }
}
