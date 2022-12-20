// main.cpp -- Julian Zulfikar, 2022
// ------------------------------------------------------
// SDL (GUI) implementation of Minesweeper Lab.

#include <iostream>
#include <cmath>
#include <unordered_map>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include "Minesweeper.hpp"
#include "NormalGame.cpp"

const int WIDTH = 800;
const int HEIGHT = 600;
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


void drawBackground(SDL_Renderer* renderer, int add)
{
    // int size = 50; // For scaling purposes
    int size = 25;
    int x = 0+add;
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
        x = 0+add;
        y += size;
        dark_green = !dark_green;
    }
}


int main(int argc, char *argv[])
{
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window* window = SDL_CreateWindow("Minesweeper Lab", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_ALLOW_HIGHDPI);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    TTF_Init();
    TTF_Font* Arial = TTF_OpenFont("pixelated.ttf", 64);

    SDL_Texture* menu = IMG_LoadTexture(renderer, "textures/minesweeperlabmenu.png");

    SDL_Event windowEvent;
    int add = 0;
    bool running = true;
    while (running)
    {
        while (SDL_PollEvent(&windowEvent)) {
            if (windowEvent.type == SDL_QUIT)
            {
                running = false;
                break;
            }
            else if (windowEvent.type == SDL_MOUSEBUTTONDOWN) {
                // User selects a gamemode
                if ( (450 <= windowEvent.motion.x && windowEvent.motion.x <= 650) && (150 <= windowEvent.motion.y && windowEvent.motion.y <= 300) ) {
                    // Check NormalGame files for documentation on return values
                    bool running_normal = true;
                    while (running_normal) {
                        Normal game;
                        std::string choice = game.runNormal(window, renderer, windowEvent, Arial);
                        if (choice == "CLOSE") {
                            running = false;
                            running_normal = false;
                        }
                        else if (choice == "BACK") {
                            running_normal = false;
                        }
                    }
                }
            }
        }
        if (!running) {
            break; // User closed window in gamemode
        }

        // ---------------------------------------

        SDL_RenderClear(renderer);

        // Checkerboard and menu
        drawBackground(renderer, add);
        SDL_RenderCopy(renderer, menu, NULL, NULL);

        SDL_RenderPresent(renderer);

        // Moving background
        if (add == -100) {
            add = -1;
        }
        else {
            add--;
        }
        SDL_Delay(100);
    }
    
    // Clean up
    TTF_CloseFont(Arial);
    TTF_Quit();
    SDL_DestroyTexture(menu);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    
    return 0;
}

