#include "DartTower.hpp"
#include "DEFINE.hpp"
#include "ExplosiveBullet.hpp"

DartTower::DartTower(sf::Vector2f position)
    : Tower(position, DART_TOWER_COLOR, VERY_SLOW, 6 * GRID_CELL_SIZE, "D", 8, 20) {
    info = "Dart Tower";
        upgrades = {
            TowerUpgrade("Dart Tower 2", VERY_SLOW, 6 * GRID_CELL_SIZE + 2/3 * GRID_CELL_SIZE, 16, 15),
            TowerUpgrade("Dart Tower 3", VERY_SLOW, 7 * GRID_CELL_SIZE + 1/3 * GRID_CELL_SIZE, 32, 35),
            TowerUpgrade("Dart Tower 4", VERY_SLOW, 8 * GRID_CELL_SIZE, 64, 60),
            TowerUpgrade("Dart Tower 5", VERY_SLOW, 8 * GRID_CELL_SIZE + 2/3 * GRID_CELL_SIZE, 128, 110),
            TowerUpgrade("ICBM Tower", VERY_SLOW, 9 * GRID_CELL_SIZE + 1/3 * GRID_CELL_SIZE, 256, 160)
        };

}

void DartTower::fireBullet(const sf::Vector2f& targetPosition, float bulletSpeed) {
    if (canAttack()) {
        float explosionRadius = 50.0f; // Define an appropriate explosion radius
        bullets.emplace_back(std::make_unique<ExplosiveBullet>(shape.getPosition(), targetPosition, bulletSpeed, damage, explosionRadius));
        resetCooldown();
    }
}

//void DartTower::update(float deltaTime) {
//    // Implementation of update specific to DartTower
//}
