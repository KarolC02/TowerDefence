#include "SwarmTower.hpp"
#include "DEFINE.hpp"
#include <algorithm> // For std::min

SwarmTower::SwarmTower(sf::Vector2f position)
    : Tower(position, SWARM_TOWER_COLOR, AVERAGE /* Average attack speed */, GRID_CELL_SIZE * 4, "SW", 20, 50) {
    info = "Swarm Tower";
    upgrades = {
        TowerUpgrade("Swarm Tower 2", AVERAGE, GRID_CELL_SIZE * 4, 40, 30),
        TowerUpgrade("Swarm Tower 3", AVERAGE, GRID_CELL_SIZE * 4 + 1/3 * GRID_CELL_SIZE, 80, 50),
        TowerUpgrade("Swarm Tower 4", AVERAGE, GRID_CELL_SIZE * 4, 160, 75),
        TowerUpgrade("Swarm Tower 5", AVERAGE, GRID_CELL_SIZE * 4 + 2/3 * GRID_CELL_SIZE, 320, 125),
        TowerUpgrade("Storm Tower", FAST, GRID_CELL_SIZE * 5, 480, 310)
    };
}

void SwarmTower::fireBullet(const Enemy* targetEnemy, float bulletSpeed) {
    bullets.emplace_back(std::make_unique<Bullet>(shape.getPosition(), targetEnemy, bulletSpeed, damage));
}

void SwarmTower::update(float deltaTime, std::vector<Enemy>& enemies, const sf::RenderWindow& window) {
    if (attackCooldown > 0) {
        attackCooldown -= deltaTime;
    }

    if (canAttack()) {
        std::vector<Enemy*> targets;
        sf::Vector2f towerPos = getPosition();
        float attackRange = getRange();

        for (auto& enemy : enemies) {
            if (!enemy.isFlying()) {
                continue; // Skip non-flying enemies
            }

            sf::Vector2f enemyPos = enemy.getPosition();
            float distance = std::hypot(towerPos.x - enemyPos.x, towerPos.y - enemyPos.y);

            if (distance <= attackRange) {
                targets.push_back(&enemy);
                if (targets.size() == 4) {
                    break; // Attack up to four enemies
                }
            }
        }

        if (!targets.empty()) {
            std::cout << "FIRING " << targets.size() << " Bullets!" << std::endl;
            for (auto* target : targets) {
                fireBullet(target, bulletSpeed);
            }
            resetCooldown(); // Reset cooldown after firing bullets
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
