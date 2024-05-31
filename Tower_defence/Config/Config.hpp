#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <string>
#include <SFML/Graphics.hpp>
#include "/Users/karol/desktop/Tower_defence/External_Libraries/json.hpp"

class Config {
public:
    static Config& getInstance(const std::string& filename = "");

    // Deleted to prevent copying and assignment
    Config(const Config&) = delete;
    Config& operator=(const Config&) = delete;

    // Getters for configuration parameters
//    int getStartX() const { return startX; }
//    int getStartY() const { return startY; }
//    int getEndX() const { return endX; }
//    int getEndY() const { return endY; }
//    float getVerySlow() const { return VERY_SLOW; }
//    float getSlow() const { return SLOW; }
//    float getAverage() const { return AVERAGE; }
//    float getFast() const { return FAST; }
//    float getVeryFast() const { return VERY_FAST; }
//    int getStartMoney() const { return START_MONEY; }
    float getBulletSpeed() const { return BULLET_SPEED; }
    float getEnemySpeed() const { return ENEMY_SPEED; }

private:
    Config(const std::string& filename);

    void loadConfig(const std::string& filename);

//    // Configuration parameters
//    int startX;
//    int startY;
//    int endX;
//    int endY;
//    float VERY_SLOW;
//    float SLOW;
//    float AVERAGE;
//    float FAST;
//    float VERY_FAST;
//    int START_MONEY;
    float BULLET_SPEED;
    float ENEMY_SPEED;
};

#endif // CONFIG_HPP
