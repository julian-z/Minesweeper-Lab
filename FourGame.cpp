// FourGame.cpp -- Julian Zulfikar, 2022
// ------------------------------------------------------
// 4x4 gamemode definitions.

#include "FourGame.hpp"
#include <cstdlib>
#include <ctime>


Four::Four(SDL_Window* initWindow, SDL_Renderer* initRenderer, SDL_Event initEvent, TTF_Font* initFont, sqlite3* initDB)
: window{initWindow}, renderer{initRenderer}, windowEvent{initEvent}, font{initFont}, scoresDB{initDB}
{
    checkmark = IMG_LoadTexture(renderer, "textures/graysquarecheckmark.png");
    shadow = IMG_LoadTexture(renderer, "textures/boardshadowfour.png");
    square = IMG_LoadTexture(renderer, "textures/graysquare.png");
    flag = IMG_LoadTexture(renderer, "textures/graysquareflag.png");
    bomb = IMG_LoadTexture(renderer, "textures/graysquarebomb.png");
    logo = IMG_LoadTexture(renderer, "textures/minesweeperlablogo.png");

    backButton = IMG_LoadTexture(renderer, "textures/backbutton.png");
    backButtonHover = IMG_LoadTexture(renderer, "textures/backbuttonhover.png");
}


Four::~Four()
{
    SDL_DestroyTexture(checkmark);
    SDL_DestroyTexture(shadow);
    SDL_DestroyTexture(square);
    SDL_DestroyTexture(flag);
    SDL_DestroyTexture(bomb);
    SDL_DestroyTexture(logo);
    
    SDL_DestroyTexture(backButtonHover);
    SDL_DestroyTexture(backButton);
}


