// main.cpp -- Julian Zulfikar, 2022
// ------------------------------------------------------
// SDL (GUI) implementation of Minesweeper Lab.

#include <iostream>
#include <unordered_map>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include "Minesweeper.hpp"

const int WIDTH = 800, HEIGHT = 600;

std::unordered_map<unsigned, const char*> NUMBER_MAP {
    {1, "textures/1.png"},
    {2, "textures/2.png"},
    {3, "textures/3.png"},
    {4, "textures/4.png"},
    {5, "textures/5.png"},
    {6, "textures/6.png"},
    {7, "textures/7.png"},
    {8, "textures/8.png"}
};


void drawGameBoardAll(SDL_Renderer* renderer, const std::vector<std::vector<Minesweeper::Cell>>& board) 
{
    int size = 50;
    int start_x = 100;
    int x = start_x;
    int y = 100;

    for (auto& row : board) {
        for (auto& cell : row) {
            if (cell.bomb) {
                // Draw gray square with bomb
                SDL_Texture* square = IMG_LoadTexture(renderer, "textures/graysquarebomb.png");
                SDL_Rect square_rect{x, y, size, size};
                SDL_RenderCopy(renderer, square, NULL, &square_rect);
                SDL_DestroyTexture(square);

                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                SDL_Rect outline{x, y, size, size};
                SDL_RenderDrawRect(renderer, &outline);
            }
            else {
                // Draw light gray square with number (ignore if 0)
                if (cell.num != 0) {
                    SDL_SetRenderDrawColor(renderer, 192, 192, 192, 255);
                    SDL_Rect rect{x, y, size, size};
                    SDL_RenderFillRect(renderer, &rect);

                    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                    SDL_Rect outline{x, y, size, size};
                    SDL_RenderDrawRect(renderer, &outline);

                    // Draw number
                    SDL_Texture* num = IMG_LoadTexture(renderer, NUMBER_MAP[cell.num]);
                    SDL_Rect num_rect{x, y, size, size};
                    SDL_RenderCopy(renderer, num, NULL, &num_rect);
                    SDL_DestroyTexture(num);
                }
                else {
                    SDL_SetRenderDrawColor(renderer, 192, 192, 192, 255);
                    SDL_Rect rect{x, y, size, size};
                    SDL_RenderFillRect(renderer, &rect);

                    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                    SDL_Rect outline{x, y, size, size};
                    SDL_RenderDrawRect(renderer, &outline);
                }
            }
            x += size;
        }
        x = start_x;
        y += size;
    }
}


void drawGameBoard(SDL_Renderer* renderer, const std::vector<std::vector<Minesweeper::Cell>>& board) 
{
    int size = 50;
    int start_x = 100;
    int x = start_x;
    int y = 100;

    for (auto& row : board) {
        for (auto& cell : row) {
            if (!cell.revealed && !cell.flag) {
                // Draw gray square
                SDL_Texture* square = IMG_LoadTexture(renderer, "textures/graysquare.png");
                SDL_Rect square_rect{x, y, size, size};
                SDL_RenderCopy(renderer, square, NULL, &square_rect);
                SDL_DestroyTexture(square);

                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                SDL_Rect outline{x, y, size, size};
                SDL_RenderDrawRect(renderer, &outline);
            }
            else if (!cell.revealed && cell.flag) {
                // Draw gray square with flag
                SDL_Texture* square = IMG_LoadTexture(renderer, "textures/graysquareflag.png");
                SDL_Rect square_rect{x, y, size, size};
                SDL_RenderCopy(renderer, square, NULL, &square_rect);
                SDL_DestroyTexture(square);

                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                SDL_Rect outline{x, y, size, size};
                SDL_RenderDrawRect(renderer, &outline);
            }
            else {
                // Draw light gray square with number (ignore if 0)
                if (cell.num != 0) {
                    SDL_SetRenderDrawColor(renderer, 192, 192, 192, 255);
                    SDL_Rect rect{x, y, size, size};
                    SDL_RenderFillRect(renderer, &rect);

                    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                    SDL_Rect outline{x, y, size, size};
                    SDL_RenderDrawRect(renderer, &outline);

                    // Draw number
                    SDL_Texture* num = IMG_LoadTexture(renderer, NUMBER_MAP[cell.num]);
                    SDL_Rect num_rect{x, y, size, size};
                    SDL_RenderCopy(renderer, num, NULL, &num_rect);
                    SDL_DestroyTexture(num);
                }
                else {
                    SDL_SetRenderDrawColor(renderer, 192, 192, 192, 255);
                    SDL_Rect rect{x, y, size, size};
                    SDL_RenderFillRect(renderer, &rect);

                    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                    SDL_Rect outline{x, y, size, size};
                    SDL_RenderDrawRect(renderer, &outline);
                }
            }
            x += size;
        }
        x = start_x;
        y += size;
    }
}


