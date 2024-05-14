#include "Game.hpp"
#include <iostream>
#include <memory> // For std::unique_ptr

int main() {
    std::unique_ptr<Game> game = std::make_unique<Game>();
    game->Run();
    return EXIT_SUCCESS;
}
