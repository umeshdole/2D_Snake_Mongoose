#ifndef RENDERER_H
#define RENDERER_H

#include "rivals.h"
#include "mongoose.h"
#include "snake.h"

class Renderer {
 public:
  // constructor
  Renderer(const std::size_t screen_width, const std::size_t screen_height,
           const std::size_t grid_width, const std::size_t grid_height);
  // copy constructor
  Renderer(const Renderer &source);
  // copy assignment
  Renderer &operator=(const Renderer &source);
  // move constructor
  Renderer(Renderer &&source);
  // move assignment
  Renderer &operator=(Renderer &&source);
  // destructor
  ~Renderer();

  // render game
  void Render(Mongoose const &mongoose, Snake const &snake, SDL_Point const &food);

  // display score
  void UpdateWindowTitle(int score, int fps);

 private:
  int count1{0}, count2{0};
  SDL_Window* sdl_window;
  SDL_Renderer* sdl_renderer;

  std::size_t screen_width;
  std::size_t screen_height;
  std::size_t grid_width;
  std::size_t grid_height;

  // clone current window
  void sdl_clonewindow(SDL_Window* sdl_window, SDL_Renderer* sdl_renderer, const Renderer &source);  
};

#endif