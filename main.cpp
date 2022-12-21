// main.cpp -- Julian Zulfikar, 2022
// ------------------------------------------------------
// SDL (GUI) implementation of Minesweeper Lab.

#include <iostream>
#include <unordered_map>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <SQLite/sqlite3.h>
#include "SQLHelpers.hpp"

#include "Minesweeper.hpp"
#include "NormalGame.hpp"


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


// Draws green checkerboard with add offset
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


// Runs game starting from menu, calls gamemodes when necessary
int main(int argc, char *argv[])
{
    sqlite3* scoresDB;
    sqlite3_open("scoresdev.db", &scoresDB); // DEVELOPER MODE
    // sqlite3_open("scores.db", &scoresDB); // LOCAL PLAYER

    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window* window = SDL_CreateWindow("Minesweeper Lab", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_ALLOW_HIGHDPI);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    TTF_Init();
    TTF_Font* Pixelated = TTF_OpenFont("pixelated.ttf", 64);

    SDL_Texture* menu = IMG_LoadTexture(renderer, "textures/minesweeperlabmenu.png");
    SDL_Texture* normalHover = IMG_LoadTexture(renderer, "textures/menuselectnormal.png");
    SDL_Texture* tutorialHover = IMG_LoadTexture(renderer, "textures/menuselecttutorial.png");
    SDL_Texture* threeHover = IMG_LoadTexture(renderer, "textures/menuselect3x3.png");
    SDL_Texture* timedHover = IMG_LoadTexture(renderer, "textures/menuselecttimed.png");
    int hover = 0;

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
            if (windowEvent.type == SDL_MOUSEBUTTONDOWN) {

                // User selects a gamemode
                if ( (450 <= windowEvent.motion.x && windowEvent.motion.x <= 650) && (150 <= windowEvent.motion.y && windowEvent.motion.y <= 300) ) {
                    // Normal
                    bool running_normal = true;
                    while (running_normal) {
                        Normal game(window, renderer, windowEvent, Pixelated, scoresDB);
                        std::string choice = game.runNormal();
                        if (choice == "CLOSE") {
                            running = false;
                            running_normal = false;
                        }
                        else if (choice == "BACK") {
                            running_normal = false;
                        }
                    }
                }
                else if ( (150 <= windowEvent.motion.x && windowEvent.motion.x <= 350) && (150 <= windowEvent.motion.y && windowEvent.motion.y <= 300) ) {
                    // Tutorial
                }
                else if ( (150 <= windowEvent.motion.x && windowEvent.motion.x <= 350) && (350 <= windowEvent.motion.y && windowEvent.motion.y <= 500) ) {
                    // 3x3
                }
                else if ( (450 <= windowEvent.motion.x && windowEvent.motion.x <= 650) && (350 <= windowEvent.motion.y && windowEvent.motion.y <= 500) ) {
                    // Timed
                }

            }
            if (windowEvent.type == SDL_MOUSEMOTION) {

                // User hovers over a gamemode
                if ( (450 <= windowEvent.motion.x && windowEvent.motion.x <= 650) && (150 <= windowEvent.motion.y && windowEvent.motion.y <= 300) ) {
                    // Hovering over Normal
                    hover = 1;
                }
                else if ( (150 <= windowEvent.motion.x && windowEvent.motion.x <= 350) && (150 <= windowEvent.motion.y && windowEvent.motion.y <= 300) ) {
                    // Hovering over Tutorial
                    hover = 2;
                }
                else if ( (150 <= windowEvent.motion.x && windowEvent.motion.x <= 350) && (350 <= windowEvent.motion.y && windowEvent.motion.y <= 500) ) {
                    // Hovering over 3x3
                    hover = 3;
                }
                else if ( (450 <= windowEvent.motion.x && windowEvent.motion.x <= 650) && (350 <= windowEvent.motion.y && windowEvent.motion.y <= 500) ) {
                    // Hovering over Timed
                    hover = 4;
                }
                else {
                    hover = 0;
                }

            }
        }
        if (!running) {
            break; // User closed window in gamemode
        }

        // ---------------------------------------
        // Render menu
        // ---------------------------------------

        SDL_RenderClear(renderer);

        // Checkerboard and menu
        drawBackground(renderer, add);
        SDL_RenderCopy(renderer, menu, NULL, NULL);

        switch (hover)
        {
        case 1:
            SDL_RenderCopy(renderer, normalHover, NULL, NULL);
            break;
        case 2:
            SDL_RenderCopy(renderer, tutorialHover, NULL, NULL);
            break;
        case 3:
            SDL_RenderCopy(renderer, threeHover, NULL, NULL);
            break;
        case 4:
            SDL_RenderCopy(renderer, timedHover, NULL, NULL);
            break;   
        default:
            break;
        }

        SDL_RenderPresent(renderer);

        // Moving background
        if (add == -100) {
            add = -1;
        }
        else {
            add--;
        }
        SDL_Delay(50);
    }
    
    // Clean up
    TTF_CloseFont(Pixelated);
    TTF_Quit();

    SDL_DestroyTexture(normalHover);
    SDL_DestroyTexture(tutorialHover);
    SDL_DestroyTexture(threeHover);
    SDL_DestroyTexture(timedHover);

    SDL_DestroyTexture(menu);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    sqlite3_close(scoresDB);
    
    return 0;
}