void drawBackground(SDL_Renderer* renderer)
{
    // int size = 50;
    int size = 25;
    int x = 0;
    int y = 0;
    bool dark_green = true;

    for (int i=0; i<40; ++i) {
        for (int j=0; j<40; ++j) {
            if (dark_green) {
                SDL_SetRenderDrawColor(renderer, 162, 209, 73, 255);
                dark_green = false;
            }
            else {
                SDL_SetRenderDrawColor(renderer, 170, 215, 81, 255);
                dark_green = true;
            }

            SDL_Rect rect{x, y, size, size};
            SDL_RenderFillRect(renderer, &rect);
            x += size;
        }
        x = 0;
        y += size;
        dark_green = !dark_green;
    }
}


std::pair<unsigned, unsigned> getCellCoords(const int& x, const int& y)
{
    int cell_row = 0;
    int counter = 100;
    for (int i=0; i<8; ++i) {
        if (y-counter <= 50) {
            break;
        }
        
        counter += 50;
        cell_row++;
    }

    int cell_col = 0;
    counter = 100;
    for (int i=0; i<12; ++i) {
        if (x-counter <= 50) {
            break;
        }

        counter += 50;
        cell_col++;
    }

    return std::pair<unsigned, unsigned>{cell_row, cell_col};
}


int main(int argc, char *argv[])
{
    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_Window* window = SDL_CreateWindow("Minesweeper Lab", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_ALLOW_HIGHDPI);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL_Event windowEvent;
    bool running = true;

    unsigned rows = 8;
    unsigned cols = 12;
    unsigned bombs = 15;
    Minesweeper game{rows, cols, bombs};
    bool gameInProgress = true;

    while (running)
    {
        while (SDL_PollEvent(&windowEvent)) {
            if (windowEvent.type == SDL_QUIT)
            {
                running = false;
                break;
            }
            else if (windowEvent.type == SDL_MOUSEBUTTONDOWN) {
                if (gameInProgress) {
                    // Make move (or flag) on cell if mouse is in grid
                    if (windowEvent.button.button == SDL_BUTTON_LEFT) {
                        if ( (100 <= windowEvent.motion.x && windowEvent.motion.x <= 700) && (100 <= windowEvent.motion.y && windowEvent.motion.y <= 500) ) {
                            auto coords = getCellCoords(windowEvent.motion.x, windowEvent.motion.y);

                            if (!game.move(coords.first, coords.second)) {
                                gameInProgress = false;
                            }
                            else if (game.checkWin()) {
                                gameInProgress = false;
                            }
                        }
                    }
                    else if (windowEvent.button.button == SDL_BUTTON_RIGHT) {
                        if ( (100 <= windowEvent.motion.x && windowEvent.motion.x <= 700) && (100 <= windowEvent.motion.y && windowEvent.motion.y <= 500) ) {
                            auto coords = getCellCoords(windowEvent.motion.x, windowEvent.motion.y);

                            if (game.findFlag(coords.first, coords.second)) {
                                game.removeFlag(coords.first, coords.second);
                            }
                            else {
                                game.flag(coords.first, coords.second);
                            }
                        }
                    }
                }
            }
        }
        SDL_RenderClear(renderer);

        drawBackground(renderer); // Checkerboard

        // ---------------------------------------
        if (gameInProgress) {
            drawGameBoard(renderer, game.getBoard());
        }
        else {
            drawGameBoardAll(renderer, game.getBoard());
        }
        // ---------------------------------------

        SDL_RenderPresent(renderer);
    }
    
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    
    return 0;
}

