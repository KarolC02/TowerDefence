#include "Arena.hpp"
#include <algorithm> // For std::remove_if
#include "PelletTower.hpp"
#include "BashTower.hpp"
#include <stdio.h>
#include "DEFINE.hpp"
#include <iostream>
#include <math.h>
#include <queue>
#include <unordered_map>
#include <cmath>  // For std::hypot
#include <algorithm>
#include <functional> // for std::greater
#include "ExplosiveBullet.hpp"
#include "FrostBullet.hpp"
#include "ResourcePath.hpp"
#include <cstdlib>  // For rand() and srand()
#include <ctime>    // For time()
#include <random>


Arena::Arena(float width, float height, sf::Vector2f position) : alreadySpawnedThisLevel(0), currentLevelIndex(0), readyForNextLevel(true),
    payCheck(0), scorePayCheck(0), livesDebt(0), justAddedTower(false){
    
    levels = LevelManager::loadLevels(resourcePath() + "levels.json");
    std::fill(&grid[0][0], &grid[0][0] + sizeof(grid), false);
        
    for (int i = 0; i < GRID_WIDTH * 2; ++i) {
        for (int j = 0; j < GRID_HEIGHT * 2; ++j) {
            if ((i == 0 && (j < 9 || j > 14)) ||
                (i == GRID_WIDTH * 2 - 1 && (j < 9 || j > 14)) ||
                j == 0 || j == 2 * GRID_HEIGHT - 1) {
                grid[i][j] = true;
            }
        }
    }
        
    background.setSize(sf::Vector2f(width, height));
    background.setFillColor(sf::Color(133, 187, 101));
    background.setPosition(position);
    
    topFrame1.setSize(sf::Vector2f(GRID_CELL_SIZE, 9 * GRID_CELL_SIZE));
    topFrame1.setPosition(position);
    topFrame1.setFillColor(BACKGROUND_FRAME_COLOR);
    
    topFrame2.setSize(sf::Vector2f(width - TOWER_SIDE_LENGTH,  GRID_CELL_SIZE));
    topFrame2.setPosition(position + sf::Vector2f(GRID_CELL_SIZE, 0) );
    topFrame2.setFillColor(BACKGROUND_FRAME_COLOR);
    
    topFrame3.setSize(sf::Vector2f(GRID_CELL_SIZE, 9 * GRID_CELL_SIZE));
    topFrame3.setPosition(position + sf::Vector2f(width - GRID_CELL_SIZE,0));
    topFrame3.setFillColor(BACKGROUND_FRAME_COLOR);
    
    bottomFrame1.setSize(sf::Vector2f(GRID_CELL_SIZE, 9 * GRID_CELL_SIZE) );
    bottomFrame1.setPosition(position + sf::Vector2f(0, 15 * GRID_CELL_SIZE) );
    bottomFrame1.setFillColor(BACKGROUND_FRAME_COLOR);
    
    bottomFrame2.setSize(sf::Vector2f(width - TOWER_SIDE_LENGTH, GRID_CELL_SIZE ));
    bottomFrame2.setPosition(position + sf::Vector2f(GRID_CELL_SIZE, 23 * GRID_CELL_SIZE) );
    bottomFrame2.setFillColor(BACKGROUND_FRAME_COLOR);
    
    bottomFrame3.setSize(sf::Vector2f(GRID_CELL_SIZE, 9 * GRID_CELL_SIZE) );
    bottomFrame3.setPosition(position + sf::Vector2f(width - GRID_CELL_SIZE, 15 * GRID_CELL_SIZE) );
    bottomFrame3.setFillColor(BACKGROUND_FRAME_COLOR);

}

void Arena::loadNextLevel(){
    pausedTime = 0;
    currentLevel = levels.at(currentLevelIndex);
    currentLevelIndex++;
    readyForNextLevel = false;
    alreadySpawnedThisLevel = 0;
}

void Arena::update(float dt, const sf::RenderWindow& window) {
    
    manageLevels();
    spawnCurrentLevelEnemies();
    updateTowers(dt, window);
    checkBulletEnemyCollisions();
    updateEnemies(dt, window);
    // bashTowerAttack();
    justAddedTower = false;
}

void Arena::updateTowers(float dt, const sf::RenderWindow &window){
    for (auto& tower : towers) {
        tower->update(dt, enemies, window);
    }
}

