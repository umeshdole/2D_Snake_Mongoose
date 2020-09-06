#ifndef GAME_H
#define GAME_H

#include <random>
#include "controller.h"
#include "renderer.h"
#include "rivals.h"
#include "mongoose.h"
#include "snake.h"

// declare Game class to run the 2D Snake - Mongoose game
class Game {
 public:
  // default constructor
  Game(std::size_t grid_width, std::size_t grid_height);

  // main function to run the 2D Snake - Mongoose game
  void Run(Controller const &controller, Renderer &renderer,
           std::size_t target_frame_duration, std::shared_ptr<SDL_Point> rival_head,
           std::shared_ptr<std::vector<SDL_Point>> rival_body);

  int GetScore() const; // get game score
  int GetSize() const;  // get snake's size

 private:
  Mongoose mongoose; // Mongoose class object
  Snake snake;       // Snake class object
  SDL_Point food;    // food location point

  // Random Number Generator from uniform distribution
  std::random_device dev;
  std::mt19937 engine;
  std::uniform_int_distribution<int> random_w;
  std::uniform_int_distribution<int> random_h;

  int score{0};      // game score variable

  void PlaceFood();  // place new food if either rival eats it

  // main update function for the 2D Snake - Mongoose game
  void Update(std::shared_ptr<SDL_Point> rival_head, std::shared_ptr<std::vector<SDL_Point>> rival_body);

  // declare future objects for std::async return of rival threads
  std::future<void> snake_future, mongoose_future;
};

#endif