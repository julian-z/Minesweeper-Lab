// MinesweeperAI.hpp -- Julian Zulfikar, 2022
// ------------------------------------------------------
// Modified game functionality for tutorial AI.
//
// NOTE: This is not a "true" AI, as some knowledge behind
// the game is used (i.e. finding a zero to reveal as
// the first move). This is due to the AI being only used 
// for demonstration purposes in the
// tutorial section.

#ifndef MINESWEEPERAI_HPP
#define MINESWEEPERAI_HPP

#include <vector>
#include <iostream>
#include <string>
#include <unordered_set>


class MinesweeperAI {
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
    MinesweeperAI(unsigned initRows, unsigned initCols, unsigned initBombs);
    ~MinesweeperAI() = default;

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

    // AI makes a move
    int move();

    // Reset the game
    void reset(unsigned initBombs);

    // Get x and y of AI's current move
    std::pair<unsigned, unsigned> getCurrentMove();

    // Set to a given gameboard
    void setGameBoard(std::vector<std::vector<Cell>> newBoard);

    // Return coords in radius (for glow overlay)
    std::vector<std::pair<unsigned, unsigned>> getRadiusCoords(const unsigned& x, const unsigned& y);

private:
    unsigned numRows;
    unsigned numCols;
    unsigned numBombs;
    unsigned numFlags;
    unsigned numNotRevealed;
    std::vector<std::vector<Cell>> gameBoard;

    struct pair_hash {
        inline std::size_t operator()(const std::pair<int,int> & v) const {
            return v.first*31+v.second;
        }
    };
    std::unordered_set<std::pair<unsigned, unsigned>, pair_hash> inspected;
    std::pair<unsigned, unsigned> currentMove;
    bool firstMove = true;
    bool move(unsigned x, unsigned y);

    // Helper function to make cell visible to player
    void revealCell(unsigned x, unsigned y);

    // Flood fill algorithm for revealing
    void floodFill(unsigned x, unsigned y);
};


#endif

