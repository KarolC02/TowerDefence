#include "SwarmTower.hpp"
#include "DEFINE.hpp"

SwarmTower::SwarmTower(sf::Vector2f position)
    : Tower(position, SWARM_TOWER_COLOR, AVERAGE /* Average attack speed */, GRID_CELL_SIZE * 4, "SW", 20, 50) {
        info = "SwarmTower";
        upgrades = {
            TowerUpgrade("Swarm  Tower 2", AVERAGE, GRID_CELL_SIZE * 4, 40, 30),
            TowerUpgrade("Swarm  Tower 3", AVERAGE, GRID_CELL_SIZE * 4 + 1/3 * GRID_CELL_SIZE, 80, 50),
            TowerUpgrade("Swarm  Tower 4", AVERAGE, GRID_CELL_SIZE * 4, 160, 75),
            TowerUpgrade("Swarm  Tower 5", AVERAGE, GRID_CELL_SIZE * 4 + 2/3 * GRID_CELL_SIZE, 320, 125),
            TowerUpgrade("Storm Tower", FAST, GRID_CELL_SIZE * 5, 480, 310)
        };
}

//void SwarmTower::update(float deltaTime) {
//    // Implementation of update specific to SwarmTower
//}
