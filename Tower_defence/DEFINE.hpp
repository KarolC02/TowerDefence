#pragma once

#define TOWER_SIDE_LENGTH 40
#define GRID_WIDTH 14
#define GRID_HEIGHT 12
#define GRID_CELL_SIZE (TOWER_SIDE_LENGTH / 2)


#define BULLET_SPEED 500
#define ENEMY_SPEED 2 * GRID_CELL_SIZE // około 12 sekund przejscie mapy
#define ARENA_WIDTH GRID_WIDTH * TOWER_SIDE_LENGTH  // Example width, adjust as needed
#define ARENA_HEIGHT GRID_HEIGHT * TOWER_SIDE_LENGTH // Example height, adjust as needed
#define WINDOW_WIDTH ARENA_WIDTH*2  // Example width, adjust as needed
#define WINDOW_HEIGHT ARENA_HEIGHT*2 // Example height, adjust as needed
#define UI_WIDTH 200

#define LEFT_OFFSET TOWER_SIDE_LENGTH*2// Example offsets from the left and top edges of the window
#define TOP_OFFSET TOWER_SIDE_LENGTH*4

#define PELLET_TOWER_COLOR sf::Color(105, 105, 105)    // Dim Gray
#define SWARM_TOWER_COLOR sf::Color(153, 76, 0)        // Dark Orange (for more distinction)
#define BASH_TOWER_COLOR sf::Color(139, 0, 0)          // Dark Red (Scarlet Dark Red)
#define FROST_TOWER_COLOR sf::Color(70, 130, 180)      // Steel Blue (Dark Ice Blue)
#define DART_TOWER_COLOR sf::Color(60, 179, 113)       // Medium Sea Green (for more distinction)
#define SQUIRT_TOWER_COLOR sf::Color(119, 136, 153)    // Light Slate Gray

#define BACKGROUND_FRAME_COLOR sf::Color(160, 160, 160, 200)
#define INFOBAR_COLOR sf::Color(121, 25, 87, 200)
#define TOWER_INFO_BAR_COLOR sf::Color(22, 200, 187, 200)
#define startX 0
#define startY 11
#define endX 27
#define endY 11

#define VERY_SLOW 2.5f
#define SLOW 2.0f
#define AVERAGE 1.5f
#define FAST 1.0f
#define VERT_FAST 0.5f
#define START_MONEY 80

#define INFOBAR_HEIGHT 50

#define BASIC_SLOW_FACTOR 0.5f
#define SLOW_FACTOR_PER_LEVEL 0.04f
#define STUN_TIME 1.0f

#define BASIC_STUN_CHANCE 0.1f
#define STUN_CHANCE_PER_LEVEL 0.5f

#define STUN_DURATION 1.0f
#define SPAWN_COOLDOWN 1.0f

#define VALUE 10

enum class TowerType {
    None,
    Pellet,
    Bash,
    Squirt,
    Frost,
    Swarm,
    Dart
};


