#include <iostream>
#include "board.hpp"
#include <unistd.h>

int main(int argc, char const *argv[])
{
    Board *game_board = new Board();

    while(true){
        cout << "\033[2J\033[1;1H";
        cout << *game_board << endl;
        
        if(!game_board->step_down()){
            cout << "\033[2J\033[1;1H";
            cout << *game_board << endl;
            return 0;
        }

        usleep(100000);
    }
    

    return 0;
}