void Arena::updateEnemies(float dt, const sf::RenderWindow &window){
    for (auto& enemy : enemies) {
        sf::Vector2i currentPosition = enemy.getGridPosition();
        if ( !enemy.isImmune() && ((enemy.gotPath == false) || justAddedTower )){
            std::pair<int, int> start = {enemy.getGridPosition().x, enemy.getGridPosition().y};  // Convert to std::pair<int, int>
            std::pair<int, int> end = {endX, endY};  // Define the target position
            enemy.setPath(findPathBFS(start, end));  // Set the path for the enemy
            enemy.gotPath = true;
        }
        enemy.update(dt);
    }
    removeDeadOrOutOfBoundsEnemies(window);
    
}
void Arena::spawnCurrentLevelEnemies(){
    if( currentLevel.numberOfEnemies > alreadySpawnedThisLevel ){
        if( currentLevel.group == true ){
            while( currentLevel.numberOfEnemies > alreadySpawnedThisLevel ){
                std::cout << "HIP HOP" << std::endl;
                spawnEnemy(currentLevel.health);
                alreadySpawnedThisLevel++;
            }
        }else{
            if( enemySpawnTimer.getElapsedTime().asSeconds() > SPAWN_COOLDOWN ){
                spawnEnemy(currentLevel.health);
                alreadySpawnedThisLevel++;
                enemySpawnTimer.restart();
            }
        }
    }
}
void Arena::manageLevels(){
    if (readyForNextLevel){
        currentLevelTimer.restart();
        enemySpawnTimer.restart();
        loadNextLevel();
        std::cout << "Level number: "<< currentLevel.levelNumber << std::endl;
        std::cout << "Enemy Health: "<< currentLevel.health << std::endl;
        std::cout << "NO enemies: "<< currentLevel.numberOfEnemies << std::endl;
        std::cout << "group?: "<< currentLevel.group << std::endl;
        std::cout << "Duration in Sec: "<< currentLevel.maxDuration << std::endl;
        std::cout << "Spawn: "<< currentLevel.spawn << std::endl;
    }
    if( currentLevelTimer.getElapsedTime().asSeconds() - pausedTime > currentLevel.maxDuration ){
        readyForNextLevel = true;
    }
}

void Arena::draw(sf::RenderWindow& window) const {
    drawBackground(window);
    // drawGrid(window);
    drawTowers(window);
    drawEnemies(window);
}

void Arena::drawTowers(sf::RenderWindow &window) const{
    for (const auto& tower : towers) {
        tower->draw(window);
    }
}

void Arena::drawEnemies(sf::RenderWindow &window) const{
    for (const auto& enemy : enemies) {
        enemy.draw(window);
    }
}

int Arena::random(int min, int max) {
    // Simple random number generation between min and max (inclusive)
    return min + rand() % ((max - min) + 1);
}

void Arena::drawBackground(sf::RenderWindow& window) const {
    window.draw(background);
    window.draw(topFrame1);
    window.draw(topFrame2);
    window.draw(topFrame3);
    window.draw(bottomFrame1);
    window.draw(bottomFrame2);
    window.draw(bottomFrame3);
}

void Arena::drawGrid(sf::RenderWindow &window) const{
    for (int x = 0; x <= GRID_WIDTH * 2 - 2; x++) {
        sf::Vertex line[] = {
            sf::Vertex(sf::Vector2f(x * GRID_CELL_SIZE + LEFT_OFFSET + GRID_CELL_SIZE, TOP_OFFSET + GRID_CELL_SIZE)),
            sf::Vertex(sf::Vector2f(x * GRID_CELL_SIZE + LEFT_OFFSET + GRID_CELL_SIZE, TOP_OFFSET + GRID_HEIGHT * 2 * GRID_CELL_SIZE - GRID_CELL_SIZE))
        };
        line[0].color = sf::Color::Black;
        line[1].color = sf::Color::Black;
        window.draw(line, 2, sf::Lines);
    }

    // Draw horizontal grid lines
    for (int y = 0; y <= GRID_HEIGHT * 2 - 2; y++) {
        sf::Vertex line[] = {
            sf::Vertex(sf::Vector2f(LEFT_OFFSET + GRID_CELL_SIZE, y * GRID_CELL_SIZE + TOP_OFFSET + GRID_CELL_SIZE)),
            sf::Vertex(sf::Vector2f(LEFT_OFFSET + GRID_WIDTH * 2 * GRID_CELL_SIZE - GRID_CELL_SIZE, y * GRID_CELL_SIZE + TOP_OFFSET + GRID_CELL_SIZE))
        };
        line[0].color = sf::Color::Black;
        line[1].color = sf::Color::Black;
        window.draw(line, 2, sf::Lines);
    }
}

