#include "snake.h"
#include <iostream>

void Snake::Update(std::shared_ptr<SDL_Point> rival_head, std::shared_ptr<std::vector<SDL_Point>> rival_body)
{
  // lock the game mutex instance for the scope of this function
  std::lock_guard<std::mutex> snake_lock(game_mutex);
  
  // capture the head's cell before updating
  SDL_Point prev_cell {static_cast<int>(head_x),static_cast<int>(head_y)};

  // update head
  UpdateHead();

  // capture the head's cell after updating
  SDL_Point curr_cell {static_cast<int>(head_x),static_cast<int>(head_y)};
    
  // update the body cells if rival head cell has moved to a new cell
  if (curr_cell.x != prev_cell.x || curr_cell.y != prev_cell.y) 
    UpdateBody(curr_cell, prev_cell, rival_head, rival_body);

  // update the rival head and body with Snake head and body
  rival_head->x = curr_cell.x;
  rival_head->y = curr_cell.y;
  *(rival_body.get()) = body;

  // set shared variable of game mutex instance to true
  isRivalHeadUpdated = true;

  // notify game condition variable that condition has been fulfilled
  game_condition.notify_one();
}

void Snake::UpdateHead()
{
  switch (direction) {
    case Direction::kUp:
      head_y -= speed;
      break;

    case Direction::kDown:
      head_y += speed;
      break;

    case Direction::kLeft:
      head_x -= speed;
      break;

    case Direction::kRight:
      head_x += speed;
      break;
  }

  // Wrap the Snake around to the beginning if going off of the screen.
  head_x = fmod(head_x + grid_width, grid_width);
  head_y = fmod(head_y + grid_height, grid_height);
}

void Snake::UpdateBody(SDL_Point &curr_head_cell, SDL_Point &prev_head_cell, 
                       std::shared_ptr<SDL_Point> rival_head, std::shared_ptr<std::vector<SDL_Point>> rival_body)
{
  // Add previous head location to vector
  body.push_back(prev_head_cell);

  if (!growing) {
    // Remove the tail from the vector.
    body.erase(body.begin());
  } else {
    growing = false;
    size++;
  }

  // Check if the snake has died.
  if (rival_head->x == curr_head_cell.x && rival_head->y == curr_head_cell.y)
    alive = false;
  for (auto const &item : body) {
    if (curr_head_cell.x == item.x && curr_head_cell.y == item.y) {
      alive = false;
    }
  }
}

bool Snake::BodyCell(int x, int y) {
  if (x == static_cast<int>(head_x) && y == static_cast<int>(head_y)) {
    return true;
  }
  for (auto const &item : body) {
    if (x == item.x && y == item.y) {
      return true;
    }
  }
  return false;
}