#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>

class Cell{
    public:
        Cell();

        void create(SDL_Renderer *renderer);
        int clicked(int posx, int posy);

        struct Wall {
            bool r = true;
            bool l = true;
            bool u = true;
            bool d = true;
        } wall;

        struct Parent {
            int x;
            int y;
        } parent_cell;
        

        int posx, posy;
        int width, height;

        bool visited = false;
        bool visiting = false;
        
        SDL_Color color;
};