std::vector<std::unique_ptr<Tower>>& Arena::getTowers() {
    return towers;
}

std::vector<Enemy>& Arena::getEnemies() {
    return enemies;
}

bool Arena::addTower(std::unique_ptr<Tower> tower) {
    sf::Vector2f snappedPos = snapToGrid(tower->getPosition());
    if (isSpaceFree(snappedPos) && isWithinBounds(snappedPos) ) {
        tower->setPosition(snappedPos);
        int towerX = tower->getShape().getPosition().x;
        int towerY = tower->getShape().getPosition().y;
        towers.push_back(std::move(tower));
        justAddedTower = true;
        int gridX = (towerX - LEFT_OFFSET) / GRID_CELL_SIZE - 1; // Center the tower to a 2x2 block
        int gridY = (towerY - TOP_OFFSET) / GRID_CELL_SIZE - 1;

        updateGridForNewTower(gridX, gridY); // Update the grid with the new tower

        return true; // Successfully added tower
    }
    return false; // Failed to add tower
}

int Arena::getGridX( int x ){
    return (x - LEFT_OFFSET) / GRID_CELL_SIZE - 1;
}

int Arena::getGridY(int y ){
    return (y - TOP_OFFSET) / GRID_CELL_SIZE - 1;
}

sf::Vector2f Arena::snapToGrid(const sf::Vector2f& position)const{
    sf::Vector2f snappedPos;
    snappedPos.x = std::floor((position.x - LEFT_OFFSET) / GRID_CELL_SIZE) * GRID_CELL_SIZE + LEFT_OFFSET;
    snappedPos.y = std::floor((position.y - TOP_OFFSET) / GRID_CELL_SIZE) * GRID_CELL_SIZE + TOP_OFFSET;
    return snappedPos;
}

void Arena::removeDeadOrOutOfBoundsEnemies(const sf::RenderWindow& window) {
    std::vector<int> indicesToRemove;
    std::vector<Enemy> enemiesToSpawn;

    // First pass: Identify enemies to remove and collect spawn data
    for (size_t i = 0; i < enemies.size(); ++i) {
        Enemy& enemy = enemies[i];
        bool isOutOfBounds = enemy.getShape().getPosition().x < 0 ||
                             enemy.getShape().getPosition().x > LEFT_OFFSET + ARENA_WIDTH + enemy.getShape().getGlobalBounds().width ||
                             enemy.getShape().getPosition().y < 0 ||
                             enemy.getShape().getPosition().y > TOP_OFFSET + ARENA_HEIGHT + enemy.getShape().getGlobalBounds().height;

        if (isOutOfBounds) {
            livesDebt += 1;  // Increment lives debt if enemy is out of bounds
            indicesToRemove.push_back(i);  // Mark for removal
        } else if (enemy.getIsDead()) {
            std::cout << "HELLO" << std::endl;
            payCheck += enemy.getValue();  // Add enemy value to paycheck if dead

            if (enemy.spawn) {
                std::cout << "SHOULD SPAWN" << std::endl;
                // Collect new enemies to spawn using emplace_back
                enemiesToSpawn.emplace_back(enemy.getPosition() + sf::Vector2f(5, 5), enemy.getMaxHealth() / 2, VALUE, enemy.flying, false, enemy.slowImmune, enemy.fast);
                enemiesToSpawn.emplace_back(enemy.getPosition(), enemy.getMaxHealth() / 2, VALUE, enemy.flying, false, enemy.slowImmune, enemy.fast);
            }

            scorePayCheck += 1;  // Increment score
            indicesToRemove.push_back(i);  // Mark for removal
        }
    }

    // Second pass: Remove identified enemies
    // We iterate in reverse to avoid invalidating indices
    for (auto it = indicesToRemove.rbegin(); it != indicesToRemove.rend(); ++it) {
        enemies.erase(enemies.begin() + *it);
    }

    // Add new enemies using emplace_back
    for (const auto& newEnemy : enemiesToSpawn) {
        std::cout << "JUST PLACEDX AN ENEMY" << std::endl;
        enemies.emplace_back(newEnemy);
    }
}


