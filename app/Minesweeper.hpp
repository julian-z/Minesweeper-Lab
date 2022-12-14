// Minesweeper.cpp -- Julian Zulfikar, 2022
// ------------------------------------------------------
// Holds the game functionality; implemented with 2D
// vectors. Definitions in Minesweeper.cpp.

#ifndef MINESWEEPER_HPP
#define MINESWEEPER_HPP

#include <vector>
#include <iostream>
#include <string>


class Minesweeper {
private:
    // Cell struct holds information of each square
    struct Cell
    {
        int num;
        bool bomb;
        bool revealed;
        bool flag;
    };

    unsigned numRows;
    unsigned numCols;
    unsigned numBombs;
    unsigned numUnrevealed;
    std::vector<std::vector<Cell>> gameBoard;

public:
    // Constructor initializes gameBoard with given parameters
    Minesweeper(unsigned initRows, unsigned initCols, unsigned initBombs);
    ~Minesweeper() = default;

    // Return number of rows, cols, and bombs
    const unsigned& getRows();
    const unsigned& getCols();
    const unsigned& getBombCount();

    // Returns true if all non-bomb squares are revealed
    bool checkWin();

    // Prints game board into iostream
    void printBoard();

    // User attempts to reveal [x][y]
    // Returns true if successful, false if bomb
    bool move(unsigned x, unsigned y);

    // User attempts to place flag at [x][y]
    // Returns true if square is unrevealed
    bool flag(unsigned x, unsigned y);
};


#endif

