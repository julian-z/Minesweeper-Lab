// NormalGame.hpp -- Julian Zulfikar, 2022
// ------------------------------------------------------
// Normal gamemode declarations.

#ifndef NORMAL_HPP
#define NORMAL_HPP

#include <iostream>
#include <unordered_map>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <SQLite/sqlite3.h>
#include "SQLHelpers.hpp"

#include "Minesweeper.hpp"


class Normal 
{
private:
    SDL_Texture* logo = nullptr;
    SDL_Texture* shadow = nullptr;
    SDL_Texture* square = nullptr;
    SDL_Texture* flag = nullptr;
    SDL_Texture* bomb = nullptr;
    
    SDL_Texture* backButton = nullptr;
    SDL_Texture* backButtonHover = nullptr;

    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    SDL_Event windowEvent;
    TTF_Font* font = nullptr;
    sqlite3* scoresDB = nullptr;

    const unsigned rows = 8;
    const unsigned cols = 12;
    const unsigned bombs = 12;
    Minesweeper game{rows, cols, bombs};

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

    void drawStats(int startTick);
    void drawStatsGameOver(int time);

    void drawGameBoardAll(const std::vector<std::vector<Minesweeper::Cell>>& board);
    void drawGameBoard(const std::vector<std::vector<Minesweeper::Cell>>& board);

    void drawBackground(int add);

    std::pair<unsigned, unsigned> getCellCoords(const int& x, const int& y);

public:
    // Parameters are passed in from main.cpp, so we are able to render onto the window
    Normal(SDL_Window* initWindow, SDL_Renderer* initRenderer, SDL_Event initEvent, TTF_Font* initFont, sqlite3* initDB);

    // Clean up (font, window, renderer, and database are not our responsibility)
    ~Normal();

    // Returns "CLOSE" if user closed game, "NEW" if user chose new game, "BACK" if user returns to menu
    std::string runNormal();
};


#endif