//void Arena::removeDeadOrOutOfBoundsEnemies(const sf::RenderWindow& window) {
//    std::vector<Enemy>::iterator it = enemies.begin();
//    while (it != enemies.end()) {
//        Enemy& enemy = *it;
//        bool isOutOfBounds = enemy.getShape().getPosition().x < 0 ||
//                             enemy.getShape().getPosition().x > LEFT_OFFSET + ARENA_WIDTH + enemy.getShape().getGlobalBounds().width ||
//                             enemy.getShape().getPosition().y < 0 ||
//                             enemy.getShape().getPosition().y > TOP_OFFSET + ARENA_HEIGHT + enemy.getShape().getGlobalBounds().height;
//
//        if (isOutOfBounds) {
//            livesDebt += 1;  // Increment lives debt if enemy is out of bounds
//            it = enemies.erase(it);  // Remove the enemy and get the next iterator
//            continue;
//        }
//
//        if (enemy.getIsDead()) {
//            std::cout << "HELLO" << std::endl;
//            payCheck += enemy.getValue();  // Add enemy value to paycheck if dead
//
//            if (enemy.spawn) {
//                std::cout << "SHOULD SPAWN" << std::endl;
//                // Uncomment and modify the following lines as needed to handle the enemy spawn logic
//                // enemies.emplace_back(enemy.getPosition() + sf::Vector2f(5, 5), enemy.getHealth() / 2, VALUE, enemy.flying, enemy.spawn, enemy.slowImmune, enemy.fast);
//                // enemies.emplace_back(enemy.getPosition(), enemy.getHealth() / 2, VALUE, enemy.flying, enemy.spawn, enemy.slowImmune, enemy.fast);
//            }
//
//            scorePayCheck += 1;  // Increment score
//            it = enemies.erase(it);  // Remove the enemy and get the next iterator
//            continue;
//        }
//
//        ++it;  // Move to the next enemy
//    }
//}




//void Arena::removeDeadOrOutOfBoundsEnemies(const sf::RenderWindow& window) {
//    std::vector<Enemy>::iterator newEnd = std::remove_if(enemies.begin(), enemies.end(),
//        [this, &window](Enemy& enemy) {
//            bool isOutOfBounds = enemy.getShape().getPosition().x < 0 ||
//                                 enemy.getShape().getPosition().x > LEFT_OFFSET + ARENA_WIDTH + enemy.getShape().getGlobalBounds().width ||
//                                 enemy.getShape().getPosition().y < 0 ||
//                                 enemy.getShape().getPosition().y > TOP_OFFSET + ARENA_HEIGHT + enemy.getShape().getGlobalBounds().height;
//
//            if (isOutOfBounds) {
//                livesDebt += 1;  // Increment lives debt if enemy is out of bounds
//                return true;     // Mark for removal
//            }
//
//            if (enemy.getIsDead()) {
//                std::cout << "HELLO" << std::endl;
//                payCheck += enemy.getValue();   // Add enemy value to paycheck if dead
//                if( enemy.spawn ){
//                    std::cout << "SHOULD SPAWN" << std::endl;
//                }
//                scorePayCheck += 1;            // Increment score
//                return true;                   // Mark for removal
//            }
//
//            return false; // Keep the enemy if not dead or out of bounds
//        });
//
//    enemies.erase(newEnd, enemies.end()); // Erase all marked enemies from the vector
//}


 

bool Arena::isWithinBounds(sf::Vector2f snappedPos){
    return (snappedPos.x > LEFT_OFFSET && snappedPos.x < ARENA_WIDTH + LEFT_OFFSET
            && snappedPos.y > TOP_OFFSET && snappedPos.y < TOP_OFFSET + ARENA_HEIGHT);
}


