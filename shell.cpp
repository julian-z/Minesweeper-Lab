// shell.cpp -- Julian Zulfikar, 2023
// ------------------------------------------------------
// Shell demonstration of the base game.

#include "Minesweeper.hpp"

int main()
{
    std::cout << "Minesweeper: Terminal Version -- Julian Zulfikar, 2022" << std::endl;
    std::cout << "------------------------------------------------------" << std::endl;
    std::cout << "Solely meant for demonstration purposes. Bounds" << std::endl;
    std::cout << "checking is not implemented, nor are invalid inputs." << std::endl;
    std::cout << "------------------------------------------------------" << std::endl;
    std::cout << "Note: X = row, Y = col" << std::endl;
    std::cout << "'M X Y' = Reveal cell at (x, y)" << std::endl;
    std::cout << "'F X Y' = Flag cell at (x, y)" << std::endl;
    std::cout << "'R X Y' = Remove flag at (x, y)" << std::endl;
    std::cout << "------------------------------------------------------" << std::endl;
    
    unsigned rows;
    unsigned cols;
    unsigned bombs;
    std::cout << "Number of rows: ";
    std::cin >> rows;
    std::cout << "Number of columns: ";
    std::cin >> cols;
    std::cout << "Number of bombs: ";
    std::cin >> bombs;
    std::cout << "Initializing game..." << std::endl;
    
    Minesweeper game{rows, cols, bombs};

    std::cout << "------------------------------------------------------" << std::endl;

    while (true) {
        game.printBoard();

        char move;
        unsigned x;
        unsigned y;
        std::cin >> move >> x >> y;

        if (move == 'M') {
            bool result = game.move(x, y);

            if (!result) {
                std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
                game.printBoardAll();
                std::cout << "LOST" << std::endl;
                break;
            }

            if (game.checkWin()) {
                std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
                game.printBoardAll();
                std::cout << "WON" << std::endl;
                break;
            }
        }
        else if (move == 'F') {
            game.flag(x, y);
        }
        else if (move == 'R') {
            game.removeFlag(x, y);
        }

        std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
    }

    return 0;
}

