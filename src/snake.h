#ifndef SNAKE_H
#define SNAKE_H

#include <cmath>
#include "rivals.h"

// declar Snake class, with snake being one of the rivals
class Snake : public Rivals
{
 public:
  // default constructor
  Snake(int grid_width, int grid_height)
      : grid_width(grid_width),
        grid_height(grid_height),
        head_x(grid_width / 2),
        head_y(grid_height / 2) {}
  // default destructor
  ~Snake() {}

  Direction direction = Direction::kRight; // default direction

  float speed{0.1f};           // default speed of movement
  int size{1};                 // default size of snake body
  bool alive{true};            // is snake dead or alive
  float head_x;                // x coordinate of snake head
  float head_y;                // y coordinate of snake head
  std::vector<SDL_Point> body; // vector of snake body cells

  bool BodyCell(int x, int y) override; // grow snake body when asked

  // main function to update snake head & body based on user input or direction
  void Update(std::shared_ptr<SDL_Point> rival_head, std::shared_ptr<std::vector<SDL_Point>> rival_body) override;

 private:
  // sub function to update snake head x & y coordinates
  void UpdateHead() override;

  // sub function to update snake body and check if alive
  void UpdateBody(SDL_Point &curr_cell, SDL_Point &prev_cell,
                  std::shared_ptr<SDL_Point> rival_head, std::shared_ptr<std::vector<SDL_Point>> rival_body) override;

  int grid_width;  // pixel grid width
  int grid_height; // pixel grid height
};

#endif