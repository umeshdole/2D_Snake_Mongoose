#include <iostream>
#include <memory>
#include "controller.h"
#include "game.h"
#include "renderer.h"

int main() {
  // declare and define game rendering parameters
  constexpr std::size_t kFramesPerSecond{60};
  constexpr std::size_t kMsPerFrame{1000 / kFramesPerSecond};
  constexpr std::size_t kScreenWidth{640};
  constexpr std::size_t kScreenHeight{640};
  constexpr std::size_t kGridWidth{32};
  constexpr std::size_t kGridHeight{32};

  // declare and define game rivals head and body
  float rhead_x{0.0};
  float rhead_y{0.0};
  std::vector<SDL_Point> rbody;

  // declare and define shared pointer for game rivals head and body
  std::shared_ptr<SDL_Point> rival_head = std::make_shared<SDL_Point>();
  *rival_head = {static_cast<int>(rhead_x),static_cast<int>(rhead_y)};
  std::shared_ptr<std::vector<SDL_Point>> rival_body = std::make_shared<std::vector<SDL_Point>>(rbody);

  // declare and define SDL renderer object with screen window and pixel grid width and height
  Renderer renderer(kScreenWidth, kScreenHeight, kGridWidth, kGridHeight);

  // declare and define game controller object
  Controller controller;

  // declare and define game instance
  Game game(kGridWidth, kGridHeight);

  // start the game
  game.Run(controller, renderer, kMsPerFrame, rival_head, rival_body);

  // exit the game
  std::cout << "Game has terminated successfully!\n";
  std::cout << "Score: " << game.GetScore() << "\n";
  std::cout << "Size: " << game.GetSize() << "\n";
  
  return 0;
}