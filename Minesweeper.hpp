// Minesweeper.hpp -- Julian Zulfikar, 2023
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
    const unsigned& getRows() const;
    const unsigned& getCols() const;
    const unsigned& getFlags() const;
    const unsigned& getBombCount() const;

    // Returns true if all non-bomb squares are revealed
    bool checkWin() const;

    // Prints game board into iostream (including unrevealed cells)
    void printBoardAll() const;

    // Prints game board into iostream
    void printBoard() const;

    // Returns const reference of gameBoard
    const std::vector<std::vector<Cell>>& getBoard() const;

    // User attempts to place flag at [x][y]
    // Returns true if square is unrevealed, flag is not already placed,
    // and numFlags > 0
    bool flag(unsigned x, unsigned y);

    // User attempts to remove flag at [x][y]
    // Returns true if there was a flag
    bool removeFlag(unsigned x, unsigned y);

    // Returns true if there was a flag
    bool findFlag(unsigned x, unsigned y);

    // User attempts to reveal [x][y]
    // Returns true if successful, false if bomb
    bool move(unsigned x, unsigned y);

    // Reset the game
    void reset(unsigned initBombs);

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

