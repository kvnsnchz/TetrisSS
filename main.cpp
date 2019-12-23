#include <iostream>
#include "board.hpp"
#include "kbhit.hpp"

void game(){
    Board *game_board = new Board();

    while(true) {
        // display game board:
        cout << "\033[2J\033[1;1H";
        cout << *game_board << endl;

        // display next figure:
        cout << "Next figure:\n";
        game_board->get_next_figure()->print();
        
        // display score:
        cout << "Score: " << game_board->get_score() << endl;
        
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
                // check for the full lines:
                game_board->erase_lines();
                
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
