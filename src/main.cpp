#define SDL_MAIN_HANDLED

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <time.h>
#include <vector>

#include "Cell.hpp"

const int GRID[] {20, 10};
const int GRID_ROWS = GRID[1];
const int GRID_COLS = GRID[0];
const int CELL_WIDTH = 40;
const int MARGIN = 20;

const int SCREEN_WIDTH = CELL_WIDTH * GRID_COLS + MARGIN * 2;
const int SCREEN_HEIGHT = CELL_WIDTH * GRID_ROWS + MARGIN * 2;

const SDL_Color BLACK = {0, 0, 0, 255};
const SDL_Color WHITE = {255, 255, 255, 255};
const SDL_Color CELL_VISITING = {50, 123, 168, 255};
const SDL_Color CELL_START_WALL = {0, 255, 0, 255};
const SDL_Color CELL_END_WALL = {255, 0, 0, 255};
const SDL_Color BACKGROUND = {97, 52, 235, 255};
const SDL_Color SELECTED = {100, 100, 200, 255};

const int FPS = 60;
const int FRAME_DELAY = 1000 / FPS;

void quit_wall(Cell (*grid)[GRID_COLS], int index_y, int index_x, char *wall){
    if(wall == "right"){
        grid[index_y][index_x].wall.r = false;
        grid[index_y][index_x + 1].wall.l = false;
    }

    else if(wall == "left"){
        grid[index_y][index_x].wall.l = false;
        grid[index_y][index_x - 1].wall.r = false;
    }

    else if(wall == "up"){
        grid[index_y][index_x].wall.u = false;
        grid[index_y - 1][index_x].wall.d = false;
    }

    else if(wall == "down"){
        grid[index_y][index_x].wall.d = false;
        grid[index_y + 1][index_x].wall.u = false;
    }
    
}

void draw_visiting_cell(SDL_Renderer *renderer, Cell (*grid)[GRID_COLS], SDL_Color color){
    for(int i = 0; i < GRID_ROWS; ++i){
        for(int k = 0; k < GRID_COLS; ++k){
            if(grid[i][k].visiting){
                SDL_SetRenderDrawColor(renderer, CELL_VISITING.r, CELL_VISITING.g, CELL_VISITING.b, CELL_VISITING.a);
                SDL_Rect visiting {MARGIN + CELL_WIDTH * k + 1, MARGIN + CELL_WIDTH * i + 1, CELL_WIDTH - 1, CELL_WIDTH - 1};
                SDL_RenderFillRect(renderer, &visiting);
            }
        }
    }
}

bool all_visited(Cell (*grid)[GRID_COLS]){
    for(int i = 0; i < GRID_ROWS; ++i){
        for(int k = 0; k < GRID_COLS; ++k){
            if(!grid[i][k].visited){
                return false;
            }
        }
    }

    return true;
}

void add_visiting(Cell (*grid)[GRID_COLS], bool (*visiting)[GRID_COLS]){
    for(int i = 0; i < GRID_ROWS; ++i){
        for(int k = 0; k < GRID_COLS; ++k){
            if(grid[i][k].visiting){
                visiting[i][k] = true;
            }
        }
    }
}

void add_visiting(Cell (*grid)[GRID_COLS], int cell_y, int cell_x){
    if(!grid[cell_y + 1][cell_x].visited && cell_y + 1 < GRID_ROWS){
        grid[cell_y + 1][cell_x].visiting = true;
        grid[cell_y + 1][cell_x].parent_cell.y = cell_y;
        grid[cell_y + 1][cell_x].parent_cell.x = cell_x;
    }

    if(!grid[cell_y - 1][cell_x].visited && cell_y - 1 >= 0){
        grid[cell_y - 1][cell_x].visiting = true;
        grid[cell_y - 1][cell_x].parent_cell.y = cell_y;
        grid[cell_y - 1][cell_x].parent_cell.x = cell_x;
    }

    if(!grid[cell_y][cell_x + 1].visited && cell_x + 1 < GRID_COLS){
        grid[cell_y][cell_x + 1].visiting = true;
        grid[cell_y][cell_x + 1].parent_cell.y = cell_y;
        grid[cell_y][cell_x + 1].parent_cell.x = cell_x;
    }

    if(!grid[cell_y][cell_x - 1].visited && cell_x - 1 >= 0){
        grid[cell_y][cell_x - 1].visiting = true;
        grid[cell_y][cell_x - 1].parent_cell.y = cell_y;
        grid[cell_y][cell_x - 1].parent_cell.x = cell_x;
    }
}

