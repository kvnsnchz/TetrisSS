#include <unistd.h>
#include "board.hpp"

void game() {
    // get user's resolution:
    Vector2f resolution(VideoMode::getDesktopMode().width, VideoMode::getDesktopMode().height);

    // create the game window
    // which size depends on user's screen resolution:
    RenderWindow window(VideoMode(min(41.0f * 14 + 20.0f, resolution.x / 2), min(41.0f * 20 + 9.0f, resolution.y)), "TetrisSS 1.0");
    // create the game board: 
    Board *game_board = new Board(window);

    // set the background image:
    Texture texture;
    try {
        if (!texture.loadFromFile("background/bg2.jpg")) 
            throw 0;
    } catch (int e) {
        if (e == 0)
            cout << "Sorry, background image not found!" << endl;
    }

    // create a background itself:
    Sprite background;
    background.setTexture(texture);
    background.setColor(Color(255, 255, 255, 200));

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

        // clear game window:
        window.clear();
        // draw background:
        window.draw(background);
        // draw a board:
        game_board->print_board(window);
        // display what we have just drawed:
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
