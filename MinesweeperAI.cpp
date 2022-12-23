// MinesweeperAI.cpp -- Julian Zulfikar, 2023
// ------------------------------------------------------
// Modified game functionality for tutorial AI.
//
// NOTE: This is not a "true" AI, as some knowledge behind
// the game is used (i.e. finding a zero to reveal as
// the first move). This is due to the AI being only used 
// for demonstration purposes in the
// tutorial section.

#include "MinesweeperAI.hpp"
#include <cstdlib>
#include <ctime>


// Helper functions
std::vector<std::pair<unsigned, unsigned>> MinesweeperAI::getRadiusCoords(const unsigned& x, const unsigned& y)
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


MinesweeperAI::MinesweeperAI(unsigned initRows, unsigned initCols, unsigned initBombs) : 
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
        for (const std::pair<unsigned, unsigned>& coord : getRadiusCoords(row, col)) {
            gameBoard[coord.first][coord.second].num++;
        }
    }
}


const unsigned& MinesweeperAI::getRows() const
{
    return numRows;
}


const unsigned& MinesweeperAI::getCols() const
{
    return numCols;
}


const unsigned& MinesweeperAI::getFlags() const
{
    return numFlags;
}


const unsigned& MinesweeperAI::getBombCount() const
{
    return numBombs;
}


bool MinesweeperAI::checkWin() const
{
    return numNotRevealed == 0;
}


const std::vector<std::vector<MinesweeperAI::Cell>>& MinesweeperAI::getBoard() const
{
    return gameBoard;
}


void MinesweeperAI::printBoardAll() const
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


void MinesweeperAI::printBoard() const
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


bool MinesweeperAI::flag(unsigned x, unsigned y)
{
    if (numFlags > 0 && !gameBoard[x][y].flag && !gameBoard[x][y].revealed) {
        gameBoard[x][y].flag = true;
        numFlags--;
        return true;
    }

    return false;
}


bool MinesweeperAI::removeFlag(unsigned x, unsigned y)
{
    if (gameBoard[x][y].flag) {
        gameBoard[x][y].flag = false;
        numFlags++;
        return true;
    }

    return false;
}


bool MinesweeperAI::findFlag(unsigned x, unsigned y) 
{
    return gameBoard[x][y].flag;
}


void MinesweeperAI::revealCell(unsigned x, unsigned y)
{
    if (!gameBoard[x][y].revealed) {
        gameBoard[x][y].revealed = true;
        gameBoard[x][y].flag = false;
        numNotRevealed--;
    }
}


void MinesweeperAI::floodFill(unsigned x, unsigned y) 
{
    if (gameBoard[x][y].revealed) {
        return;
    }

    revealCell(x, y);
    for (const std::pair<unsigned, unsigned>& coord : getRadiusCoords(x, y)) {
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


bool MinesweeperAI::move(unsigned x, unsigned y)
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


int MinesweeperAI::move()
{
    // Find a zero to reveal as first move
    if (firstMove) {
        for (unsigned x=0; x<numRows; ++x) {
            for (unsigned y=0; y<numCols; ++y) {
                if (gameBoard[x][y].num == 0 && !gameBoard[x][y].bomb) {
                    move(x, y);
                    currentMove = std::pair<unsigned, unsigned>{x, y};
                    inspected.insert(currentMove);
                    firstMove = false;
                    return 0;
                }
            }
        }
    }

    // Rule 1
    for (unsigned x=0; x<numRows; ++x) {
        for (unsigned y=0; y<numCols; ++y) {
            int num = gameBoard[x][y].num;

            if (num != 0 && !inspected.count(std::pair<unsigned, unsigned>{x, y}) && gameBoard[x][y].revealed) {
                int numHidden = 0;
                std::vector<std::pair<unsigned, unsigned>> hiddenCells;

                for (auto coord : getRadiusCoords(x, y)) {
                    if (!gameBoard[coord.first][coord.second].revealed && !gameBoard[coord.first][coord.second].flag) {
                        numHidden++;
                        hiddenCells.push_back(coord);
                    }
                    else if (!gameBoard[coord.first][coord.second].revealed && gameBoard[coord.first][coord.second].flag) {
                        num--;
                    }
                }

                if (numHidden == num) {
                    for (auto coord : hiddenCells) {
                        flag(coord.first, coord.second);
                        inspected.insert(std::pair<unsigned, unsigned>{x, y});
                    }
                    currentMove = std::pair<unsigned, unsigned>{x, y};
                    inspected.insert(currentMove);
                    return 1;
                }
            }
        }
    }

    // Rule 2
    for (unsigned x=0; x<numRows; ++x) {
        for (unsigned y=0; y<numCols; ++y) {
            int num = gameBoard[x][y].num;

            if (num != 0 && !inspected.count(std::pair<unsigned, unsigned>{x, y}) && gameBoard[x][y].revealed) {
                int numFlags = 0;
                std::vector<std::pair<unsigned, unsigned>> hiddenCells;

                for (auto coord : getRadiusCoords(x, y)) {
                    if (gameBoard[coord.first][coord.second].flag) {
                        numFlags++;
                    }
                    else if (!gameBoard[coord.first][coord.second].revealed) {
                        hiddenCells.push_back(coord);
                    }
                }

                if (numFlags == num) {
                    for (auto coord : hiddenCells) {
                        move(coord.first, coord.second);
                    }
                    currentMove = std::pair<unsigned, unsigned>{x, y};
                    inspected.insert(currentMove);
                    return 2;
                }
            }
        }
    }

    // Rule 3
    for (unsigned x=0; x<numRows; ++x) {
        for (unsigned y=0; y<numCols; ++y) {
            int num = gameBoard[x][y].num;

            if (num != 0 && !inspected.count(std::pair<unsigned, unsigned>{x, y}) && gameBoard[x][y].revealed) {
                for (auto coord : getRadiusCoords(x, y)) {
                    if (!gameBoard[coord.first][coord.second].revealed && !gameBoard[coord.first][coord.second].bomb) {
                        currentMove = std::pair<unsigned, unsigned>{x, y};
                        revealCell(coord.first, coord.second);
                        return 3; // AI cannot guess, use behind the scenes knowledge
                    }
                }
            }
        }
    }

    return -1;
}


void MinesweeperAI::reset(unsigned initBombs)
{
    // Reinitialize bombs
    numBombs = initBombs;
    numFlags = initBombs;
    numNotRevealed = (numRows*numCols)-initBombs;

    // Initialize empty gameBoard
    gameBoard.clear();
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
        for (const std::pair<unsigned, unsigned>& coord : getRadiusCoords(row, col)) {
            gameBoard[coord.first][coord.second].num++;
        }
    }
}


std::pair<unsigned, unsigned> MinesweeperAI::getCurrentMove()
{
    return currentMove;
}


void MinesweeperAI::setGameBoard(std::vector<std::vector<Cell>> newBoard)
{
    gameBoard = newBoard;
}

