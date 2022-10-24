#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "Cell.hpp"

Cell::Cell()
    :posx(0), posy(0), width(0), height(0){}

void Cell::create(SDL_Renderer *renderer){
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

    if(wall.r){
        SDL_RenderDrawLine(renderer, posx + width, posy, posx + width, posy + height);
    }

    if(wall.l){
        SDL_RenderDrawLine(renderer, posx, posy, posx, posy + height);
    }

    if(wall.u){
        SDL_RenderDrawLine(renderer, posx, posy, posx + width, posy);
    }

    if(wall.d){
        SDL_RenderDrawLine(renderer, posx, posy + height, posx + width, posy + height);
    }
}

int Cell::clicked(int mouse_x, int mouse_y){
    if(mouse_x >= posx && mouse_x <= posx + width){
        if(mouse_y >= posy && mouse_y <= posy + height){
            return 1;
        }
    }

    return 0;
}
