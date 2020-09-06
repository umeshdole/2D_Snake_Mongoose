#ifndef RIVALS_H
#define RIVALS_H

#include <vector>
#include <iostream>
#include <memory>
#include <thread>
#include <future>
#include <mutex>
#include <condition_variable>
#include "SDL.h"

// declare global mutex and condition variable
extern std::mutex game_mutex;
extern std::condition_variable game_condition;
extern bool isRivalHeadUpdated;

// declare class interface for game rivals
class Rivals
{
    public:
        Rivals() {};         // default constructor;
        virtual ~Rivals() {} // default destructor

        enum class Direction { kUp, kDown, kLeft, kRight };

        Direction direction = Direction::kUp; // default direction

        float speed{0.1f};       // default speed of movement
        int size{1};             // default size of rival body
        bool alive{true};        // is rival dead or alive
        bool growing{false};     // is rival growing or not
        float head_x{0.0};       // x coordinate of rival head
        float head_y{0.0};       // y cooridnate of rival head
        std::vector<float> body; // vector of rival body cells

        void GrowBody();         // grow rival body when asked

        virtual bool BodyCell(int x, int y) = 0; // check if cell is on rival body

        // main function to update rival head & body based on user input or direction
        virtual void Update(std::shared_ptr<SDL_Point> rival_head, std::shared_ptr<std::vector<SDL_Point>> rival_body) = 0;
    private:
        // sub function to update rival head x & y coordinates
        virtual void UpdateHead() = 0;

        // sub function to update rival body and check if alive
        virtual void UpdateBody(SDL_Point &curr_cell, SDL_Point &prev_cell, 
                                std::shared_ptr<SDL_Point> rival_head, std::shared_ptr<std::vector<SDL_Point>> rival_body) = 0;

        int grid_width;  // pixel grid width
        int grid_height; // pixel grid height
};

#endif