#include "FrostTower.hpp"
#include "DEFINE.hpp"

FrostTower::FrostTower(sf::Vector2f position)
: Tower(position, FROST_TOWER_COLOR, SLOW, 3 * GRID_CELL_SIZE + 1/3 * GRID_CELL_SIZE, "F", 10, 50) {
    info = "Frost Tower";
    upgrades = {
        TowerUpgrade("Frost Tower 2", SLOW, 3 * GRID_CELL_SIZE + 1/3 * GRID_CELL_SIZE, 15, 25),
        TowerUpgrade("Frost Tower 3", SLOW, 3 * GRID_CELL_SIZE + 1/3 * GRID_CELL_SIZE, 20, 25),
        TowerUpgrade("Frost Tower 4", SLOW, 3 * GRID_CELL_SIZE + 1/3 * GRID_CELL_SIZE, 25, 25),
        TowerUpgrade("Frost Tower 5", SLOW, 3 * GRID_CELL_SIZE + 1/3 * GRID_CELL_SIZE, 30, 25),
        TowerUpgrade("Billzard Tower", SLOW, 5 * GRID_CELL_SIZE, 40, 50)
    };
    
}
void FrostTower::fireBullet(const Enemy* targetEnemy, float bulletSpeed) {
    if (canAttack()) {
        float slowFactor = BASIC_SLOW_FACTOR + SLOW_FACTOR_PER_LEVEL * currentLevel;
        float areaEffectRadius = 50.f;
        bullets.emplace_back(std::make_unique<FrostBullet>(shape.getPosition(), targetEnemy, bulletSpeed, damage, slowFactor, areaEffectRadius, 2.0f));
        resetCooldown();
    }
}

//void FrostTower::update(float deltaTime) {
//    // Implementation of update specific to FrostTower
//}
