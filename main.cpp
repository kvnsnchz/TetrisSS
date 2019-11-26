#include <iostream>
#include "board.hpp"
#include "kbhit.hpp"

void game(){
    Board *game_board = new Board();

    while(true){
        cout << "\033[2J\033[1;1H";
        cout << *game_board << endl;
        
        if(kbhit()){
            switch (getchar()){
            case 'd':
                game_board->step_right();
                break;
            case 'q':
                game_board->step_left();
                break;
            default:
                break;
            }
        }
        else{
            if(!game_board->step_down())
                game_board->add_figure();

            usleep(200000);
        }

    }

    delete game_board;
}

int main(int argc, char const *argv[])
{
    
    game();
    return 0;
}