int main(int argv, char *args[]){
    if(SDL_Init(SDL_INIT_VIDEO) < 0){
        return 1;
    }

    SDL_Window *win = SDL_CreateWindow("Maze generation algorithm", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

    if(win == NULL){
        std::cout << "Window could not be created. SDL_Error: " << SDL_GetError() << std::endl;
        return -1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_SOFTWARE);

    if(TTF_Init()){
        std::cout << "TTF could not be initialized. TTF_Error" << SDL_GetError() << std::endl;
        return -1;
    }

    SDL_SetWindowAlwaysOnTop(win, SDL_TRUE);
    SDL_SetWindowBordered(win, SDL_FALSE);
    SDL_SetWindowOpacity(win, 0.8);
    SDL_ShowCursor(SDL_DISABLE);

    srand(time(NULL));
    int cell_x = rand() % GRID_COLS;
    int cell_y = rand() % GRID_ROWS;

    Cell cell_start, cell_end;

    int rand_num = 0;
    if(rand_num == 0){
        cell_start.posx = MARGIN + 5;
        cell_start.posy = (rand() % GRID_ROWS) * CELL_WIDTH + MARGIN + 5;

        cell_end.posx = MARGIN + (GRID_COLS - 1) * CELL_WIDTH + 5;
        cell_end.posy = (rand() % GRID_ROWS) * CELL_WIDTH + MARGIN + 5;
    }

    else{
        cell_start.posx = (rand() % GRID_COLS) * CELL_WIDTH + MARGIN + 5;
        cell_start.posy = MARGIN + 5;

        cell_end.posx = (rand() % GRID_COLS) * CELL_WIDTH + MARGIN + 5;
        cell_end.posy = MARGIN + (GRID_ROWS - 1) * CELL_WIDTH + 5;
    }

    cell_start.width = cell_end.width = cell_start.height = cell_end.height = CELL_WIDTH - 10;
    
    cell_start.color = CELL_START_WALL;
    cell_end.color = CELL_END_WALL;

    Cell grid[GRID_ROWS][GRID_COLS];

    for(int i = 0; i < GRID_ROWS; ++i){
        for(int k = 0; k < GRID_COLS; ++k){
            grid[i][k].posx = MARGIN + CELL_WIDTH * k;
            grid[i][k].posy = MARGIN + CELL_WIDTH * i;
            grid[i][k].width = grid[i][k].height = CELL_WIDTH;
            grid[i][k].color = WHITE;
        }
    }

    grid[cell_y][cell_x].visited = true;

    const Uint8 *keyboard_state = SDL_GetKeyboardState(NULL);

    SDL_Event event;

    Uint32 frame_start;
    int frame_time;

    int num;

    SDL_Point mouse;
    SDL_Point parent_cell;

    SDL_Rect mouse_rect;

    mouse_rect.x = cell_start.posx + cell_start.width / 2 - 10;
    mouse_rect.y = cell_start.posy + cell_start.height / 2 - 10;
    mouse_rect.w = 20;
    mouse_rect.h = 20;

    std::vector<std::vector<int>> visiting_list;

    add_visiting(grid, cell_y, cell_x);

    bool running = true;
    while(running){
        frame_start = SDL_GetTicks();

        SDL_GetMouseState(&mouse.x, &mouse.y);

        SDL_SetRenderDrawColor(renderer, BACKGROUND.r, BACKGROUND.g, BACKGROUND.b, BACKGROUND.a);
        SDL_RenderClear(renderer);
        
        if(!all_visited(grid)){
            draw_visiting_cell(renderer, grid, CELL_VISITING);

            visiting_list.erase(visiting_list.begin(), visiting_list.end());
            for(int i = 0; i < GRID_ROWS; ++i){
                for(int k = 0; k < GRID_COLS; ++k){
                    if(grid[i][k].visiting && !grid[i][k].visited){
                        visiting_list.push_back({i, k});
                    }
                }
            }

            num = rand() % visiting_list.size();

            grid[visiting_list[num][0]][visiting_list[num][1]].visited = true;
            grid[visiting_list[num][0]][visiting_list[num][1]].visiting = false;

            cell_y = grid[visiting_list[num][0]][visiting_list[num][1]].parent_cell.y;
            cell_x = grid[visiting_list[num][0]][visiting_list[num][1]].parent_cell.x;

            if(visiting_list[num][0] - cell_y == -1){
                quit_wall(grid, cell_y, cell_x, "up");
            }

            else if(visiting_list[num][0] - cell_y == 1){
                quit_wall(grid, cell_y, cell_x, "down");
            }

            else if(visiting_list[num][1] - cell_x == -1){
                quit_wall(grid, cell_y, cell_x, "left");
            }

            else if(visiting_list[num][1] - cell_x == 1){
                quit_wall(grid, cell_y, cell_x, "right");
            }

            cell_y = visiting_list[num][0];
            cell_x = visiting_list[num][1];

            add_visiting(grid, cell_y, cell_x);
        }

        SDL_SetRenderDrawColor(renderer, WHITE.r, WHITE.g, WHITE.b, WHITE.a);

        for(int i = 0; i < GRID_ROWS; ++i){
            for(int k = 0; k < GRID_COLS; ++k){
                grid[i][k].create(renderer);
            }
        }

        if(keyboard_state[SDL_SCANCODE_ESCAPE]){
            running = false;
        }

        SDL_PollEvent(&event);

        if(event.type == SDL_QUIT){
            running = false;
        }

        SDL_RenderPresent(renderer);

        frame_time = SDL_GetTicks() - frame_start;
        if(FRAME_DELAY > frame_time){
            SDL_Delay(FRAME_DELAY - frame_time);
        }
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(win);

    SDL_Quit();

    return 0;
}
