#include "renderer.h"
#include <string>

// default constructor
Renderer::Renderer(std::size_t screen_width, std::size_t screen_height,
                   std::size_t grid_width, std::size_t grid_height)
    : screen_width(screen_width), screen_height(screen_height),
      grid_width(grid_width), grid_height(grid_height) {
  // Initialize SDL
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    std::cerr << "SDL could not initialize.\n";
    std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
  }

  // Create Window
  sdl_window = SDL_CreateWindow("Snake Game", SDL_WINDOWPOS_CENTERED,
                                SDL_WINDOWPOS_CENTERED, screen_width,
                                screen_height, SDL_WINDOW_SHOWN);

  if (nullptr == sdl_window) {
    std::cerr << "Window could not be created.\n";
    std::cerr << " SDL_Error: " << SDL_GetError() << "\n";
  }

  // Create renderer
  sdl_renderer = SDL_CreateRenderer(sdl_window, -1, SDL_RENDERER_ACCELERATED);
  if (nullptr == sdl_renderer) {
    std::cerr << "Renderer could not be created.\n";
    std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
  }
}

///// BEGIN RULE OF FIVE
// copy constructor
Renderer::Renderer(const Renderer &source) 
    : screen_width(source.screen_width), screen_height(source.screen_height),
      grid_width(source.grid_width), grid_height(source.grid_height) {

  sdl_clonewindow(sdl_window, sdl_renderer, source);

}

// copy assignment operator
Renderer& Renderer::operator=(const Renderer &source) {

  if (this == &source) return *this;

  screen_width = source.screen_width;
  screen_height = source.screen_height;
  grid_width = source.grid_width;
  grid_height = source.grid_height;

  sdl_clonewindow(sdl_window, sdl_renderer, source);

  return *this;
}

// move constructor
Renderer::Renderer(Renderer &&source) 
    : screen_width(source.screen_width), screen_height(source.screen_height),
      grid_width(source.grid_width), grid_height(source.grid_height) {
  
  sdl_clonewindow(sdl_window, sdl_renderer, source);

  source.screen_width = 0;
  source.screen_height = 0;
  source.grid_width = 0;
  source.grid_height = 0;
  source.sdl_window = nullptr;
  source.sdl_renderer = nullptr;
}

// move assignment operator
Renderer& Renderer::operator=(Renderer &&source) {

  if (this == &source) return *this;

  screen_width = source.screen_width;
  screen_height = source.screen_height;
  grid_width = source.grid_width;
  grid_height = source.grid_height;

  sdl_clonewindow(sdl_window, sdl_renderer, source);

  source.screen_width = 0;
  source.screen_height = 0;
  source.grid_width = 0;
  source.grid_height = 0;
  source.sdl_window = nullptr;
  source.sdl_renderer = nullptr;

  return *this;
}

// destructor
Renderer::~Renderer() {
  SDL_DestroyWindow(sdl_window);
  SDL_DestroyRenderer(sdl_renderer);
  SDL_Quit();
}
///// END RULE OF FIVE

// render game and its elements
void Renderer::Render(Mongoose const &mongoose, Snake const &snake, SDL_Point const &food) {
  SDL_Rect block;
  block.w = screen_width / grid_width;
  block.h = screen_height / grid_height;

  // Clear screen
  SDL_SetRenderDrawColor(sdl_renderer, 0x1E, 0x1E, 0x1E, 0xFF);
  SDL_RenderClear(sdl_renderer);

  // Render food
  SDL_SetRenderDrawColor(sdl_renderer, 0x00, 0xFF, 0x00, 0xFF);
  block.x = food.x * block.w;
  block.y = food.y * block.h;
  SDL_RenderFillRect(sdl_renderer, &block);

  // Render snake's body
  SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
  for (SDL_Point const &point : snake.body) {
    block.x = point.x * block.w;
    block.y = point.y * block.h;
    SDL_RenderFillRect(sdl_renderer, &block);
  }

  // Render snake's head
  block.x = static_cast<int>(snake.head_x) * block.w;
  block.y = static_cast<int>(snake.head_y) * block.h;
  if (snake.alive) {
    SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0xCC, 0x00, 0xFF);
  } else {
    count1++;
    SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0x00, 0x00, 0xFF);
    if (count1==1) SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "GAME OVER!!!!!",
                      "Snake has been killed (SIGH)! Game is OVER... Close the window...", NULL);
  }
  SDL_RenderFillRect(sdl_renderer, &block);

  // Render mongoose's body
  SDL_SetRenderDrawColor(sdl_renderer, 0x70, 0x80, 0x90, 0xFF);
  for (SDL_Point const &point : mongoose.body) {
    block.x = point.x * block.w;
    block.y = point.y * block.h;
    SDL_RenderFillRect(sdl_renderer, &block);
  }

  // Render mongoose's head
  block.x = static_cast<int>(mongoose.head_x) * block.w;
  block.y = static_cast<int>(mongoose.head_y) * block.h;
  if (mongoose.alive) {
    SDL_SetRenderDrawColor(sdl_renderer, 0x70, 0x80, 0x90, 0xFF);
  } else {
    count2++;
    SDL_SetRenderDrawColor(sdl_renderer, 0x80, 0x00, 0x00, 0xFF);
    if (count2==1) SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Mongoose KILLED!!!", 
                      "Snake KILLED the mongoose!!! Snake can continue to EAT food and GROW...", NULL);
  }
  SDL_RenderFillRect(sdl_renderer, &block);

  // Update Screen
  SDL_RenderPresent(sdl_renderer);
}

// display score and frames per second (fps) in window title
void Renderer::UpdateWindowTitle(int score, int fps) {
  std::string title{"Snake Score: " + std::to_string(score) + " FPS: " + std::to_string(fps)};
  SDL_SetWindowTitle(sdl_window, title.c_str());
}

// clone current window with title, size and position information
void Renderer::sdl_clonewindow(SDL_Window* sdl_window, SDL_Renderer* sdl_renderer, const Renderer &source) {

  int window_x{0}, window_y{0}, window_w{0}, window_h{0};

  SDL_SetWindowTitle(sdl_window,SDL_GetWindowTitle(source.sdl_window));

  SDL_GetWindowPosition(source.sdl_window, &window_x, &window_y);
  SDL_SetWindowPosition(sdl_window, window_x, window_y);

  SDL_GetWindowSize(source.sdl_window, &window_w, &window_h);
  SDL_SetWindowSize(sdl_window, window_w, window_h);

  sdl_renderer = SDL_GetRenderer(source.sdl_window);
}