/*
 
 #include "Game.hpp"
 #include "DEFINE.hpp"
 #include "ResourcePath.hpp"
 #include <iostream>
 #include "PelletTower.hpp"
 #include "BashTower.hpp"
 #include "SquirtTower.hpp"
 #include "FrostTower.hpp"
 #include "SwarmTower.hpp"
 #include "DartTower.hpp"

 Game::Game() : window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Desktop Tower Defence"),
                arena(ARENA_WIDTH, ARENA_HEIGHT, sf::Vector2f(LEFT_OFFSET, TOP_OFFSET)),
                currentState(GameState::Default), currentlyPlacing(TowerType::None) {
     if (!gameFont.loadFromFile(resourcePath() + "arial.ttf")) {
         throw std::runtime_error("Failed to load font!");
     }

     // Button setup
     {
         sf::Vector2f buttonPosition(WINDOW_WIDTH - 8 * TOWER_SIDE_LENGTH , WINDOW_HEIGHT/4);
         sf::Vector2f buttonSize(TOWER_SIDE_LENGTH, TOWER_SIDE_LENGTH);
         std::string buttonText = "P";
         pelletTowerButton = Button(buttonPosition, buttonSize, buttonText, PELLET_TOWER_COLOR);
         buttons.push_back(pelletTowerButton);
         
         sf::Vector2f buttonPosition1(WINDOW_WIDTH - 7 * TOWER_SIDE_LENGTH , WINDOW_HEIGHT/4);
         std::string buttonText1 = "SW";
         SwarmTowerButton = Button(buttonPosition1, buttonSize, buttonText1, SWARM_TOWER_COLOR);
         buttons.push_back(SwarmTowerButton);
         
         sf::Vector2f buttonPosition2(WINDOW_WIDTH - 6 * TOWER_SIDE_LENGTH , WINDOW_HEIGHT/4);
         std::string buttonText2 = "D";
         DartTowerButton = Button(buttonPosition2, buttonSize, buttonText2, DART_TOWER_COLOR);
         buttons.push_back(DartTowerButton);
         
         sf::Vector2f buttonPosition3(WINDOW_WIDTH - 5 * TOWER_SIDE_LENGTH , WINDOW_HEIGHT/4);
         std::string buttonText3 = "S";
         SquirtTowerButton = Button(buttonPosition3, buttonSize, buttonText3, SQUIRT_TOWER_COLOR);
         buttons.push_back(SquirtTowerButton);
         
         sf::Vector2f buttonPosition4(WINDOW_WIDTH - 4 * TOWER_SIDE_LENGTH , WINDOW_HEIGHT/4);
         std::string buttonText4 = "B";
         BashTowerButton = Button(buttonPosition4, buttonSize, buttonText4, BASH_TOWER_COLOR);
         buttons.push_back(BashTowerButton);
         
         sf::Vector2f buttonPosition5(WINDOW_WIDTH - 3 * TOWER_SIDE_LENGTH , WINDOW_HEIGHT/4);
         std::string buttonText5 = "F";
         FrostTowerButton = Button(buttonPosition5, buttonSize, buttonText5, FROST_TOWER_COLOR);
         buttons.push_back(FrostTowerButton);
     }

 }

 void Game::Run() {
     sf::Clock clock;
     const float timestep = 1.0f / 60.0f;
     float accumulator = 0.0f;

     while (window.isOpen()) {
         sf::Time deltaTime = clock.restart();
         accumulator += deltaTime.asSeconds();

         processEvents();

         while (accumulator >= timestep) {
             accumulator -= timestep;
             update(timestep);
         }
         handleMouseHover(sf::Mouse::getPosition(window));
         render();
     }
 }

 void Game::processEvents() {
     sf::Event event;
     while (window.pollEvent(event)) {
         switch (event.type) {
             case sf::Event::Closed:
                 window.close();
                 break;
             case sf::Event::MouseButtonPressed:
                 if (pelletTowerButton.isMouseOver(window)) {
                     std::cout << "Pellet Tower Button Clicked\n";
                     currentState = GameState::PlacingTower;
                     previewTower = std::make_unique<PelletTower>(sf::Vector2f(0, 0)); // Position will be updated on hover
                     currentlyPlacing = TowerType::Frost;
                     showPreview = true;
                 } else if (SwarmTowerButton.isMouseOver(window)) {
                     std::cout << "Swarm Tower Button Clicked\n";
                     currentState = GameState::PlacingTower;
                     previewTower = std::make_unique<SwarmTower>(sf::Vector2f(0, 0)); // Adjust the class name as necessary
                     currentlyPlacing = TowerType::Swarm;
                     showPreview = true;
                 } else if (DartTowerButton.isMouseOver(window)) {
                     std::cout << "Dart Tower Button Clicked\n";
                     currentState = GameState::PlacingTower;
                     previewTower = std::make_unique<DartTower>(sf::Vector2f(0, 0)); // Adjust the class name as necessary
                     currentlyPlacing = TowerType::Dart;
                     showPreview = true;
                 } else if (SquirtTowerButton.isMouseOver(window)) {
                     std::cout << "Squirt Tower Button Clicked\n";
                     currentState = GameState::PlacingTower;
                     previewTower = std::make_unique<SquirtTower>(sf::Vector2f(0, 0)); // Adjust the class name as necessary
                     currentlyPlacing = TowerType::Squirt;
                     showPreview = true;
                 } else if (BashTowerButton.isMouseOver(window)) {
                     std::cout << "Bash Tower Button Clicked\n";
                     currentState = GameState::PlacingTower;
                     previewTower = std::make_unique<BashTower>(sf::Vector2f(0, 0)); // Adjust the class name as necessary
                     currentlyPlacing = TowerType::Bash;
                     showPreview = true;
                 } else if (FrostTowerButton.isMouseOver(window)) {
                     std::cout << "Frost Tower Button Clicked\n";
                     currentState = GameState::PlacingTower;
                     previewTower = std::make_unique<FrostTower>(sf::Vector2f(0, 0)); // Adjust the class name as necessary
                     currentlyPlacing = TowerType::Frost;
                     showPreview = true;
                 } else {
                     handleMouseClick(sf::Mouse::getPosition(window));
                 }
                 break;

             // Handle other events as needed
         }
     }
 }

 void Game::update(float dt) {
     // Update your game state here
     arena.update(dt, window);
 }

 void Game::render() {
     window.clear(sf::Color(211, 211, 211));
     arena.draw(window);
     for (auto& button : buttons) {
         button.draw(window);
     }
     // Render other game objects here
     if (showPreview && previewTower) {
         previewTower->draw(window); // Draw the preview tower
         // Optionally, draw the tower's range
         sf::CircleShape rangeCircle(previewTower->getRange());
         rangeCircle.setFillColor(sf::Color(255, 255, 255, 50)); // Semi-transparent
         rangeCircle.setPosition(previewTower->getPosition());
         rangeCircle.setOrigin(previewTower->getRange(), previewTower->getRange()); // Center the circle on the tower
         window.draw(rangeCircle);
     }
     window.display();
 }

 void Game::setGameState(GameState newState) {
     currentState = newState;
 }

 void Game::handleMouseClick(sf::Vector2i clickPosition) {
     // Convert click position to world coordinates if needed and handle based on current game state
     sf::Vector2f worldPos = window.mapPixelToCoords(clickPosition);
     
     //    sf::Vector2f snappedPos;
     //    snappedPos.x = std::floor((worldPos.x - LEFT_OFFSET) / GRID_CELL_SIZE) * GRID_CELL_SIZE + LEFT_OFFSET;
     //    snappedPos.y = std::floor((worldPos.y - TOP_OFFSET) / GRID_CELL_SIZE) * GRID_CELL_SIZE + TOP_OFFSET;
     switch (currentState) {
         case GameState::PlacingTower: {
             std::cout << "Attempting to place a tower...\n";
             
             // Handling no tower type selected
             if (currentlyPlacing == TowerType::None) {
                 std::cerr << "ERROR: No tower type selected!\n";
                 window.close();
                 break; // Exiting after the error handling
             }
             
             // Declare a base class pointer for polymorphism
             std::unique_ptr<Tower> tower;
             
             // Decide which type of tower to create based on the currently selected type
             switch (currentlyPlacing) {
                 case TowerType::Pellet:
                     tower = std::make_unique<PelletTower>(worldPos);
                     break;
                 case TowerType::Bash:
                     // Assuming BashTower is a valid class inheriting from Tower
                     tower = std::make_unique<BashTower>(worldPos);
                     break;
                 case TowerType::Squirt:
                     tower = std::make_unique<SquirtTower>(worldPos);
                     break;
                 case TowerType::Frost:
                     tower = std::make_unique<FrostTower>(worldPos);
                     break;
                 case TowerType::Swarm:
                     tower = std::make_unique<SwarmTower>(worldPos);
                     break;
                 case TowerType::Dart:
                     // Assuming DartTower is a valid class inheriting from Tower
                     tower = std::make_unique<DartTower>(worldPos);
                     break;
                 default:
                     std::cerr << "ERROR: Unknown tower type!\n";
                     break;
             }
             
             // If a tower was successfully created, add it to the game arena or similar
             if (tower) {
                 arena.addTower(tower);
                 std::cout << "Tower placed successfully.\n";
             }
             
             break; // Break after handling the placement state
         }
             
             // Other cases for different game states can follow here..
         case GameState::Default: {
             std::cout << "Executing handleMouseClick for state: Default" << std::endl;
             // Handle default state mouse clicks
             break;
         }
         case GameState::TowerSelected: {
             std::cout << "Executing handleMouseClick for state: TowerSelected" << std::endl;
             // Handle tower selected state mouse clicks
             break;
         }
             // Add cases for other states as necessary
         default: {
             std::cout << "Mouse click received in an undefined game state." << std::endl;
             break;
         }
     }
 }

 void Game::handleMouseHover(sf::Vector2i hoverPosition) {
     if (currentState == GameState::PlacingTower) {
         sf::Vector2f worldPos = window.mapPixelToCoords(hoverPosition);
         sf::Vector2f snappedPos = arena.snapToGrid(worldPos);

         if (arena.isWithinBounds(snappedPos)) {
             if (!previewTower) {
                 previewTower = std::make_unique<PelletTower>(snappedPos);
             } else {
                 previewTower->setPosition(snappedPos);
             }
             
             // Check if the space is free and update the color accordingly
             if (arena.isSpaceFree(snappedPos)) {
                 showPreview = true;
                 previewTower->setColor(sf::Color::Green); // Default color
             } else {
                 showPreview = true;
                 previewTower->setColor(sf::Color::Red); // Indicate the space is taken
             }
         } else {
             showPreview = false;
         }
     } else {
         showPreview = false;
     }
 }






 bool Game::isFreeSpace(const sf::Vector2f& gridPosition) const {
     for (const auto& tower : towers) {
         if (std::abs(gridPosition.x - tower->getShape().getPosition().x) < GRID_CELL_SIZE * 2 &&
             std::abs(gridPosition.y - tower->getShape().getPosition().y) < GRID_CELL_SIZE * 2) {
             // This means the new tower's position is too close to an existing tower (i.e., overlapping)
             return false;
         }
     }
     return true; // No existing tower is too close
 }


 bool Game::collision(const sf::Shape& shape1, const sf::Shape& shape2) const {
     return shape1.getGlobalBounds().intersects(shape2.getGlobalBounds());
 }


 */
