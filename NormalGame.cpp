#include <iostream>
#include <cmath>
#include <unordered_map>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include "Minesweeper.hpp"


class Normal 
{
private:
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

    void drawStats(SDL_Renderer* renderer, const Minesweeper& game, TTF_Font* font, int startTick) 
    {
        // Draw logo at top
        SDL_Texture* logo = IMG_LoadTexture(renderer, "textures/minesweeperlablogo.png");
        SDL_Rect logo_rect{WIDTH/4, 0, 365, 100};
        SDL_RenderCopy(renderer, logo, NULL, &logo_rect);
        SDL_DestroyTexture(logo);
        
        // Draw flag
        SDL_Texture* flag = IMG_LoadTexture(renderer, "textures/graysquareflag.png");
        SDL_Rect flag_rect{WIDTH/2-50, HEIGHT-75, 50, 50};
        SDL_RenderCopy(renderer, flag, NULL, &flag_rect);
        SDL_DestroyTexture(flag);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_Rect outline{WIDTH/2-50, HEIGHT-75, 50, 50};
        SDL_RenderDrawRect(renderer, &outline);

        // Draw flag count
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_Rect textBg{WIDTH/2, HEIGHT-75, 50, 50};
        SDL_RenderFillRect(renderer, &textBg);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_Rect textBgOutline{WIDTH/2, HEIGHT-75, 50, 50};
        SDL_RenderDrawRect(renderer, &textBgOutline);
        SDL_Surface* flagCount = TTF_RenderText_Solid(font, std::to_string(game.getFlags()).c_str(), {0, 0, 0});
        SDL_Texture* countTexture = SDL_CreateTextureFromSurface(renderer, flagCount);
        SDL_Rect countRect;
        if (game.getFlags() > 9) {
            countRect = SDL_Rect{WIDTH/2+2, HEIGHT-75, 50, 50};
        }
        else {
            countRect = SDL_Rect{WIDTH/2+14, HEIGHT-75, 25, 50};
        }
        SDL_RenderCopy(renderer, countTexture, NULL, &countRect);
        SDL_FreeSurface(flagCount);
        SDL_DestroyTexture(countTexture);

        // Draw time
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_Rect timeRectBg{100, HEIGHT-75, 200, 50};
        SDL_RenderFillRect(renderer, &timeRectBg);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_Rect timeRectOutline{100, HEIGHT-75, 200, 50};
        SDL_RenderDrawRect(renderer, &timeRectOutline);
        SDL_Texture* timeLogo = IMG_LoadTexture(renderer, "textures/timer.png");
        SDL_Rect timeLogoRect{100, HEIGHT-75, 50, 50};
        SDL_RenderCopy(renderer, timeLogo, NULL, &timeLogoRect);
        SDL_DestroyTexture(timeLogo);
        if (startTick == 0) {
            // Draw 0
            SDL_Surface* timer = TTF_RenderText_Solid(font, "0", {0, 0, 0});
            SDL_Texture* timerTexture = SDL_CreateTextureFromSurface(renderer, timer);
            SDL_Rect timerRect{164, HEIGHT-75, 25, 50};
            SDL_RenderCopy(renderer, timerTexture, NULL, &timerRect);
            SDL_FreeSurface(timer);
            SDL_DestroyTexture(timerTexture);
        }
        else {
            // Draw (getTicks()-startTick)/1000
            int currentTime = (SDL_GetTicks64()-startTick)/1000;
            SDL_Rect timerRect;
            if (currentTime > 9) {
                timerRect = SDL_Rect{164, HEIGHT-75, 50, 50};
            }
            else if (currentTime > 99) {
                timerRect = SDL_Rect{164, HEIGHT-75, 75, 50};
            }
            else if (currentTime > 999) {
                timerRect = SDL_Rect{164, HEIGHT-75, 100, 50};
            }
            else {
                timerRect = SDL_Rect{164, HEIGHT-75, 25, 50};
            }
            SDL_Surface* timer = TTF_RenderText_Solid(font, std::to_string(currentTime).c_str(), {0, 0, 0});
            SDL_Texture* timerTexture = SDL_CreateTextureFromSurface(renderer, timer);
            SDL_RenderCopy(renderer, timerTexture, NULL, &timerRect);
            SDL_FreeSurface(timer);
            SDL_DestroyTexture(timerTexture);
        }

        // Draw high score
        // ...
    }