std::string Four::runFour()
{
    int add = 0;
    bool gameInProgress = true;
    int startTick = 0;
    int solvedCount = 0;
    int hover = 0;
    bool win = false;

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

                                SDL_RenderClear(renderer);

                                drawBackgroundLost(add); // Checkerboard
                                SDL_RenderCopy(renderer, shadow, NULL, NULL);
                                drawGameBoardAll(game.getBoard());
                                drawStats(startTick, solvedCount);
                                SDL_RenderCopy(renderer, backButton, NULL, NULL);

                                SDL_RenderPresent(renderer);
                                SDL_Delay(500);

                                // Reset 4x4, choose random bomb count from 1-4
                                srand((int)time(0));
                                int randBomb = rand() % 5;
                                if (randBomb == 0) {
                                    randBomb++;
                                }
                                game.reset(randBomb);
                                auto gameBoard = game.getBoard();

                                // Find a zero to reveal as first move
                                while (true) {
                                    for (unsigned x=0; x<4; ++x) {
                                        for (unsigned y=0; y<4; ++y) {
                                            if (gameBoard[x][y].num == 0 && !gameBoard[x][y].bomb) {
                                                game.move(x, y);
                                            }
                                        }
                                    }

                                    if (!game.checkWin()) {
                                        break;
                                    }
                                    else {
                                        game.reset(randBomb);
                                    }
                                }
                            }
                            else if (game.checkWin()) {
                                solvedCount++;

                                SDL_RenderClear(renderer);

                                drawBackgroundWin(add); // Checkerboard
                                SDL_RenderCopy(renderer, shadow, NULL, NULL);
                                drawGameBoardAll(game.getBoard());
                                drawStats(startTick, solvedCount);
                                SDL_RenderCopy(renderer, backButton, NULL, NULL);
                                
                                SDL_RenderPresent(renderer);
                                SDL_Delay(500);

                                // Reset 4x4
                                game.reset(2);
                                auto gameBoard = game.getBoard();

                                // Find a zero to reveal as first move
                                while (true) {
                                    for (unsigned x=0; x<4; ++x) {
                                        for (unsigned y=0; y<4; ++y) {
                                            if (gameBoard[x][y].num == 0 && !gameBoard[x][y].bomb) {
                                                game.move(x, y);
                                            }
                                        }
                                    }

                                    if (!game.checkWin()) {
                                        break;
                                    }
                                    else {
                                        game.reset(2);
                                    }
                                }
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

                // Back Button was clicked by user
                if ( (0 <= windowEvent.motion.x && windowEvent.motion.x <= 100) && (0 <= windowEvent.motion.y && windowEvent.motion.y <= 100) ) {
                    return "BACK"; // Return to menu
                }
            }
            else if (windowEvent.type == SDL_MOUSEMOTION) {
                if (!gameInProgress) {
                    if ( (200 <= windowEvent.motion.x && windowEvent.motion.x <= 600) && (250 <= windowEvent.motion.y && windowEvent.motion.y <= 400) ) {
                        // Hovering over new
                        hover = 1;
                    }
                    else if ( (0 <= windowEvent.motion.x && windowEvent.motion.x <= 100) && (0 <= windowEvent.motion.y && windowEvent.motion.y <= 100) ) {
                        // Hovering over back
                        hover = 2;
                    }
                    else {
                        hover = 0;
                    }
                }
                else {
                    if ( (0 <= windowEvent.motion.x && windowEvent.motion.x <= 100) && (0 <= windowEvent.motion.y && windowEvent.motion.y <= 100) ) {
                        // Hovering over back (game in progress)
                        hover = 3;
                    }
                    else {
                        hover = 0;
                    }
                }
            }
        }
        SDL_RenderClear(renderer);

        drawBackground(add); // Checkerboard

        // ---------------------------------------
        if ((SDL_GetTicks64()-startTick)/1000 == 30) {
            gameInProgress = false;

            // Select high score from database
            Result result;
            std::string sqlCommand = "SELECT four from players where name = \"local\"";
            sqlite3_exec(scoresDB, sqlCommand.c_str(), callback, &result, NULL);
            
            if (result.resultingCall == 0 || solvedCount > result.resultingCall) {
                // Update high score
                sqlCommand = "UPDATE players set four = "+std::to_string(solvedCount)+" where name=\"local\"";
                sqlite3_exec(scoresDB, sqlCommand.c_str(), callback, &result, NULL);
                win = true;
            }
        }

        if (gameInProgress) {
            SDL_RenderCopy(renderer, shadow, NULL, NULL);
            drawGameBoard(game.getBoard());
            drawStats(startTick, solvedCount);

            // Draw back button
            SDL_RenderCopy(renderer, backButton, NULL, NULL);
            if (hover == 3) {
                SDL_RenderCopy(renderer, backButtonHover, NULL, NULL);
            }
        }
        else {
            if (win) {
                SDL_RenderCopy(renderer, shadow, NULL, NULL);
                drawGameBoardAll(game.getBoard());

                SDL_Texture* overlay;
                switch (hover)
                {
                case 0:
                    overlay = IMG_LoadTexture(renderer, "textures/minesweeperwon.png");
                    break;
                case 1:
                    overlay = IMG_LoadTexture(renderer, "textures/minesweeperwonnew.png");
                    break;
                case 2:
                    overlay = IMG_LoadTexture(renderer, "textures/minesweeperwonback.png");
                    break;
                }
                
                SDL_RenderCopy(renderer, overlay, NULL, NULL);
                SDL_DestroyTexture(overlay);

                drawStatsGameOver(30, solvedCount);
            }
            else {
                SDL_RenderCopy(renderer, shadow, NULL, NULL);
                drawGameBoardAll(game.getBoard());

                SDL_Texture* overlay;
                switch (hover)
                {
                case 0:
                    overlay = IMG_LoadTexture(renderer, "textures/minesweeperlost.png");
                    break;
                case 1:
                    overlay = IMG_LoadTexture(renderer, "textures/minesweeperlostnew.png");
                    break;
                case 2:
                    overlay = IMG_LoadTexture(renderer, "textures/minesweeperlostback.png");
                    break;
                }

                SDL_RenderCopy(renderer, overlay, NULL, NULL);
                SDL_DestroyTexture(overlay);

                drawStatsGameOver(30, solvedCount);
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


// ------------------------------------------------------------


void Four::drawStats(int startTick, int solvedCount)
{
    // Draw logo at top
    SDL_Rect logo_rect{WIDTH/4, 0, 365, 100};
    SDL_RenderCopy(renderer, logo, NULL, &logo_rect);
    
    // Draw flag
    SDL_Rect flag_rect{WIDTH/2-50, HEIGHT-75, 50, 50};
    SDL_RenderCopy(renderer, flag, NULL, &flag_rect);
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

    // Draw checkmark
    SDL_Rect checkmark_rect{WIDTH/2-50, HEIGHT-150, 50, 50};
    SDL_RenderCopy(renderer, checkmark, NULL, &checkmark_rect);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_Rect checkmark_outline{WIDTH/2-50, HEIGHT-150, 50, 50};
    SDL_RenderDrawRect(renderer, &checkmark_outline);
    // Draw solvedCount
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_Rect countBg{WIDTH/2, HEIGHT-150, 50, 50};
    SDL_RenderFillRect(renderer, &countBg);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_Rect countBgOutline{WIDTH/2, HEIGHT-150, 50, 50};
    SDL_RenderDrawRect(renderer, &countBgOutline);
    SDL_Surface* solvedCountSurface = TTF_RenderText_Solid(font, std::to_string(solvedCount).c_str(), {0, 0, 0});
    SDL_Texture* solvedCountTexture = SDL_CreateTextureFromSurface(renderer, solvedCountSurface);
    SDL_Rect solvedCountRect;
    if (solvedCount > 9) {
        solvedCountRect = SDL_Rect{WIDTH/2+2, HEIGHT-150, 50, 50};
    }
    else {
        solvedCountRect = SDL_Rect{WIDTH/2+14, HEIGHT-150, 25, 50};
    }
    SDL_RenderCopy(renderer, solvedCountTexture, NULL, &solvedCountRect);
    SDL_FreeSurface(solvedCountSurface);
    SDL_DestroyTexture(solvedCountTexture);

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
            timerRect = SDL_Rect{164, HEIGHT-75, 100, 50};
        }
        else if (currentTime > 999) {
            timerRect = SDL_Rect{164, HEIGHT-75, 150, 50};
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
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_Rect hsRectBg{500, HEIGHT-75, 200, 50};
    SDL_RenderFillRect(renderer, &hsRectBg);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_Rect hsRectOutline{500, HEIGHT-75, 200, 50};
    SDL_RenderDrawRect(renderer, &hsRectOutline);
    SDL_Texture* trophy = IMG_LoadTexture(renderer, "textures/trophy.png");
    SDL_Rect trophyRect{500, HEIGHT-75, 50, 50};
    SDL_RenderCopy(renderer, trophy, NULL, &trophyRect);
    SDL_DestroyTexture(trophy);
    // Select four high score
    Result result;
    std::string sqlCommand = "SELECT four from players where name = \"local\"";
    sqlite3_exec(scoresDB, sqlCommand.c_str(), callback, &result, NULL);
    SDL_Rect hsRect;
    SDL_Surface* hsSurface;
    if (result.resultingCall == 0) {
        hsRect = SDL_Rect{564, HEIGHT-75, 75, 50};
        hsSurface = TTF_RenderText_Solid(font, "N/A", {0, 0, 0});
    }
    else if (result.resultingCall > 9) {
        hsRect = SDL_Rect{564, HEIGHT-75, 50, 50};
        hsSurface = TTF_RenderText_Solid(font, std::to_string(result.resultingCall).c_str(), {0, 0, 0});
    }
    else if (result.resultingCall > 99) {
        hsRect = SDL_Rect{564, HEIGHT-75, 100, 50};
        hsSurface = TTF_RenderText_Solid(font, std::to_string(result.resultingCall).c_str(), {0, 0, 0});
    }
    else if (result.resultingCall > 999) {
        hsRect = SDL_Rect{564, HEIGHT-75, 150, 50};
        hsSurface = TTF_RenderText_Solid(font, std::to_string(result.resultingCall).c_str(), {0, 0, 0});
    }
    else {
        hsRect = SDL_Rect{564, HEIGHT-75, 25, 50};
        hsSurface = TTF_RenderText_Solid(font, std::to_string(result.resultingCall).c_str(), {0, 0, 0});
    }
    SDL_Texture* hsTexture = SDL_CreateTextureFromSurface(renderer, hsSurface);
    SDL_RenderCopy(renderer, hsTexture, NULL, &hsRect);
    SDL_FreeSurface(hsSurface);
    SDL_DestroyTexture(hsTexture);
}


void Four::drawStatsGameOver(int time, int solvedCount) 
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
        timerRect = SDL_Rect{164, HEIGHT-75, 100, 50};
    }
    else if (time > 999) {
        timerRect = SDL_Rect{164, HEIGHT-75, 150, 50};
    }
    else {
        timerRect = SDL_Rect{164, HEIGHT-75, 25, 50};
    }
    SDL_RenderCopy(renderer, timerTexture, NULL, &timerRect);
    SDL_FreeSurface(timer);
    SDL_DestroyTexture(timerTexture);

    // Draw checkmark
    SDL_Rect checkmark_rect{WIDTH/2-50, HEIGHT-75, 50, 50};
    SDL_RenderCopy(renderer, checkmark, NULL, &checkmark_rect);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_Rect checkmark_outline{WIDTH/2-50, HEIGHT-75, 50, 50};
    SDL_RenderDrawRect(renderer, &checkmark_outline);
    // Draw solvedCount
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_Rect countBg{WIDTH/2, HEIGHT-75, 50, 50};
    SDL_RenderFillRect(renderer, &countBg);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_Rect countBgOutline{WIDTH/2, HEIGHT-75, 50, 50};
    SDL_RenderDrawRect(renderer, &countBgOutline);
    SDL_Surface* solvedCountSurface = TTF_RenderText_Solid(font, std::to_string(solvedCount).c_str(), {0, 0, 0});
    SDL_Texture* solvedCountTexture = SDL_CreateTextureFromSurface(renderer, solvedCountSurface);
    SDL_Rect solvedCountRect;
    if (solvedCount > 9) {
        solvedCountRect = SDL_Rect{WIDTH/2+2, HEIGHT-75, 50, 50};
    }
    else {
        solvedCountRect = SDL_Rect{WIDTH/2+14, HEIGHT-75, 25, 50};
    }
    SDL_RenderCopy(renderer, solvedCountTexture, NULL, &solvedCountRect);
    SDL_FreeSurface(solvedCountSurface);
    SDL_DestroyTexture(solvedCountTexture);

    // Draw high score
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_Rect hsRectBg{500, HEIGHT-75, 200, 50};
    SDL_RenderFillRect(renderer, &hsRectBg);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_Rect hsRectOutline{500, HEIGHT-75, 200, 50};
    SDL_RenderDrawRect(renderer, &hsRectOutline);
    SDL_Texture* trophy = IMG_LoadTexture(renderer, "textures/trophy.png");
    SDL_Rect trophyRect{500, HEIGHT-75, 50, 50};
    SDL_RenderCopy(renderer, trophy, NULL, &trophyRect);
    SDL_DestroyTexture(trophy);
    // Select four high score
    Result result;
    std::string sqlCommand = "SELECT four from players where name = \"local\"";
    sqlite3_exec(scoresDB, sqlCommand.c_str(), callback, &result, NULL);
    SDL_Rect hsRect;
    SDL_Surface* hsSurface;
    if (result.resultingCall == 0) {
        hsRect = SDL_Rect{564, HEIGHT-75, 75, 50};
        hsSurface = TTF_RenderText_Solid(font, "N/A", {0, 0, 0});
    }
    else if (result.resultingCall > 9) {
        hsRect = SDL_Rect{564, HEIGHT-75, 50, 50};
        hsSurface = TTF_RenderText_Solid(font, std::to_string(result.resultingCall).c_str(), {0, 0, 0});
    }
    else if (result.resultingCall > 99) {
        hsRect = SDL_Rect{564, HEIGHT-75, 100, 50};
        hsSurface = TTF_RenderText_Solid(font, std::to_string(result.resultingCall).c_str(), {0, 0, 0});
    }
    else if (result.resultingCall > 999) {
        hsRect = SDL_Rect{564, HEIGHT-75, 150, 50};
        hsSurface = TTF_RenderText_Solid(font, std::to_string(result.resultingCall).c_str(), {0, 0, 0});
    }
    else {
        hsRect = SDL_Rect{564, HEIGHT-75, 25, 50};
        hsSurface = TTF_RenderText_Solid(font, std::to_string(result.resultingCall).c_str(), {0, 0, 0});
    }
    SDL_Texture* hsTexture = SDL_CreateTextureFromSurface(renderer, hsSurface);
    SDL_RenderCopy(renderer, hsTexture, NULL, &hsRect);
    SDL_FreeSurface(hsSurface);
    SDL_DestroyTexture(hsTexture);
}


