#ifndef MONGOOSE_H
#define MONGOOSE_H

#include <cmath>
#include "rivals.h"

// declare Mongoose class, with mongoose being one of the rivals
class Mongoose : public Rivals
{
    public:
        // default constructor
        Mongoose(int grid_width, int grid_height)
                : grid_width(grid_width),
                  grid_height(grid_height),
                  head_x(0.0),
                  head_y(0.0) {}
        // default destructor
        ~Mongoose() {}

        Direction direction = Direction::kRight; // default direction

        float speed{0.4f};           // default speed of movement
        int size{1};                 // default size of mongoose body
        int max_size{2};             // maximum size of mongoose body
        bool alive{true};            // is mongoose dead or alive
        float head_x;                // x coordinate of mongoose head
        float head_y;                // y coordinate of mongoose head
        std::vector<SDL_Point> body; // vector of mongoose body cells

        bool BodyCell(int x, int y) override; // grow mongoose body when asked

        // main function to update mongoose head & body based on user input or direction
        void Update(std::shared_ptr<SDL_Point> rival_head, std::shared_ptr<std::vector<SDL_Point>> rival_body) override;
    private:
        // sub function to update mongoose x & y coordinates
        void UpdateHead() override;
        // sub function to update mongoose body and check if alive
        void UpdateBody(SDL_Point &curr_cell, SDL_Point &prev_cell, 
                        std::shared_ptr<SDL_Point> rival_head, std::shared_ptr<std::vector<SDL_Point>> rival_body) override;

        int grid_width;  // pixel grid width
        int grid_height; // pixel grid height

        // buffer of cells around the food cell in x and y direction
        int buffer = (grid_width<grid_height) ? grid_width/2-1 : grid_height/2-1;
        // direction of mongoose movement
        Direction spiral = Direction::kDown;
};

#endif