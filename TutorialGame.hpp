// TutorialGame.hpp -- Julian Zulfikar, 2022
// ------------------------------------------------------
// Tutorial gamemode declarations.

#ifndef TUTORIAL_HPP
#define TUTORIAL_HPP

#include <iostream>
#include <unordered_map>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <SQLite/sqlite3.h>
#include "SQLHelpers.hpp"

#include "MinesweeperAI.hpp"


class Tutorial 
{
private:
    SDL_Texture* AIoutline = nullptr;

    SDL_Texture* logo = nullptr;
    SDL_Texture* shadow = nullptr;
    SDL_Texture* square = nullptr;
    SDL_Texture* flag = nullptr;
    SDL_Texture* bomb = nullptr;

    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    SDL_Event windowEvent;
    TTF_Font* font = nullptr;
    sqlite3* scoresDB = nullptr;

    const unsigned rows = 8;
    const unsigned cols = 12;
    const unsigned bombs = 7;
    MinesweeperAI game{rows, cols, bombs};

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

    void drawGameBoardAll(const std::vector<std::vector<MinesweeperAI::Cell>>& board);
    void drawGameBoard(const std::vector<std::vector<MinesweeperAI::Cell>>& board);
    void drawMove(const std::pair<unsigned, unsigned>& move);

    void drawBackground(int add);

    std::pair<unsigned, unsigned> getCellCoords(const int& x, const int& y);

public:
    // Parameters are passed in from main.cpp, so we are able to render onto the window
    Tutorial(SDL_Window* initWindow, SDL_Renderer* initRenderer, SDL_Event initEvent, TTF_Font* initFont);

    // Clean up (font, window, and renderer are not our responsibility)
    ~Tutorial();

    // Returns "BACK" once user has finished tutorial
    std::string runTutorial();
};


#endif

