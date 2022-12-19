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
public:
    // Cell struct holds information of each square
    struct Cell
    {
        int num;
        bool bomb;
        bool revealed;
        bool flag;
    };

    // Constructor initializes gameBoard with given parameters
    Minesweeper(unsigned initRows, unsigned initCols, unsigned initBombs);
    ~Minesweeper() = default;

    // Return number of rows, cols, flags, bombs
    const unsigned& getRows();
    const unsigned& getCols();
    const unsigned& getFlags();
    const unsigned& getBombCount();

    // Returns true if all non-bomb squares are revealed
    bool checkWin();

    // Prints game board into iostream (including unrevealed cells)
    void printBoardAll();

    // Prints game board into iostream
    void printBoard();

    // Returns const reference of gameBoard
    const std::vector<std::vector<Cell>>& getBoard();

    // User attempts to place flag at [x][y]
    // Returns true if square is unrevealed, flag is not already placed,
    // and numFlags > 0
    bool flag(unsigned x, unsigned y);

    // User attempts to remove fag at [x][y]
    // Returns true if there was a flag
    bool removeFlag(unsigned x, unsigned y);

    // User attempts to reveal [x][y]
    // Returns true if successful, false if bomb
    bool move(unsigned x, unsigned y);

private:
    unsigned numRows;
    unsigned numCols;
    unsigned numBombs;
    unsigned numFlags;
    unsigned numNotRevealed;
    std::vector<std::vector<Cell>> gameBoard;

    // Helper function to make cell visible to player
    void revealCell(unsigned x, unsigned y);

    // Flood fill algorithm for revealing
    void floodFill(unsigned x, unsigned y);
};


#endif

