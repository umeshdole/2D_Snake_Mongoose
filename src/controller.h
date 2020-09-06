#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "rivals.h"
#include "snake.h"

class Controller {
 public:
  // get key inputs
  void HandleInput(bool &running, Snake &snake) const;

 private:
  // respond to key inputs
  void ChangeDirection(Snake &snake, Snake::Direction input,
                       Snake::Direction opposite) const;
};

#endif