void Four::drawGameBoardAll(const std::vector<std::vector<Minesweeper::Cell>>& board) 
{
    int size = 50;
    int start_x = 300;
    int x = start_x;
    int y = 200;

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


void Four::drawGameBoard(const std::vector<std::vector<Minesweeper::Cell>>& board) 
{
    int size = 50;
    int start_x = 300;
    int x = start_x;
    int y = 200;

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


void Four::drawBackground(int add)
{
    // int size = 50; // For scaling purposes
    int size = 25;
    int x = 0+add;
    int y = 0;
    bool dark_purple = true;

    for (int i=0; i<40; ++i) {
        for (int j=0; j<40; ++j) {
            if (dark_purple) {
                SDL_SetRenderDrawColor(renderer, 120, 73, 209, 255);
                dark_purple = false;
            }
            else {
                SDL_SetRenderDrawColor(renderer, 126, 81, 215, 255);
                dark_purple = true;
            }

            SDL_Rect rect{x, y, size, size};
            SDL_RenderFillRect(renderer, &rect);
            x += size;
        }
        x = 0+add;
        y += size;
        dark_purple = !dark_purple;
    }
}


void Four::drawBackgroundWin(int add)
{
    // int size = 50; // For scaling purposes
    int size = 25;
    int x = 0+add;
    int y = 0;
    bool dark_green = true;

    for (int i=0; i<40; ++i) {
        for (int j=0; j<40; ++j) {
            if (dark_green) {
                SDL_SetRenderDrawColor(renderer, 73, 209, 77, 255);
                dark_green = false;
            }
            else {
                SDL_SetRenderDrawColor(renderer, 81, 215, 84, 255);
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

void Four::drawBackgroundLost(int add)
{
    // int size = 50; // For scaling purposes
    int size = 25;
    int x = 0+add;
    int y = 0;
    bool dark_red = true;

    for (int i=0; i<40; ++i) {
        for (int j=0; j<40; ++j) {
            if (dark_red) {
                SDL_SetRenderDrawColor(renderer, 209, 73, 74, 255);
                dark_red = false;
            }
            else {
                SDL_SetRenderDrawColor(renderer, 215, 81, 82, 255);
                dark_red = true;
            }

            SDL_Rect rect{x, y, size, size};
            SDL_RenderFillRect(renderer, &rect);
            x += size;
        }
        x = 0+add;
        y += size;
        dark_red = !dark_red;
    }
}


std::pair<unsigned, unsigned> Four::getCellCoords(const int& x, const int& y)
{
    int cell_row = 0;
    int counter = 200;
    for (int i=0; i<4; ++i) {
        if (y-counter <= 50) {
            break;
        }
        
        counter += 50;
        cell_row++;
    }

    int cell_col = 0;
    counter = 300;
    for (int i=0; i<4; ++i) {
        if (x-counter <= 50) {
            break;
        }

        counter += 50;
        cell_col++;
    }

    return std::pair<unsigned, unsigned>{cell_row, cell_col};
}

