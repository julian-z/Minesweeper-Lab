// TimedGame.cpp -- Julian Zulfikar, 2022
// ------------------------------------------------------
// Timed gamemode definitions.

#include "TimedGame.hpp"


Timed::Timed(SDL_Window* initWindow, SDL_Renderer* initRenderer, SDL_Event initEvent, TTF_Font* initFont, sqlite3* initDB)
: window{initWindow}, renderer{initRenderer}, windowEvent{initEvent}, font{initFont}, scoresDB{initDB}
{
    timedSelection = IMG_LoadTexture(renderer, "textures/minesweeperlabtimedselection.png");
    shadow = IMG_LoadTexture(renderer, "textures/boardshadow.png");
    square = IMG_LoadTexture(renderer, "textures/graysquare.png");
    flag = IMG_LoadTexture(renderer, "textures/graysquareflag.png");
    bomb = IMG_LoadTexture(renderer, "textures/graysquarebomb.png");
    logo = IMG_LoadTexture(renderer, "textures/minesweeperlablogo.png");
}


Timed::~Timed()
{
    SDL_DestroyTexture(timedSelection);
    SDL_DestroyTexture(shadow);
    SDL_DestroyTexture(square);
    SDL_DestroyTexture(flag);
    SDL_DestroyTexture(bomb);
    SDL_DestroyTexture(logo);
}


std::string Timed::runTimed()
{
    int add = 0;

    // Select time
    int selectedTime = 10; // Min = 1, max = 999
    bool selecting = true;
    while (selecting) {
        while (SDL_PollEvent(&windowEvent)) {
            if (windowEvent.type == SDL_QUIT)
            {
                return "CLOSE"; // Signal to close window in main.cpp
            }
            if (windowEvent.type == SDL_MOUSEBUTTONDOWN) {
                if ( (200 <= windowEvent.motion.x && windowEvent.motion.x <= 250) && (300 <= windowEvent.motion.y && windowEvent.motion.y <= 350) ) {
                    selectedTime = (1 <= selectedTime-1 && selectedTime-1 <= 999) ? selectedTime-1 : 999;
                }
                else if ( (250 <= windowEvent.motion.x && windowEvent.motion.x <= 300) && (300 <= windowEvent.motion.y && windowEvent.motion.y <= 350) ) {
                    selectedTime = (1 <= selectedTime+1 && selectedTime+1 <= 999) ? selectedTime+1 : 1;
                }
                else if ( (500 <= windowEvent.motion.x && windowEvent.motion.x <= 600) && (300 <= windowEvent.motion.y && windowEvent.motion.y <= 350) ) {
                    selecting = false;
                }
            }
        }

        // ------------------------

        SDL_RenderClear(renderer);

        drawBackground(add); // Checkerboard

        SDL_RenderCopy(renderer, timedSelection, NULL, NULL);
        SDL_Surface* selectionSurface = TTF_RenderText_Solid(font, std::to_string(selectedTime).c_str(), {0, 0, 0});
        SDL_Texture* selectionTexture = SDL_CreateTextureFromSurface(renderer, selectionSurface);
        SDL_Rect selectionRect;
        if (selectedTime > 9) {
            selectionRect = SDL_Rect{450, 300, 50, 50};
        }
        else if (selectedTime > 99) {
            selectionRect = SDL_Rect{425, 300, 100, 50};
        }
        else {
            selectionRect = SDL_Rect{475, 300, 25, 50};
        }
        SDL_RenderCopy(renderer, selectionTexture, NULL, &selectionRect);
        SDL_FreeSurface(selectionSurface);
        SDL_DestroyTexture(selectionTexture);

        SDL_RenderPresent(renderer);

        // ------------------------

        if (add == -100) {
            add = -1;
        }
        else {
            add--; // Moving background
        }
        SDL_Delay(50);
    }

    // Run Timed gamemode
    bool gameInProgress = true;
    int lastMoveTick = 0;
    bool win = false;
    int hover = 0;

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

                            if (lastMoveTick == 0) {
                                lastMoveTick = SDL_GetTicks64(); // Start recording the time
                            }

                            if (!game.move(coords.first, coords.second)) {
                                gameInProgress = false;
                            }
                            else if (game.checkWin()) {
                                gameInProgress = false;
                                win = true;

                                // Select high score from database
                                Result result;
                                std::string sqlCommand = "SELECT timed from players where name = \"local\"";
                                sqlite3_exec(scoresDB, sqlCommand.c_str(), callback, &result, NULL);

                                if (result.resultingCall == 0 || selectedTime < result.resultingCall) {
                                    // Update high score
                                    sqlCommand = "UPDATE players set timed = "+std::to_string(selectedTime)+" where name=\"local\"";
                                    sqlite3_exec(scoresDB, sqlCommand.c_str(), callback, &result, NULL);
                                }
                            }
                            else {
                                lastMoveTick = SDL_GetTicks64(); // Reset timer every move
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
            else if (!gameInProgress && windowEvent.type == SDL_MOUSEMOTION) {
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
        }
        SDL_RenderClear(renderer);

        drawBackground(add); // Checkerboard

        // ---------------------------------------
        if ((SDL_GetTicks64()-lastMoveTick)/1000 == selectedTime) {
            gameInProgress = false;
        }

        if (gameInProgress) {
            SDL_RenderCopy(renderer, shadow, NULL, NULL);
            drawGameBoard(game.getBoard());
            drawStats(lastMoveTick);
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

                drawStatsGameOver(selectedTime);
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

                drawStatsGameOver(selectedTime);
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


void Timed::drawStats(int startTick)
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
    // Select timed high score
    Result result;
    std::string sqlCommand = "SELECT timed from players where name = \"local\"";
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


void Timed::drawStatsGameOver(int time) 
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
    // Select timed high score
    Result result;
    std::string sqlCommand = "SELECT timed from players where name = \"local\"";
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


void Timed::drawGameBoardAll(const std::vector<std::vector<Minesweeper::Cell>>& board) 
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


void Timed::drawGameBoard(const std::vector<std::vector<Minesweeper::Cell>>& board) 
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


void Timed::drawBackground(int add)
{
    // int size = 50; // For scaling purposes
    int size = 25;
    int x = 0+add;
    int y = 0;
    bool dark_purple = true;

    for (int i=0; i<40; ++i) {
        for (int j=0; j<40; ++j) {
            if (dark_purple) {
                SDL_SetRenderDrawColor(renderer, 189, 144, 143, 255);
                dark_purple = false;
            }
            else {
                SDL_SetRenderDrawColor(renderer, 196, 151, 150, 255);
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


std::pair<unsigned, unsigned> Timed::getCellCoords(const int& x, const int& y)
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

