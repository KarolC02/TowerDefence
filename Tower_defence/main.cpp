#include "Game.hpp"
#include <iostream>
#include <memory> // For std::unique_ptr
#include "Config.hpp"
#include "ResourcePath.hpp"
int main() {
    try {
        Config& config = Config::getInstance(resourcePath() + "config.json"); // Load configuration from JSON file
        Game game;
        game.Run();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }
    std::unique_ptr<Game> game = std::make_unique<Game>();
    game->Run();
    return EXIT_SUCCESS;
}