bool Arena::isSpaceFree(const sf::Vector2f& position){
    sf::Vector2f snappedPos = snapToGrid(position);
    for (const auto& enemy : enemies) {
        if( enemy.flying ){
            continue;
        }
        sf::Vector2f enemyPos = enemy.getPosition();
        float distance = std::hypot(snappedPos.x - enemyPos.x, snappedPos.y - enemyPos.y);
        if (distance < 1.5 * GRID_CELL_SIZE) {
            return false;  // Enemy is too close
        }
    }
    int gridX = (snappedPos.x - LEFT_OFFSET) / GRID_CELL_SIZE - 1; // Adjust for center
    int gridY = (snappedPos.y - TOP_OFFSET) / GRID_CELL_SIZE - 1;
    if (gridX == 0 || gridX == endX - 1) {
        return false;
    }
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            int cellX = gridX + i;
            int cellY = gridY + j;
            if (cellX < 0 || cellX >= GRID_WIDTH * 2 || cellY < 0 || cellY >= GRID_HEIGHT * 2 || grid[cellX][cellY]) {
                return false;
            }
        }
    }

    // Temporarily place tower on grid
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            grid[gridX + i][gridY + j] = true;
        }
    }

    // Check if path is still available with the temporary tower
    bool pathExists = !findPathBFS({startX, startY}, {endX, endY}).empty();

    // Remove temporary tower from grid
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            grid[gridX + i][gridY + j] = false;
        }
    }

    if (!pathExists) {
        printf("The tower would block the path!\n");
        return false;
    }

    return true; // All checks passed, space is free
}


std::vector<std::pair<int, int>> Arena::findPathAStar(const std::pair<int, int>& start, const std::pair<int, int>& end) {
    std::vector<std::pair<int, int>> directions = {{1, 1}, {-1, 1}, {-1, -1}, {1, -1}, {1, 0}, {0, 1}, {-1, 0}, {0, -1}}; // Right, Down, Left, Up
    std::priority_queue<std::pair<int, std::pair<int, int>>,
                        std::vector<std::pair<int, std::pair<int, int>>>,
                        std::greater<std::pair<int, std::pair<int, int>>>> queue;
    std::unordered_map<int, std::pair<int, int>> cameFrom;
    std::unordered_map<int, int> costSoFar;
    std::vector<std::pair<int, int>> path;
    int cols = GRID_WIDTH * 2;

    auto toIndex = [&](int x, int y) { return y * cols + x; };
    auto manhattanDistance = [&](const std::pair<int, int>& a, const std::pair<int, int>& b) {
        return std::abs(a.first - b.first) + std::abs(a.second - b.second);
    };

    queue.push({0, start});
    cameFrom[toIndex(start.first, start.second)] = {-1, -1};
    costSoFar[toIndex(start.first, start.second)] = 0;

    while (!queue.empty()) {
        auto current = queue.top().second;
        queue.pop();

        if (current == end) {
            // Trace path backwards from end to start
            std::pair<int, int> step = end;
            while (step != start) {
                path.push_back(step);
                step = cameFrom[toIndex(step.first, step.second)];
                if (step == std::pair<int, int>{-1, -1}) break; // If reach an undefined step, stop
            }
            path.push_back(start);
            std::reverse(path.begin(), path.end());
            return path;
        }

        for (const auto& dir : directions) {
            std::pair<int, int> next = {current.first + dir.first, current.second + dir.second};
            int newCost = costSoFar[toIndex(current.first, current.second)] + 1; // Assuming uniform cost grid

            if (next.first >= 0 && next.first < GRID_WIDTH * 2 && next.second >= 0 && next.second < GRID_HEIGHT * 2) {
                if (!grid[next.first][next.second] && (cameFrom.find(toIndex(next.first, next.second)) == cameFrom.end() || newCost < costSoFar[toIndex(next.first, next.second)])) {
                    costSoFar[toIndex(next.first, next.second)] = newCost;
                    int priority = newCost + manhattanDistance(end, next);
                    queue.push({priority, next});
                    cameFrom[toIndex(next.first, next.second)] = current;
                }
            }
        }
    }
    
    return {}; // No path found if we exhaust the queue without finding 'end'
}


void Arena::updateGridForNewTower(int gridX, int gridY) {
    std::cout << "Updating grid for new tower at grid position (" << gridX << ", " << gridY << "):\n";
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            int cellX = gridX + i;
            int cellY = gridY + j;
            if (cellX >= 0 && cellX < GRID_WIDTH * 2 && cellY >= 0 && cellY < GRID_HEIGHT * 2) {
                grid[cellX][cellY] = true; // Mark the cell as occupied by the tower
                std::cout << "Marked cell (" << cellX << ", " << cellY << ") as occupied.\n";
            } else {
                std::cout << "Cell (" << cellX << ", " << cellY << ") is out of bounds and not marked.\n";
            }
        }
    }
}

