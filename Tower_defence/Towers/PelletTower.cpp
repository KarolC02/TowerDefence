#include "PelletTower.hpp"
#include "DEFINE.hpp"

PelletTower::PelletTower(sf::Vector2f position)
    : Tower(position, PELLET_TOWER_COLOR, VERY_SLOW, GRID_CELL_SIZE * 4, "P", 10, 5) {
        info = "Pellet Tower";
        upgrades = {
            TowerUpgrade("Pellet Tower 2", VERY_SLOW, GRID_CELL_SIZE * 4, 20, 5),
            TowerUpgrade("Pellet Tower 3", VERY_SLOW, GRID_CELL_SIZE * 4, 40, 10),
            TowerUpgrade("Pellet Tower 4", VERY_SLOW, GRID_CELL_SIZE * 4, 80, 20),
            TowerUpgrade("Pellet Tower 5", VERY_SLOW, GRID_CELL_SIZE * 4, 160, 40),
            TowerUpgrade("Sniper Tower", VERY_SLOW, GRID_CELL_SIZE * 4 * 3, 400, 120)
        };
}

//void PelletTower::update(float deltaTime) {
//    // PelletTower's specific update logic
//    Tower::resetCooldown(); // Example of calling base class method
//}

