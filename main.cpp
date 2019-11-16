#include <iostream>
#include "board.hpp"

int main(int argc, char const *argv[])
{
    Board *game_board = new Board();
    cout << *game_board << endl;

    return 0;
}
