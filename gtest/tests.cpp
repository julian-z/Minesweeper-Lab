// tests.cpp -- Julian Zulfikar, 2022
// ------------------------------------------------------
// Tests written during test-driven development.

#include "gtest/gtest.h"
#include "Minesweeper.hpp"


namespace {

    // Helper function (from Minesweeper.cpp)
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


    TEST(TDD, SimpleInit)
    {
        Minesweeper ms{8, 9, 10};
        EXPECT_TRUE( ms.getBombCount() == 10 );
        EXPECT_TRUE( ms.getCols() == 9 );
        EXPECT_TRUE( ms.getRows() == 8 );
    }


    TEST(TDD, BoardInitTest)
    {
        Minesweeper ms{8, 10, 10};
        auto board = ms.getBoard();
        int bombsFound = 0;

        // Check number of bombs == initBombs
        for (int i=0; i<ms.getRows(); ++i) {
            for (int k=0; k<ms.getCols(); ++k) {
                if (board[i][k].bomb) {
                    bombsFound++;
                }
            }
        }
        EXPECT_TRUE( bombsFound == 10 );

        // Check if numbers in Cell are correct
        for (int i=0; i<ms.getRows(); ++i) {
            for (int k=0; k<ms.getCols(); ++k) {
                if (board[i][k].num != 0) {
                    int target = board[i][k].num;
                    int found = 0;

                    // Scan radius
                    for (const std::pair<unsigned, unsigned>& coord : getRadiusCoords(i, k, ms.getRows(), ms.getCols())) {
                        if (board[coord.first][coord.second].bomb) {
                            found++;
                        }
                    }

                    EXPECT_TRUE( found == target );
                }
            }
        }
    }


    TEST(TDD, TestingFlags)
    {
        Minesweeper ms{8, 10, 5};
        ms.flag(1, 1);
        ms.flag(2, 2);
        ms.flag(3, 3);
        ms.flag(4, 4);
        ms.flag(5, 5);
        auto board = ms.getBoard();

        EXPECT_FALSE( ms.flag(3,1) ); // No more flags
        EXPECT_TRUE( board[1][1].flag );
        EXPECT_TRUE( board[2][2].flag );
        EXPECT_TRUE( board[3][3].flag );
        EXPECT_TRUE( board[4][4].flag );
        EXPECT_TRUE( board[5][5].flag );

        ms.removeFlag(1, 1);
        ms.removeFlag(2, 2);
        board = ms.getBoard();
        EXPECT_FALSE( board[1][1].flag );
        EXPECT_FALSE( board[2][2].flag );
        EXPECT_TRUE( board[3][3].flag );
        EXPECT_TRUE( board[4][4].flag );
        EXPECT_TRUE( board[5][5].flag );

        EXPECT_FALSE( ms.flag(3, 3) ); // Flag already placed there
        EXPECT_FALSE( ms.removeFlag(1, 1) ); // Flag not there
    }


}
