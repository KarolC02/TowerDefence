#include "BashTower.hpp"
#include "DEFINE.hpp"

BashTower::BashTower(sf::Vector2f position)
    : Tower(position, BASH_TOWER_COLOR, SLOW, 2 * GRID_CELL_SIZE + 2/3 * GRID_CELL_SIZE, "B", 10, 30) {
        info = "Bash Tower";
        upgrades = {
            TowerUpgrade("Bash Tower 2", SLOW, 2 * GRID_CELL_SIZE + 2/3 * GRID_CELL_SIZE, 20, 25),
            TowerUpgrade("Bash Tower 3", SLOW, 2 * GRID_CELL_SIZE + 2/3 * GRID_CELL_SIZE, 40, 50),
            TowerUpgrade("Bash Tower 4", SLOW, 2 * GRID_CELL_SIZE + 2/3 * GRID_CELL_SIZE, 80, 100),
            TowerUpgrade("Bash Tower 5", SLOW, 2 * GRID_CELL_SIZE + 2/3 * GRID_CELL_SIZE, 160, 185),
            TowerUpgrade("Quake Tower", SLOW, 2 * GRID_CELL_SIZE + 2/3 * GRID_CELL_SIZE, 320, 355)
        };
}

void BashTower::fireBullet(const sf::Vector2f& targetPosition, float bulletSpeed) {
    // Because of BashTower unique effect, it's logic is handled by the Arena
}

void BashTower::update(float deltaTime, const std::vector<Enemy> &enemies, const sf::RenderWindow& window) {
    if (attackCooldown > 0) {
        attackCooldown -= deltaTime;
    }
    // Because of BashTower unique effect, it's logic is handled by the Arena
}

float BashTower::getChanceToStun(){
    return BASIC_STUN_CHANCE + STUN_CHANCE_PER_LEVEL * currentLevel;
}
