#include "Config.hpp"
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <mutex>

Config& Config::getInstance(const std::string& filename) {
    static std::once_flag onceFlag;
    static Config* instance = nullptr;
    std::call_once(onceFlag, [&]() {
        instance = new Config(filename);
    });
    return *instance;
}

Config::Config(const std::string& filename) {
    if (!filename.empty()) {
        loadConfig(filename);
    }
}

void Config::loadConfig(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file: " + filename);
    }
    nlohmann::json json;
    file >> json;
    file.close();

//    startX = json.at("startX").get<int>();
//    startY = json.at("startY").get<int>();
//    endX = json.at("endX").get<int>();
//    endY = json.at("endY").get<int>();
//    VERY_SLOW = json.at("VERY_SLOW").get<float>();
//    SLOW = json.at("SLOW").get<float>();
//    AVERAGE = json.at("AVERAGE").get<float>();
//    FAST = json.at("FAST").get<float>();
//    VERY_FAST = json.at("VERY_FAST").get<float>();
//    START_MONEY = json.at("START_MONEY").get<int>();
    BULLET_SPEED = json.at("BULLET_SPEED").get<float>();
    ENEMY_SPEED = json.at("ENEMY_SPEED").get<float>();
}
