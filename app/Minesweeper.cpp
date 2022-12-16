// Minesweeper.cpp -- Julian Zulfikar, 2022
// ------------------------------------------------------
// Holds the game functionality; implemented with 2D
// vectors. Declarations & docstrings in Minesweeper.hpp.

#include "Minesweeper.hpp"
#include <cstdlib>
#include <ctime>


// Helper functions
namespace {
    std::vector<std::pair<unsigned, unsigned>> getRadiusCoords(const unsigned& x, const unsigned& y, 
                                                                const unsigned& numRows, const unsigned& numCols)
    {
        std::vector<std::pair<unsigned, unsigned>> coords;

        // Above row
        if (x > 0) {
            unsigned topX = x-1;

            // Above middle
            coords.push_back(std::pair<unsigned, unsigned>{topX, y});

            // Above left
            if (y > 0) {
                coords.push_back(std::pair<unsigned, unsigned>{topX, y-1});
            }

            // Above right
            if (y < numCols-1) {
                coords.push_back(std::pair<unsigned, unsigned>{topX, y+1});
            }
        }

        // Left and right
        if (y > 0) {
            coords.push_back(std::pair<unsigned, unsigned>{x, y-1});
        }
        if (y < numCols-1) {
            coords.push_back(std::pair<unsigned, unsigned>{x, y+1});
        }

        // Below row
        if (x < numRows-1) {
            unsigned belowX = x+1;

            // Below middle
            coords.push_back(std::pair<unsigned, unsigned>{belowX, y});

            // Below left
            if (y > 0) {
                coords.push_back(std::pair<unsigned, unsigned>{belowX, y-1});
            }

            // Below right
            if (y < numCols-1) {
                coords.push_back(std::pair<unsigned, unsigned>{belowX, y+1});
            }
        }

        return coords;
    }
}


Minesweeper::Minesweeper(unsigned initRows, unsigned initCols, unsigned initBombs) : 
numRows{initRows}, numCols{initCols}, numBombs{initBombs}, numFlags{numBombs},
numNotRevealed{(initRows*initCols)-initBombs}
{
    // Initialize empty gameBoard
    for (int i=0; i<numRows; ++i) {
        std::vector<Cell> row;
        for (int k=0; k<numCols; ++k) {
            row.push_back( Cell{0, false, false, false} );
        }
        gameBoard.push_back(row);
    }

    // Randomly select numBombs locations
    for (int i=0; i<numBombs; ++i) {
        unsigned row;
        unsigned col;

        srand((int)time(0));
        while (true) {
            unsigned x = rand() % numRows;
            unsigned y = rand() % numCols;
            if (!gameBoard[x][y].bomb) {
                gameBoard[x][y].bomb = true;
                row = x;
                col = y;
                break;
            }
        }

        // num++ all Cells in radius of [row][col]
        for (const std::pair<unsigned, unsigned>& coord : getRadiusCoords(row, col, numRows, numCols)) {
            gameBoard[coord.first][coord.second].num++;
        }
    }
}


const unsigned& Minesweeper::getRows()
{
    return numRows;
}


const unsigned& Minesweeper::getCols()
{
    return numCols;
}


const unsigned& Minesweeper::getFlags()
{
    return numFlags;
}


const unsigned& Minesweeper::getBombCount()
{
    return numBombs;
}


bool Minesweeper::checkWin()
{
    return numNotRevealed == 0;
}


const std::vector<std::vector<Minesweeper::Cell>>& Minesweeper::getBoard()
{
    return gameBoard;
}


void Minesweeper::printBoardAll()
{
    std::cout << "   ";
    for (int i=0; i<numCols; ++i) {
        std::cout << i << " ";
    }
    std::cout << std::endl;

    int i = 0;
    for (auto& row : gameBoard) {
        std::cout << i << " |";
        for (int i=0; i<numCols; ++i) {
            if (row[i].bomb) {
                std::cout << 'B';
            }
            else {
                std::cout << row[i].num;
            }
            std::cout << '|';
        }
        std::cout << " " << i << std::endl;
        i++;
    }

    std::cout << "   ";
    for (int i=0; i<numCols; ++i) {
        std::cout << i << " ";
    }
    std::cout << std::endl;
}


void Minesweeper::printBoard()
{
    std::cout << "   ";
    for (int i=0; i<numCols; ++i) {
        std::cout << i << " ";
    }
    std::cout << std::endl;

    int i = 0;
    for (auto& row : gameBoard) {
        std::cout << i << " |";
        for (int i=0; i<numCols; ++i) {
            if (row[i].revealed) {
                std::cout << row[i].num;
            }
            else if (row[i].flag) {
                std::cout << 'F';
            }
            else {
                std::cout << " ";
            }
            std::cout << '|';
        }
        std::cout << " " << i << std::endl;
        i++;
    }

    std::cout << "   ";
    for (int i=0; i<numCols; ++i) {
        std::cout << i << " ";
    }
    std::cout << std::endl;
}


bool Minesweeper::flag(unsigned x, unsigned y)
{
    if (numFlags > 0 && !gameBoard[x][y].flag && !gameBoard[x][y].revealed) {
        gameBoard[x][y].flag = true;
        numFlags--;
        return true;
    }

    return false;
}


bool Minesweeper::removeFlag(unsigned x, unsigned y)
{
    if (gameBoard[x][y].flag) {
        gameBoard[x][y].flag = false;
        numFlags++;
        return true;
    }

    return false;
}


void Minesweeper::revealCell(unsigned x, unsigned y)
{
    if (!gameBoard[x][y].revealed) {
        gameBoard[x][y].revealed = true;
        gameBoard[x][y].flag = false;
        numNotRevealed--;
    }
}


void Minesweeper::floodFill(unsigned x, unsigned y) 
{
    if (gameBoard[x][y].revealed) {
        return;
    }

    revealCell(x, y);
    for (const std::pair<unsigned, unsigned>& coord : getRadiusCoords(x, y, numRows, numCols)) {
        if (gameBoard[coord.first][coord.second].num != 0) {
            revealCell(coord.first, coord.second);
        }
    }

    if (x > 0 && gameBoard[x-1][y].num == 0) {
        floodFill(x-1, y);
    }
    if (y > 0 && gameBoard[x][y-1].num == 0) {
        floodFill(x, y-1);
    }
    if (y < numCols-1 && gameBoard[x][y+1].num == 0) {
        floodFill(x, y+1);
    }
    if (x < numRows-1 && gameBoard[x+1][y].num == 0) {
        floodFill(x+1, y);
    }
}


bool Minesweeper::move(unsigned x, unsigned y)
{
    if (gameBoard[x][y].bomb) {
        return false; // Game over
    }
    else if (gameBoard[x][y].num != 0) {
        revealCell(x, y);
        return true;
    }

    // Flood Fill Algorithm
    floodFill(x, y);

    return true;
}

