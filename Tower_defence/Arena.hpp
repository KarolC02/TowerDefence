#pragma once

#include "DEFINE.hpp"
#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include "Bullet.hpp"
#include "Enemy.hpp"
#include "Tower.hpp"
#include "PathFinder.hpp"
#include "LevelManager.hpp"
#include "Level.hpp"


class Arena {
public:
    Arena(float width, float height, sf::Vector2f position);
    void update(float dt, const sf::RenderWindow& window);
    void draw(sf::RenderWindow& window) const;

    // Accessor for demonstration purposes
    std::vector<std::unique_ptr<Tower>>& getTowers();
    std::vector<Enemy>& getEnemies();
    
    bool addTower(std::unique_ptr<Tower> tower);
    sf::Vector2f snapToGrid(const sf::Vector2f& position) const;
    bool isWithinBounds(sf::Vector2f snappedPos);
    bool isSpaceFree(const sf::Vector2f& position);
    
    int getGridX( int x );
    int getGridY( int y );
    int payCheck;
    int scorePayCheck;
    int livesDebt;
    bool grid[GRID_WIDTH*2][GRID_HEIGHT*2];
    bool deleteTower(const sf::Vector2f& position);
    bool upgradeTower(const sf::Vector2f& position, int gold);
    
    void loadLevel(const Level& level);
    sf::Clock currentLevelTimer;
    int currentLevelMaxTime;
    std::vector<Level> levels;
    Level currentLevel;
    float pausedTime;
private:
    void loadNextLevel();
    void spawnEnemy(int health);
    void manageLevels();
    void spawnCurrentLevelEnemies();
    void updateTowers(float dt, const sf::RenderWindow& window);
    void updateEnemies(float dt, const sf::RenderWindow& window);
    
    sf::Clock enemySpawnTimer;
    int currentLevelIndex;
    int alreadySpawnedThisLevel;
    bool readyForNextLevel;
    
    size_t nextEnemyIndex; // To keep track of the next enemy to spawn
    bool spawnFlag; // Flag to control spawning process
    
    int random(int min, int max);
    void freeGridForDeletedTower(int gridX, int gridY);
    void printPath(const std::vector<std::pair<int, int>>& path);
    void printGrid() const;
    bool justAddedTower;
    std::vector<std::pair<int, int>> path;
    std::vector<sf::RectangleShape> pathCells;
    sf::RectangleShape background;
    sf::RectangleShape topFrame1;
    sf::RectangleShape topFrame2;
    sf::RectangleShape topFrame3;
    sf::RectangleShape bottomFrame1;
    sf::RectangleShape bottomFrame2;
    sf::RectangleShape bottomFrame3;
    std::vector<std::unique_ptr<Tower>> towers;
    std::vector<Enemy> enemies;
    sf::Clock _enemySpawner;
    sf::Clock _logTimer;
    void markPathCellsRed(const std::vector<std::pair<int, int>>& path);
    void updateGridForNewTower(int gridX, int gridY);
    void removeDeadOrOutOfBoundsEnemies(const sf::RenderWindow& window);
    sf::Vector2i convertToGridCoordinates(const sf::Vector2f& position);
    void checkBulletEnemyCollisions();
    void bashTowerAttack();
    std::vector<std::pair<int, int>> findPathBFS(const std::pair<int, int>& start, const std::pair<int, int>& end);
    
    std::vector<std::pair<int, int>> findPathAStar(const std::pair<int, int>& start, const std::pair<int, int>& end);
    void drawBackground(sf::RenderWindow& window) const;
    void drawFrames(sf::RenderWindow& window) const;
    void drawGrid(sf::RenderWindow& window) const;
    void drawTowers(sf::RenderWindow& window) const;
    void drawEnemies(sf::RenderWindow& window) const;


};

