#include <unistd.h>
#include "board.hpp"

void main_menu(RenderWindow& window, const Sprite& background) {
    Font font;
    try {
        if (!font.loadFromFile("amatic/Amatic-Bold.ttf")) 
            throw 0;
    } catch (int e) {
        if (e == 0)
            cout << "Sorry, font file not found!" << endl;
    }

    Text new_game;
    new_game.setFont(font);
    new_game.setString("New Game");
    new_game.setPosition(window.getSize().x / 2 - 50.0f, 25.0f);
    new_game.setCharacterSize(50); 
    new_game.setFillColor(Color(242, 0, 0, 255));
    new_game.setStyle(Text::Bold);

    while (window.isOpen()) {
        Event event;

        while (window.pollEvent(event)) {
            switch (event.type) {
                // close window:  
                case Event::Closed:
                    window.close();
                    break;
                default:
                    break;
            }
        }

        window.clear();
        window.draw(background);
        window.draw(new_game);
        window.display();
    }    
}

void game(RenderWindow& window, const Vector2f& resolution, const Sprite& background) {
    // change window's size:
    window.setSize(Vector2u(min(41.0f * 14 + 20.0f, resolution.x / 2), min(41.0f * 20 + 9.0f, resolution.y - 100.0f)));

    // create the game board: 
    Board *game_board = new Board(window);

    while (window.isOpen()) {
        Event event;

        while (window.pollEvent(event)) {
            switch (event.type) {
                // close window:  
                case Event::Closed:
                    window.close();
                    break;
                // if we pressed some button:
                case Event::KeyPressed:
                    switch (event.key.code) {
                        // while we want to move a current figure to the right:
                        case Keyboard::D:
                        case Keyboard::Right:
                            game_board->step_right();
                            break;
                        // while we want to move a current figure to the left:
                        case Keyboard::A:
                        case Keyboard::Q:
                        case Keyboard::Left:
                            game_board->step_left();
                            break;
                        // while we want to fall faster:
                        case Keyboard::S:
                        case Keyboard::Down:
                            game_board->step_down();
                            // usleep(50000);
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
                // // set the background image:
                // Texture texture;
                // try {
                //     if (!texture.loadFromFile("background/game_over.png")) 
                //         throw 0;
                // } catch (int e) {
                //     if (e == 0)
                //         cout << "Sorry, game over image not found!" << endl;
                // }

                // // create a background itself:
                // Sprite game_over;
                // game_over.setTexture(texture);
                // window.draw(game_over);
                cout << " Game Over!!!" << endl;
                usleep(1000000);
                break;
            }

            // putting next figure into a current figure:
            game_board->set_current_figure(game_board->get_next_figure());
            // adding new current figure on the board:
            game_board->add_figure();
            // creating the next figure:
            game_board->set_next_figure(game_board->create_figure());
        }

        usleep(300000);
    }

    delete game_board;
}

int main(int argc, char const *argv[]) {
    // get user's resolution:
    Vector2f resolution(VideoMode::getDesktopMode().width, VideoMode::getDesktopMode().height);

    // create the game window
    // which size depends on user's screen resolution:
    RenderWindow window(VideoMode(min(41.0f * 14 + 20.0f, resolution.x / 2), min(41.0f * 20 + 9.0f, resolution.y - 100.0f)), "TetrisSS 1.0");
    
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

    // main_menu(window, background);

    game(window, resolution, background);

    return 0;
}
