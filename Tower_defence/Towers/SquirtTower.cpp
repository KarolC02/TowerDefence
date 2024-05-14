#include "SquirtTower.hpp"
#include "DEFINE.hpp"

SquirtTower::SquirtTower(sf::Vector2f position)
    : Tower(position, SQUIRT_TOWER_COLOR, FAST /* Fast attack speed */, 70, "S", 5, 15) {
        info = "Squirt Tower";
        upgrades = {
            TowerUpgrade("Squit Tower 2", FAST, 4 * GRID_CELL_SIZE + 2/3 * GRID_CELL_SIZE, 10, 12),
            TowerUpgrade("Squit Tower 3", FAST, 4 * GRID_CELL_SIZE + 2/3 * GRID_CELL_SIZE, 18, 23),
            TowerUpgrade("Squit Tower 4", FAST, 4 * GRID_CELL_SIZE + 2/3 * GRID_CELL_SIZE, 34, 35),
            TowerUpgrade("Squit Tower 5", FAST, 4 * GRID_CELL_SIZE + 2/3 * GRID_CELL_SIZE, 65, 75),
            TowerUpgrade("Typhoon Tower", FAST, 6 * GRID_CELL_SIZE, 320, 290)
        };
}

//void SquirtTower::update(float deltaTime) {
//    // Implementation of update specific to SquirtTower
//}
