#include "Minesweeper.hpp"

int main()
{
    Minesweeper game{8,10,10};
    
    game.printBoardAll();

    std::cout << std::endl;

    game.printBoard();

    std::cout << std::endl;

    game.flag(3, 4);
    game.printBoard();

    std::cout << std::endl;

    game.removeFlag(3, 4);
    game.printBoard();

    return 0;
}