bool Arena::deleteTower(const sf::Vector2f& position) {
    // First, snap the position to the grid to ensure alignment with tower positions.
    sf::Vector2f snappedPos = snapToGrid(position);

    // Iterate through the towers to find one at the snapped position.
    for (auto it = towers.begin(); it != towers.end(); ++it) {
        if ((*it)->getPosition() == snappedPos) {
            // Get the grid position of the tower to update the grid.
            int towerX = (*it)->getShape().getPosition().x;
            int towerY = (*it)->getShape().getPosition().y;
            int gridX = (towerX - LEFT_OFFSET) / GRID_CELL_SIZE - 1;
            int gridY = (towerY - TOP_OFFSET) / GRID_CELL_SIZE - 1;

            // Free up the grid cells occupied by the tower.
            freeGridForDeletedTower(gridX, gridY);

            // Remove the tower from the vector.
            payCheck += (*it)->getCost() * 0.6;
            towers.erase(it);

            // Set the flag to indicate paths need recalculation if that's a game rule.
            justAddedTower = true;  // You might rename this flag to something like `towerLayoutChanged`.

            return true; // Tower successfully deleted.
        }
    }
    return false; // No tower found at the given position.
}

bool Arena::upgradeTower(const sf::Vector2f& position, int gold) {
    // Snap the position to the grid to ensure alignment with tower positions.
    sf::Vector2f snappedPos = snapToGrid(position);

    // Iterate through the towers to find one at the snapped position.
    for (auto& tower : towers) {
        if (tower->getPosition() == snappedPos) {
            // Check if the tower can be upgraded.
            if (tower->canUpgrade()) {
                int upgradeCost = tower->getNextUpgradeCost();
                if(gold >= upgradeCost ){
                    tower->upgrade();
                    payCheck -= upgradeCost;
                    return true; // Tower successfully updated.
                }else{
                    return false;
                }
            } else {
                // Tower is at maximum level, cannot upgrade.
                return false;
            }
        }
    }
    return false; // No tower found at the given position.
}

void Arena::freeGridForDeletedTower(int gridX, int gridY) {
    std::cout << "Freeing grid cells previously occupied by a tower at (" << gridX << ", " << gridY << "):\n";
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            int cellX = gridX + i;
            int cellY = gridY + j;
            if (cellX >= 0 && cellX < GRID_WIDTH * 2 && cellY >= 0 && cellY < GRID_HEIGHT * 2) {
                grid[cellX][cellY] = false; // Mark the cell as free
                std::cout << "Freed cell (" << cellX << ", " << cellY << ").\n";
            }
        }
    }
}


void Arena::checkBulletEnemyCollisions() {
    for (auto& tower : towers) {
        auto& bullets = tower->getBullets();
        for (auto bulletIter = bullets.begin(); bulletIter != bullets.end();) {
            bool bulletDeleted = false;
            auto& bullet = *bulletIter;
            FrostBullet* frostBullet = dynamic_cast<FrostBullet*>(bullet.get());
            ExplosiveBullet* explosiveBullet = dynamic_cast<ExplosiveBullet*>(bullet.get());

            for (auto& enemy : enemies) {
                if (bullet->getShape().getGlobalBounds().intersects(enemy.getShape().getGlobalBounds())) {
                    // Check for ExplosiveBullet first
                    if (explosiveBullet) {
                        // Handle explosion effect: damage all enemies within explosion radius
                        for (auto& otherEnemy : enemies) {
                            sf::Vector2f bulletPos = bullet->getPosition();
                            sf::Vector2f enemyPos = otherEnemy.getShape().getPosition();
                            float distance = std::hypot(bulletPos.x - enemyPos.x, bulletPos.y - enemyPos.y);

                            if (distance <= explosiveBullet->getExplosionRadius()) {
                                otherEnemy.damage(explosiveBullet->getDamage());
                            }
                        }
                        bulletDeleted = true;
                    }
                    // Now check for FrostBullet
                    else if (frostBullet) {
                        // Handle frost effect: slow and damage all enemies within area effect radius
                        for (auto& otherEnemy : enemies) {
                            sf::Vector2f bulletPos = bullet->getPosition();
                            sf::Vector2f enemyPos = otherEnemy.getShape().getPosition();
                            float distance = std::hypot(bulletPos.x - enemyPos.x, bulletPos.y - enemyPos.y);

                            if (distance <= frostBullet->getAreaEffectRadius()) {
                                otherEnemy.damage(frostBullet->getDamage());
                                otherEnemy.applySlow(frostBullet->getSlowFactor(), frostBullet->getSlowDuration());
                            }
                        }
                        bulletDeleted = true;
                    }
                    // Handle normal bullets
                    else {
                        enemy.damage(bullet->getDamage());
                        bulletDeleted = true;
                    }

                    if (bulletDeleted) break;
                }
            }
            if (bulletDeleted) {
                bulletIter = bullets.erase(bulletIter);
            } else {
                ++bulletIter;
            }
        }
    }
}