    void drawStatsGameOver(SDL_Renderer* renderer, TTF_Font* font, int time) 
    {
        // Draw clock
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_Rect timeRectBg{100, HEIGHT-75, 200, 50};
        SDL_RenderFillRect(renderer, &timeRectBg);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_Rect timeRectOutline{100, HEIGHT-75, 200, 50};
        SDL_RenderDrawRect(renderer, &timeRectOutline);
        SDL_Texture* timeLogo = IMG_LoadTexture(renderer, "textures/timer.png");
        SDL_Rect timeLogoRect{100, HEIGHT-75, 50, 50};
        SDL_RenderCopy(renderer, timeLogo, NULL, &timeLogoRect);
        SDL_DestroyTexture(timeLogo);

        // Draw time
        SDL_Surface* timer = TTF_RenderText_Solid(font, std::to_string(time).c_str(), {0, 0, 0});
        SDL_Texture* timerTexture = SDL_CreateTextureFromSurface(renderer, timer);
        SDL_Rect timerRect;
        if (time > 9) {
            timerRect = SDL_Rect{164, HEIGHT-75, 50, 50};
        }
        else if (time > 99) {
            timerRect = SDL_Rect{164, HEIGHT-75, 75, 50};
        }
        else if (time > 999) {
            timerRect = SDL_Rect{164, HEIGHT-75, 100, 50};
        }
        else {
            timerRect = SDL_Rect{164, HEIGHT-75, 25, 50};
        }
        SDL_RenderCopy(renderer, timerTexture, NULL, &timerRect);
        SDL_FreeSurface(timer);
        SDL_DestroyTexture(timerTexture);

        // Draw high score
        // ...
    }


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

public:
    // Returns "CLOSE" if user closed game, "NEW" if user chose new game, "BACK" if user returns to menu
    std::string runNormal(SDL_Window* window, SDL_Renderer* renderer, SDL_Event windowEvent, TTF_Font* font)
    {
        int add = 0;
        unsigned rows = 8;
        unsigned cols = 12;
        unsigned bombs = 12;
        Minesweeper game{rows, cols, bombs};
        bool gameInProgress = true;
        int startTick = 0;
        bool win = false;
        int finishingTime = 0;

        while (true)
        {
            while (SDL_PollEvent(&windowEvent)) {
                if (windowEvent.type == SDL_QUIT)
                {
                    return "CLOSE"; // Signal to close window in main.cpp
                }
                else if (windowEvent.type == SDL_MOUSEBUTTONDOWN) {
                    if (gameInProgress) {
                        // Make move (or flag) on cell if mouse is in grid
                        if (windowEvent.button.button == SDL_BUTTON_LEFT) {
                            if ( (100 <= windowEvent.motion.x && windowEvent.motion.x <= 700) && (100 <= windowEvent.motion.y && windowEvent.motion.y <= 500) ) {
                                auto coords = getCellCoords(windowEvent.motion.x, windowEvent.motion.y);

                                if (startTick == 0) {
                                    startTick = SDL_GetTicks64(); // Start recording the time
                                }

                                if (!game.move(coords.first, coords.second)) {
                                    if (startTick != 0) {
                                        finishingTime = (SDL_GetTicks64()-startTick)/1000;
                                    }
                                    gameInProgress = false;
                                }
                                else if (game.checkWin()) {
                                    if (startTick != 0) {
                                        finishingTime = (SDL_GetTicks64()-startTick)/1000;
                                    }
                                    gameInProgress = false;
                                    win = true;
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
                    else if ( (200 <= windowEvent.motion.x && windowEvent.motion.x <= 600) && (250 <= windowEvent.motion.y && windowEvent.motion.y <= 400) ) {
                        return "NEW"; // Start new game
                    }
                    else if ( (0 <= windowEvent.motion.x && windowEvent.motion.x <= 100) && (0 <= windowEvent.motion.y && windowEvent.motion.y <= 100) ) {
                        return "BACK"; // Return to menu
                    }
                }
            }
            SDL_RenderClear(renderer);

            drawBackground(renderer, add); // Checkerboard

            // ---------------------------------------
            if (gameInProgress) {
                SDL_Texture* shadow = IMG_LoadTexture(renderer, "textures/boardshadow.png");
                SDL_RenderCopy(renderer, shadow, NULL, NULL);
                SDL_DestroyTexture(shadow);
                drawGameBoard(renderer, game.getBoard());
                
                drawStats(renderer, game, font, startTick);
            }
            else {
                if (win) {
                    SDL_Texture* shadow = IMG_LoadTexture(renderer, "textures/boardshadow.png");
                    SDL_RenderCopy(renderer, shadow, NULL, NULL);
                    SDL_DestroyTexture(shadow);

                    drawGameBoardAll(renderer, game.getBoard());

                    SDL_Texture* overlay = IMG_LoadTexture(renderer, "textures/minesweeperwon.png");
                    SDL_RenderCopy(renderer, overlay, NULL, NULL);
                    SDL_DestroyTexture(overlay);

                    drawStatsGameOver(renderer, font, finishingTime);
                }
                else {
                    SDL_Texture* shadow = IMG_LoadTexture(renderer, "textures/boardshadow.png");
                    SDL_RenderCopy(renderer, shadow, NULL, NULL);
                    SDL_DestroyTexture(shadow);

                    drawGameBoardAll(renderer, game.getBoard());

                    SDL_Texture* overlay = IMG_LoadTexture(renderer, "textures/minesweeperlost.png");
                    SDL_RenderCopy(renderer, overlay, NULL, NULL);
                    SDL_DestroyTexture(overlay);

                    drawStatsGameOver(renderer, font, finishingTime);
                }
            }
            // ---------------------------------------

            SDL_RenderPresent(renderer);

            if (add == -100) {
                add = -1;
            }
            else {
                add--; // Moving background
            }
        }
    }
};

