#include <iostream>
#include "board.hpp"
#include "kbhit.hpp"

void game(){
    Board *game_board = new Board();

    while(true){
        cout << "\033[2J\033[1;1H";
        cout << *game_board << endl;

        // display next figure:
        cout << "Next figure:\n";
        game_board->get_next_figure()->print();
        
        if(kbhit()) {
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
        } else {
            // if we can move down no more:
            if(!game_board->step_down()) {
                // if we reached game over condition:
                if (game_board->game_over()) {
                    cout << " Game Over!!!" << endl;
                    break;
                }

                // putting next figure into a current figure:
                game_board->set_current_figure(game_board->get_next_figure());
                // adding new current figure on the board:
                game_board->add_figure();
                // creating the next figure:
                game_board->set_next_figure(game_board->create_figure());
            }

            usleep(200000);
        }

    }

    delete game_board;
}

int main(int argc, char const *argv[]) {
    game();
    return 0;
}