std::vector<std::pair<int, int>> Arena::findPathBFS(const std::pair<int, int>& start, const std::pair<int, int>& end) {
    std::vector<std::pair<int, int>> directions = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}}; // Right, Down, Left, Up
    std::queue<std::pair<int, int>> queue;
    std::unordered_map<int, std::pair<int, int>> cameFrom;
    std::vector<std::pair<int, int>> path;
    int cols = GRID_WIDTH * 2;

    auto toIndex = [&](int x, int y) { return y * cols + x; };
    queue.push(start);
    cameFrom[toIndex(start.first, start.second)] = {-1, -1}; // Mark start position came from nowhere

    while (!queue.empty()) {
        auto current = queue.front();
        queue.pop();

        if (current == end) {
            // Trace path backwards from end to start
            std::pair<int, int> step = end;
            while (step != start) {
                path.push_back(step);
                step = cameFrom[toIndex(step.first, step.second)];
                if (step == std::pair<int, int>{-1, -1}) break; // If reach an undefined step, stop
            }
            path.push_back(start);
            std::reverse(path.begin(), path.end());
            return path;
        }

        for (const auto& dir : directions) {
            std::pair<int, int> next = {current.first + dir.first, current.second + dir.second};
            if (next.first >= 0 && next.first < GRID_WIDTH * 2 && next.second >= 0 && next.second < GRID_HEIGHT * 2) {
                if (!grid[next.first][next.second] && cameFrom.find(toIndex(next.first, next.second)) == cameFrom.end()) {
                    queue.push(next);
                    cameFrom[toIndex(next.first, next.second)] = current;
                }
            }
        }
    }
    
    return {};
}



void Arena::printGrid() const {
    std::cout << "Current Grid State:\n";
    for (int y = 0; y < GRID_HEIGHT * 2; y++) {
        for (int x = 0; x < GRID_WIDTH * 2; x++) {
            if (grid[x][y]) {
                std::cout << "X ";
            } else {
                std::cout << ". ";
            }
        }
        std::cout << "\n"; 
    }
    std::cout << std::endl;
}

void Arena::printPath(const std::vector<std::pair<int, int>>& path) {
    if (path.empty()) {
        std::cout << "No path found." << std::endl;
        return;
    }

    std::cout << "Path: ";
    for (const auto& step : path) {
        std::cout << "(" << step.first << ", " << step.second << ") -> ";
    }
    std::cout << "End" << std::endl;
}

void Arena::markPathCellsRed(const std::vector<std::pair<int, int>>& path){
    pathCells.clear();
    for (const auto& cell : path) {
        sf::RectangleShape rect(sf::Vector2f(GRID_CELL_SIZE, GRID_CELL_SIZE));
        rect.setPosition(cell.first * GRID_CELL_SIZE + LEFT_OFFSET, cell.second * GRID_CELL_SIZE + TOP_OFFSET);
        rect.setFillColor(sf::Color::Red); // Set path cells to red
        pathCells.push_back(rect);
    }
}

void Arena::spawnEnemy(int health) {
    std::random_device rd;
    std::mt19937 eng(rd());
    std::uniform_real_distribution<> distr(TOP_OFFSET + 9.5 * GRID_CELL_SIZE, TOP_OFFSET + 14.5 * GRID_CELL_SIZE);

    float y = distr(eng);
    // Assuming `Enemy` constructor takes (x, y, health)
    enemies.emplace_back( sf::Vector2f(0, y) , health, VALUE, currentLevel.flying, currentLevel.spawn, currentLevel.immune, currentLevel.fast );  // Add the new enemy to the vector
}
