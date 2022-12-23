// TutorialGame.cpp -- Julian Zulfikar, 2022
// ------------------------------------------------------
// Tutorial gamemode definitions.

#include "TutorialGame.hpp"


Tutorial::Tutorial(SDL_Window* initWindow, SDL_Renderer* initRenderer, SDL_Event initEvent, TTF_Font* initFont, sqlite3* initDB)
: window{initWindow}, renderer{initRenderer}, windowEvent{initEvent}, font{initFont}, scoresDB{initDB}
{
    shadow = IMG_LoadTexture(renderer, "textures/boardshadow.png");
    square = IMG_LoadTexture(renderer, "textures/graysquare.png");
    flag = IMG_LoadTexture(renderer, "textures/graysquareflag.png");
    bomb = IMG_LoadTexture(renderer, "textures/graysquarebomb.png");
    logo = IMG_LoadTexture(renderer, "textures/minesweeperlablogo.png");
}


Tutorial::~Tutorial()
{
    SDL_DestroyTexture(shadow);
    SDL_DestroyTexture(square);
    SDL_DestroyTexture(flag);
    SDL_DestroyTexture(bomb);
    SDL_DestroyTexture(logo);
}


std::string Tutorial::runTutorial()
{
    int add = 0;
    int currentPic = 1;
    SDL_Texture* buttonOverlaySlideshow = IMG_LoadTexture(renderer, "textures/tutorialbuttonhover.png");
    SDL_Texture* tutorialOne = IMG_LoadTexture(renderer, "textures/tutorial1.png");
    SDL_Texture* tutorialTwo = IMG_LoadTexture(renderer, "textures/tutorial2.png");
    SDL_Texture* tutorialThree = IMG_LoadTexture(renderer, "textures/tutorial3.png");
    SDL_Texture* tutorialFour = IMG_LoadTexture(renderer, "textures/tutorial4.png");
    bool hoveringSlideshow = false;
    // Draw tutorial slideshow
    while (currentPic < 5) 
    {
        while (SDL_PollEvent(&windowEvent)) {
            if (windowEvent.type == SDL_QUIT)
            {
                return "CLOSE"; // Signal to close window in main.cpp
            }
            else if (windowEvent.type == SDL_MOUSEBUTTONDOWN)
            {
                if ( (325 <= windowEvent.motion.x && windowEvent.motion.x <= 475) && (450 <= windowEvent.motion.y && windowEvent.motion.y <= 500) ) {
                    currentPic++;
                }
            }
            else if (windowEvent.type == SDL_MOUSEMOTION)
            {
                if ( (325 <= windowEvent.motion.x && windowEvent.motion.x <= 475) && (450 <= windowEvent.motion.y && windowEvent.motion.y <= 500) ) {
                    hoveringSlideshow = true;
                }
                else {
                    hoveringSlideshow = false;
                }
            }
        }

        SDL_RenderClear(renderer);
        drawBackground(add); // Checkerboard
        switch (currentPic)
        {
        case 1:
            SDL_RenderCopy(renderer, tutorialOne, NULL, NULL);
            break;
        case 2:
            SDL_RenderCopy(renderer, tutorialTwo, NULL, NULL);
            break;
        case 3:
            SDL_RenderCopy(renderer, tutorialThree, NULL, NULL);
            break;
        case 4:
            SDL_RenderCopy(renderer, tutorialFour, NULL, NULL);
            break;
        }
        if (hoveringSlideshow)
        {
            SDL_RenderCopy(renderer, buttonOverlaySlideshow, NULL, NULL);
        }
        SDL_RenderPresent(renderer);

        if (add == -100) {
            add = -1;
        }
        else {
            add--; // Moving background
        }
        SDL_Delay(50);
    }
    SDL_DestroyTexture(buttonOverlaySlideshow);
    SDL_DestroyTexture(tutorialOne);
    SDL_DestroyTexture(tutorialTwo);
    SDL_DestroyTexture(tutorialThree);
    SDL_DestroyTexture(tutorialFour);

    SDL_Texture* nextMove = IMG_LoadTexture(renderer, "textures/tutorialnextmove.png");
    SDL_Texture* nextMoveHover = IMG_LoadTexture(renderer, "textures/tutorialnextmovehover.png");
    bool gameInProgress = true;
    bool hover = false;
    while (true)
    {
        while (SDL_PollEvent(&windowEvent)) {
            if (windowEvent.type == SDL_QUIT)
            {
                return "CLOSE"; // Signal to close window in main.cpp
            }
            else if (windowEvent.type == SDL_MOUSEBUTTONDOWN) {
                if (gameInProgress) {
                    // // Make move (or flag) on cell if mouse is in grid
                    // if (windowEvent.button.button == SDL_BUTTON_LEFT) {
                    //     if ( (100 <= windowEvent.motion.x && windowEvent.motion.x <= 700) && (100 <= windowEvent.motion.y && windowEvent.motion.y <= 500) ) {
                    //         auto coords = getCellCoords(windowEvent.motion.x, windowEvent.motion.y);

                    //         if (!game.move(coords.first, coords.second)) {
                    //             gameInProgress = false;
                    //         }
                    //         else if (game.checkWin()) {
                    //             gameInProgress = false;
                    //         }
                    //     }
                    // }
                    // else if (windowEvent.button.button == SDL_BUTTON_RIGHT) {
                    //     if ( (100 <= windowEvent.motion.x && windowEvent.motion.x <= 700) && (100 <= windowEvent.motion.y && windowEvent.motion.y <= 500) ) {
                    //         auto coords = getCellCoords(windowEvent.motion.x, windowEvent.motion.y);

                    //         if (game.findFlag(coords.first, coords.second)) {
                    //             game.removeFlag(coords.first, coords.second);
                    //         }
                    //         else {
                    //             game.flag(coords.first, coords.second);
                    //         }
                    //     }
                    // }
                    if ( (325 <= windowEvent.motion.x && windowEvent.motion.x <= 475) && (525 <= windowEvent.motion.y && windowEvent.motion.y <= 575) ) {
                        // Clicked NEXT MOVE
                        // ...
                    }
                }
                else if ( (325 <= windowEvent.motion.x && windowEvent.motion.x <= 475) && (450 <= windowEvent.motion.y && windowEvent.motion.y <= 500) ) {
                    return "BACK"; // Return to menu
                }
            }
            else if (windowEvent.type == SDL_MOUSEMOTION) {
                if (!gameInProgress) {
                    if ( (325 <= windowEvent.motion.x && windowEvent.motion.x <= 475) && (450 <= windowEvent.motion.y && windowEvent.motion.y <= 500) ) {
                        // Hovering over HOME
                        hover = true;
                    }
                    else {
                        hover = false;
                    }
                }
                else {
                    if ( (325 <= windowEvent.motion.x && windowEvent.motion.x <= 475) && (525 <= windowEvent.motion.y && windowEvent.motion.y <= 575) ) {
                        // Hovering over NEXT MOVE
                        hover = true;
                    }
                    else {
                        hover = false;
                    }
                }
            }
        }
        SDL_RenderClear(renderer);
        drawBackground(add); // Checkerboard
        SDL_Rect logo_rect{WIDTH/4, 0, 365, 100}; // Draw logo at top
        SDL_RenderCopy(renderer, logo, NULL, &logo_rect);

        // ---------------------------------------
        if (gameInProgress) {
            SDL_RenderCopy(renderer, shadow, NULL, NULL);
            drawGameBoard(game.getBoard());
            SDL_RenderCopy(renderer, nextMove, NULL, NULL);
            if (hover) {
                SDL_RenderCopy(renderer, nextMoveHover, NULL, NULL);
            }
        }
        else {
            SDL_RenderCopy(renderer, shadow, NULL, NULL);
            drawGameBoardAll(game.getBoard());

            SDL_Texture* overlay = IMG_LoadTexture(renderer, "textures/tutorialend.png");
            SDL_RenderCopy(renderer, overlay, NULL, NULL);
            SDL_DestroyTexture(overlay);
            if (hover) {
                SDL_Texture* hover = IMG_LoadTexture(renderer, "textures/tutorialbuttonhover.png");
                SDL_RenderCopy(renderer, hover, NULL, NULL);
                SDL_DestroyTexture(hover);
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
    SDL_DestroyTexture(nextMove);
    SDL_DestroyTexture(nextMoveHover);
}


// ------------------------------------------------------------


void Tutorial::drawGameBoardAll(const std::vector<std::vector<Minesweeper::Cell>>& board) 
{
    int size = 50;
    int start_x = 100;
    int x = start_x;
    int y = 100;

    for (auto& row : board) {
        for (auto& cell : row) {
            if (cell.bomb) {
                // Draw gray square with bomb
                SDL_Rect square_rect{x, y, size, size};
                SDL_RenderCopy(renderer, bomb, NULL, &square_rect);

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


void Tutorial::drawGameBoard(const std::vector<std::vector<Minesweeper::Cell>>& board) 
{
    int size = 50;
    int start_x = 100;
    int x = start_x;
    int y = 100;

    for (auto& row : board) {
        for (auto& cell : row) {
            if (!cell.revealed && !cell.flag) {
                // Draw gray square
                SDL_Rect square_rect{x, y, size, size};
                SDL_RenderCopy(renderer, square, NULL, &square_rect);

                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                SDL_Rect outline{x, y, size, size};
                SDL_RenderDrawRect(renderer, &outline);
            }
            else if (!cell.revealed && cell.flag) {
                // Draw gray square with flag
                SDL_Rect square_rect{x, y, size, size};
                SDL_RenderCopy(renderer, flag, NULL, &square_rect);

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


void Tutorial::drawBackground(int add)
{
    // int size = 50; // For scaling purposes
    int size = 25;
    int x = 0+add;
    int y = 0;
    bool dark_blue = true;

    for (int i=0; i<40; ++i) {
        for (int j=0; j<40; ++j) {
            if (dark_blue) {
                SDL_SetRenderDrawColor(renderer, 73, 94, 209, 255);
                dark_blue = false;
            }
            else {
                SDL_SetRenderDrawColor(renderer, 81, 103, 205, 255);
                dark_blue = true;
            }

            SDL_Rect rect{x, y, size, size};
            SDL_RenderFillRect(renderer, &rect);
            x += size;
        }
        x = 0+add;
        y += size;
        dark_blue = !dark_blue;
    }
}


std::pair<unsigned, unsigned> Tutorial::getCellCoords(const int& x, const int& y)
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
