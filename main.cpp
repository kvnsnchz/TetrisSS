#include <unistd.h>
#include "board.hpp"

void game() {
    RenderWindow window(VideoMode(1000, 1000), "TetrisSS 1.0");
    Board *game_board = new Board(window);

    while (window.isOpen()) {
        Event event;

        while (window.pollEvent(event)) {
            switch (event.type) {
                case Event::Closed:
                    window.close();
                    break;
                case Event::KeyPressed:
                    switch (event.key.code) {
                        case Keyboard::D:
                        case Keyboard::Right:
                            game_board->step_right();
                            break;
                        case Keyboard::A:
                        case Keyboard::Q:
                        case Keyboard::Left:
                            game_board->step_left();
                            break;
                        default:
                            break;
                    }
                default:
                    break;
            }
        }

        window.clear();
        game_board->print_board(window);
        window.display();

        // if we can't move down no more:
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

    delete game_board;
}

int main(int argc, char const *argv[]) {
    game();

    return 0;
}
