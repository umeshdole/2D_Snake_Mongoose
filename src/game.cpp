#include "game.h"

// define global mutex and condition variable
std::mutex game_mutex;
std::condition_variable game_condition;
bool isRivalHeadUpdated = false;

// default constructor
Game::Game(std::size_t grid_width, std::size_t grid_height)
    : mongoose(grid_width, grid_height),
      snake(grid_width, grid_height),
      engine(dev()),
      random_w(0, static_cast<int>(grid_width-1)),
      random_h(0, static_cast<int>(grid_height-1)) {
  PlaceFood();
}

// main function to run the 2D Snake-Mongoose game
void Game::Run(Controller const &controller, Renderer &renderer,
               std::size_t target_frame_duration, std::shared_ptr<SDL_Point> rival_head,
               std::shared_ptr<std::vector<SDL_Point>> rival_body) {
  Uint32 title_timestamp = SDL_GetTicks();
  Uint32 frame_start;
  Uint32 frame_end;
  Uint32 frame_duration;
  int frame_count = 0;
  bool running = true;

  while (running) {
    frame_start = SDL_GetTicks();

    // Input, Update, Render - the main game loop.
    controller.HandleInput(running, snake);
    Update(rival_head, rival_body);
    renderer.Render(mongoose, snake, food);

    frame_end = SDL_GetTicks();

    // Keep track of how long each loop through the input/update/render cycle
    // takes.
    frame_count++;
    frame_duration = frame_end - frame_start;

    // After every second, update the window title.
    if (frame_end - title_timestamp >= 1000) {
      renderer.UpdateWindowTitle(score, frame_count);
      frame_count = 0;
      title_timestamp = frame_end;
    }

    // If the time for this frame is too small (i.e. frame_duration is smaller than the target ms_per_frame), delay the loop to
    // achieve the correct frame rate.
    if (frame_duration < target_frame_duration) {
      SDL_Delay(target_frame_duration - frame_duration);
    }
  }
}

// Place new food if either rival eats it
void Game::PlaceFood() {
  int x, y;
  while (true) {
    x = random_w(engine);
    y = random_h(engine);
    
    // Check that the location is not occupied by a snake and mongoose before placing food.
    if (!snake.BodyCell(x, y) && !mongoose.BodyCell(x, y)) {
      food.x = x;
      food.y = y;
      return;
    }
  }
}

// main update function for the 2D Snake-Mongoose game
void Game::Update(std::shared_ptr<SDL_Point> rival_head, std::shared_ptr<std::vector<SDL_Point>> rival_body) {
  // if snake is dead, game is over
  if (!snake.alive) return;

  // initialize shared variable of game mutex instance to false
  isRivalHeadUpdated = false;

  ///// BEGIN ASYNCHRONOUS EXECUTION OF SNAKE OBJECT AND FUNCTIONS

  // start asynchronous execution of snake's update function
  snake_future = std::async(std::launch::async, &Snake::Update, &snake, rival_head, rival_body);
  
  // declare and define unique_lock to lock the game mutex
  std::unique_lock<std::mutex> snake_lock(game_mutex);
  
  // set wait on game condition variable and release the mutex
  game_condition.wait(snake_lock, [this]{ return isRivalHeadUpdated; });

  // type cast snake head's x & y coordiantes to integer
  int snake_x = static_cast<int>(snake.head_x);
  int snake_y = static_cast<int>(snake.head_y);

  // Check if there's food over here
  if (food.x == snake_x && food.y == snake_y) {
    score++;
    PlaceFood();
    // Grow snake
    snake.GrowBody();
    // Increase speed
    snake.speed += 0.005;
  }

  // block until future become available
  snake_future.wait();

  // unlock the game mutex instance
  game_mutex.unlock();

  ///// END ASYNCHRONOUS EXECUTION OF SNAKE OBJECT AND FUNCTIONS

  // create mongoose body with fixed size
  if (mongoose.size<mongoose.max_size)
    mongoose.GrowBody();
  
  // initialize shared variable of game mutex instance to false
  isRivalHeadUpdated = false;

  ///// BEGIN ASYNCHRONOUS EXECUTION OF MONGOOSE OBJECT AND FUNCTIONS

  // if mongoose is alive, let mongoose run around
  if (mongoose.alive) {
    
    // start asynchronous execution of mongoose's update function
    mongoose_future = std::async(std::launch::async, &Mongoose::Update, &mongoose, rival_head, rival_body);

    // declare and define unique_lock to lock the game mutex
    std::unique_lock<std::mutex> mongoose_lock(game_mutex);

    // set wait on game condition variable and release the mutex
    game_condition.wait(mongoose_lock, [this]{ return isRivalHeadUpdated; });
  }

  // type cast mongoose head's x & y coordinates to integer
  int mongoose_x = static_cast<int>(mongoose.head_x);
  int mongoose_y = static_cast<int>(mongoose.head_y);

  // If mongoose eats food, replace
  if (food.x == mongoose_x && food.y == mongoose_y)
    PlaceFood();

  // if future is available, block until future is available
  if (mongoose_future.valid())
    mongoose_future.wait();
  
  ///// END ASYNCHRONOUS EXECUTION OF MONGOOSE OBJECT AND FUNCTIONS
}


int Game::GetScore() const { return score; }     // get game score
int Game::GetSize() const { return snake.size; } // get snake's size