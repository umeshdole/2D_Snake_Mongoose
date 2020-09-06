#include "mongoose.h"
#include <iostream>

void Mongoose::Update(std::shared_ptr<SDL_Point> rival_head, std::shared_ptr<std::vector<SDL_Point>> rival_body)
{
    // lock the game mutex instance for the scope of this function
    std::lock_guard<std::mutex> mongoose_lock(game_mutex);

    // capture the head's cell before updating
    SDL_Point prev_cell {static_cast<int>(head_x),static_cast<int>(head_y)};

    // update head
    UpdateHead();

    // capture the head's cell after updating
    SDL_Point curr_cell {static_cast<int>(head_x),static_cast<int>(head_y)};
    
    // update the body cells if rival head cell has moved to a new cell
    if (curr_cell.x != prev_cell.x || curr_cell.y != prev_cell.y)
        UpdateBody(curr_cell, prev_cell, rival_head, rival_body);

    // update rival head and body with mongoose head and body
    rival_head->x = curr_cell.x;
    rival_head->y = curr_cell.y;
    *(rival_body.get()) = body;

    // set shared variable of game mutex instance to true
    isRivalHeadUpdated = true;

    // notify game condition variable that condition has been fulfilled
    game_condition.notify_one();
}

void Mongoose::UpdateHead()
{
    switch (direction)
    {
        case Direction::kUp :
            head_y -= speed;
            if (head_y <= 0.0) {
                direction = Direction::kDown;
            } else if (head_y <= grid_height/2 - buffer) {
                direction = Direction::kRight;
            }
            break;
        case Direction::kDown :
            head_y += speed;
            if (head_y >= grid_height) {
                direction = Direction::kUp;
            } else if (head_y >= grid_height/2 + buffer) {
                direction = Direction::kLeft;
            }
            break;
        case Direction::kLeft :
            head_x -= speed;
            if (head_x <= 0.0) {
                direction = Direction::kRight;
            } else if (head_x <= grid_width/2 - buffer) {
                direction = Direction::kUp;
            }
            break;
        case Direction::kRight :
            head_x += speed;
            if (head_x >= grid_width) {
                direction = Direction::kLeft;
            } else if (head_x >= grid_width/2 + buffer) {
                direction = Direction::kDown;
            }
            break;
    }

    // Run mongoose down towards the center of the screen in a spiral
    if ((spiral==Direction::kDown) &&
        ((int(head_x)==grid_width/2-buffer && int(head_y)==grid_height/2-buffer) || 
         (int(head_x)==grid_width/2-buffer && int(head_y)==grid_height/2+buffer) ||
         (int(head_x)==grid_width/2+buffer && int(head_y)==grid_height/2-buffer) || 
         (int(head_x)==grid_width/2+buffer && int(head_y)==grid_height/2+buffer)))
    {
        buffer--;
        if (buffer<2)
            spiral = Direction::kUp;
    }

    // Run mongoose up towards the edges of the screen in a spiral
    if ((spiral==Direction::kUp) &&
        ((int(head_x)==grid_width/2-buffer && int(head_y)==grid_height/2-buffer) || 
         (int(head_x)==grid_width/2-buffer && int(head_y)==grid_height/2+buffer) ||
         (int(head_x)==grid_width/2+buffer && int(head_y)==grid_height/2-buffer) || 
         (int(head_x)==grid_width/2+buffer && int(head_y)==grid_height/2+buffer)))
    {
        buffer++;
        if (buffer>14)
            spiral = Direction::kDown;
    }
}

void Mongoose::UpdateBody(SDL_Point &curr_head_cell, SDL_Point &prev_head_cell, 
                          std::shared_ptr<SDL_Point> rival_head, std::shared_ptr<std::vector<SDL_Point>> rival_body)
{
    body.push_back(prev_head_cell);

    if (!growing && body.size()>max_size) {
        body.erase(body.begin());
    } else {
        growing = false;
        if (size<max_size) size++;
    }

    // Check if the mongoose has died.
    for (auto const &item : body) {
        if (rival_head->x == item.x && rival_head->y == item.y) {
            alive = false;
        }
    }
}

bool Mongoose::BodyCell(int x, int y) {
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