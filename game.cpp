#include "game.hpp"

// new button initialization function (for code reduction):
Text create_button(const Font& font, const char* title, const double& button_size, Vector2f position, const bool& outline, const bool& center) {
    // Initialize new button:
    Text new_button;
    new_button.setFont(font);
    new_button.setString(title);
    new_button.setCharacterSize(5 * button_size / 6);
    new_button.setStyle(Text::Bold);
    new_button.setFillColor(Color(144, 12, 63, 255));
    if (outline) {
        new_button.setOutlineThickness(button_size / 6);
        new_button.setOutlineColor(Color(218, 247, 166, 255));
    }
    if (center)
        position.x -= new_button.getGlobalBounds().width / 2;
    new_button.setPosition(position);

    return new_button;
};

// captured button identifier:
bool captured_button(RenderWindow& window, Text& button) {
    if ((Mouse::getPosition(window).x - button.getGlobalBounds().left >= 0) &&
        (Mouse::getPosition(window).y - button.getGlobalBounds().top >= 0) &&
        (Mouse::getPosition(window).x - button.getGlobalBounds().left <= button.getGlobalBounds().width) &&
        (Mouse::getPosition(window).y - button.getGlobalBounds().top <= button.getGlobalBounds().height))
        return true;
    else
        return false;                               
};

void game(RenderWindow& window, Sprite& background, const Font& font, const unsigned& complexity) {
    // counter of the currently chosen button:
    unsigned focused_button_counter = 0;
    // button size:
    double button_size = min(60.0, 3.75 * (0.27 * window.getSize().x - 5.0f) / 10.0);

    // initialize cell size according the current window size:
    Vector2f cell_size(min(min(40.0f, (float) (0.73 * window.getSize().x - 29.0f) / 10), min(40.0f, ((float) window.getSize().y - 29.0f) / 20)),
                    min(min(40.0f, (float) (0.73 * window.getSize().x - 29.0f) / 10), min(40.0f, ((float) window.getSize().y - 29.0f) / 20)));

    // create the game board: 
    Board* game_board = new Board(window, complexity, cell_size);

    // Initialize pause button:
    Text pause = create_button(font, "Pause", button_size,
        Vector2f((cell_size.x + 1) * game_board->get_x_dim() + 29.0f, 4 * button_size + 125.0f), true, false);

    state_figure _state_figure = DESCEND_FIGURE;
    int count_change_figure = DEF_COU_CHA_FIG;

    // We are using descend counter to manage the figures' fall rate:
    for (unsigned descend_counter = 0; window.isOpen(); descend_counter++) {
        Event event;

        while (window.pollEvent(event)) {
            switch (event.type) {
                // close window:  
                case Event::Closed:
                    window.close();
                    break;
                // when we are moving mouse:
                case Event::MouseMoved:
                    // unfocus all the buttons:
                    pause.setFillColor(Color(144, 12, 63, 255));
                    pause.setOutlineColor(Color(218, 247, 166, 255));

                    // If appropriate mouse position was captured:
                    if (captured_button(window, pause)) {
                        // focus pause button:
                        pause.setFillColor(Color(255, 195, 0, 255));
                        pause.setOutlineColor(Color(8, 0, 93, 255));
                        focused_button_counter = 1;
                    }
                    break;
                case Event::MouseButtonPressed:
                    switch (event.key.code) {
                        case Mouse::Left:
                            // If appropriate mouse position was captured:
                            // 1) pause game:
                            if (captured_button(window, pause)) {
                                // execute pause button:
                                pause_menu(window, background, game_board, font);
                                        
                                // update cell size:
                                cell_size.x = min(min(40.0f, (float) (0.73 * window.getSize().x - 29.0f) / 10), min(40.0f, ((float) window.getSize().y - 29.0f) / 20));
                                cell_size.y = min(min(40.0f, (float) (0.73 * window.getSize().x - 29.0f) / 10), min(40.0f, ((float) window.getSize().y - 29.0f) / 20));
                                game_board->set_cell_size(cell_size);

                                // update button size:
                                button_size = min(60.0, 3.75 * (0.27 * window.getSize().x - 5.0f) / 10.0);

                                // update all the buttons and their positions:
                                pause.setCharacterSize(5 * button_size / 6);
                                pause.setOutlineThickness(button_size / 6);
                                pause.setPosition((cell_size.x + 1) * game_board->get_x_dim() + 29.0f, 4 * button_size + 125.0f);
                        
                                // unfocus pause button:
                                pause.setFillColor(Color(144, 12, 63, 255));
                                pause.setOutlineColor(Color(218, 247, 166, 255));
                            }
                            break;
                        default:
                            break;
                    }
                    break;
                // if we pressed some button:
                case Event::KeyPressed:
                    switch (event.key.code) {
                        // if we want to focus (Tab) or push (Enter) some button using keyboard:
                        case Keyboard::Tab:
                        case Keyboard::Return:
                            // focus or push the button according to 
                            // the current focused_button_counter value:
                            switch (focused_button_counter) {
                                // if it is the first press of Tab:
                                case 0:
                                    // in this case, Enter won't do nothing:
                                    if (event.key.code == Keyboard::Return)
                                        break;

                                    // focus main menu button:
                                    pause.setFillColor(Color(255, 195, 0, 255));
                                    pause.setOutlineColor(Color(8, 0, 93, 255));
                                    focused_button_counter++;
                                    break;
                                case 1:
                                    // if we have pressed Enter:
                                    if (event.key.code == Keyboard::Return) {
                                        // execute pause button:
                                        pause_menu(window, background, game_board, font);

                                        // update cell size:
                                        cell_size.x = min(min(40.0f, (float) (0.73 * window.getSize().x - 29.0f) / 10), min(40.0f, ((float) window.getSize().y - 29.0f) / 20));
                                        cell_size.y = min(min(40.0f, (float) (0.73 * window.getSize().x - 29.0f) / 10), min(40.0f, ((float) window.getSize().y - 29.0f) / 20));
                                        game_board->set_cell_size(cell_size);

                                        // update button size:
                                        button_size = min(60.0, 3.75 * (0.27 * window.getSize().x - 5.0f) / 10.0);
                                    
                                        // update all the buttons and their positions:
                                        pause.setCharacterSize(5 * button_size / 6);
                                        pause.setOutlineThickness(button_size / 6);
                                        pause.setPosition((cell_size.x + 1) * game_board->get_x_dim() + 29.0f, 4 * button_size + 125.0f);
                                    }

                                    // unfocus main menu button:
                                    pause.setFillColor(Color(144, 12, 63, 255));
                                    pause.setOutlineColor(Color(218, 247, 166, 255));
                                    focused_button_counter = 0;
                                    break;
                                default:
                                    break;
                            }
                            break;
                        // if Escape is pushed: 
                        case Keyboard::Escape:
                            // execute pause button:
                            pause_menu(window, background, game_board, font);

                            // update cell size:
                            cell_size.x = min(min(40.0f, (float) (0.73 * window.getSize().x - 29.0f) / 10), min(40.0f, ((float) window.getSize().y - 29.0f) / 20));
                            cell_size.y = min(min(40.0f, (float) (0.73 * window.getSize().x - 29.0f) / 10), min(40.0f, ((float) window.getSize().y - 29.0f) / 20));
                            game_board->set_cell_size(cell_size);

                            // update button size:
                            button_size = min(60.0, 3.75 * (0.27 * window.getSize().x - 5.0f) / 10.0);

                            // update all the buttons and their positions:
                            pause.setCharacterSize(5 * button_size / 6);
                            pause.setOutlineThickness(button_size / 6);
                            pause.setPosition((cell_size.x + 1) * game_board->get_x_dim() + 29.0f, 4 * button_size + 125.0f);
                            break;
                        // while we want to move a current figure to the right:
                        case Keyboard::D:
                        case Keyboard::Right:
                            if(_state_figure == STOP_FIGURE){
                                bool step_done = game_board->step_right(true);
                                if(step_done)
                                    count_change_figure++;
                                else
                                    count_change_figure = 0;
                                break;
                            }
                            
                            game_board->step_right(false);
                            break;
                        // while we want to move a current figure to the left:
                        case Keyboard::A:
                        case Keyboard::Q:
                        case Keyboard::Left:
                            
                            if(_state_figure == STOP_FIGURE){
                                bool step_done = game_board->step_left(true);
                                if(step_done)
                                    count_change_figure++;
                                else
                                   count_change_figure = 0;
                                break;
                            }

                            game_board->step_left(false);
                            break;
                        // while we want to fall faster:
                        case Keyboard::S:
                        case Keyboard::Down:
                            if(_state_figure != STOP_FIGURE)
                                game_board->step_down();
                            break;
                        case Keyboard::G:
                        case Keyboard::Up:
                            if(_state_figure != STOP_FIGURE)
                                game_board->rotate(false);
                            break;
                        case Keyboard::H:
                            if(_state_figure != STOP_FIGURE)
                                game_board->rotate(true);
                            break;
                        default:
                            break;
                    }
                    break;
                // if we have changed window's size:
                case Event::Resized:
                    // set minimal window size:
                    if ((window.getSize().y < 600) || (window.getSize().x < 400))
                        window.setSize(Vector2u(400, 600));
                    
                    // update view:
                    window.setView(View(FloatRect(0.0f, 0.0f, (float) window.getSize().x, (float) window.getSize().y)));

                    // update cell size:
                    cell_size.x = min(min(40.0f, (float) (0.73 * window.getSize().x - 29.0f) / 10), min(40.0f, ((float) window.getSize().y - 29.0f) / 20));
                    cell_size.y = min(min(40.0f, (float) (0.73 * window.getSize().x - 29.0f) / 10), min(40.0f, ((float) window.getSize().y - 29.0f) / 20));
                    game_board->set_cell_size(cell_size);

                    // update button size:
                    button_size = min(60.0, 3.75 * (0.27 * window.getSize().x - 5.0f) / 10.0);

                    // update all the buttons and their positions:
                    pause.setCharacterSize(5 * button_size / 6);
                    pause.setOutlineThickness(button_size / 6);
                    pause.setPosition((cell_size.x + 1) * game_board->get_x_dim() + 29.0f, 4 * button_size + 125.0f);
                    break;
                default:
                    break;
            }
        }

        // clear game window:
        window.clear();

        // draw background:
        window.draw(background);

        // update view:
        // window.setView(View(FloatRect(0.0f, 0.0f, (float) window.getSize().x, (float) window.getSize().y)));

        // check for the full lines:
        game_board->erase_lines(complexity);

        // draw a board:
        game_board->print_board(window, font, 5 * button_size / 6);
        // draw pause button:
        window.draw(pause);
        
        // set delay according to the complexity:
        if (descend_counter >= 300 / complexity) {
            if(_state_figure == STOP_FIGURE)
                count_change_figure--;
            if(count_change_figure <= 0)
                _state_figure = CHANGE_FIGURE;
            if(_state_figure == DESCEND_FIGURE)
                _state_figure = game_board->step_down() ? DESCEND_FIGURE : STOP_FIGURE; 
            // if we can't move down no more:
            if (_state_figure == CHANGE_FIGURE) {

                count_change_figure = DEF_COU_CHA_FIG;
                _state_figure = DESCEND_FIGURE;
                // check for the full lines:
                game_board->fix_current_figure();
                // game_board->erase_lines(complexity);

                // if we have reached game over condition:
                if (game_board->game_over())
                    game_over_menu(window, background, game_board, font);

                // putting next figure into a current figure:
                game_board->set_current_figure(game_board->get_next_figure());
                // adding new current figure on the board:
                game_board->add_figure();
                // creating the next figure:
                game_board->set_next_figure(game_board->create_figure());
            }

            descend_counter = 0;
        }

        // display what we have just drawn:
        window.display();
    }

    delete game_board;
}

void game_over_menu(RenderWindow& window, Sprite& background, Board* game_board, const Font& font) {
    // counter of the currently chosen button:
    unsigned focused_button_counter = 0;
    // number of buttons:
    const unsigned number_of_buttons = 4;
    // button size:
    double button_size = min(min(60.0, 3.5 * (window.getSize().x - 10.0) / 16), 
        min((window.getSize().y - 50.0 - 15.0 * (number_of_buttons - 1)) / number_of_buttons / 3, 3.75 * (0.27 * window.getSize().x - 5.0f) / 10.0));

    // initalize the game over background:
    RectangleShape game_over_background;
    game_over_background.setSize(Vector2f(window.getSize().x, window.getSize().y));
    game_over_background.setFillColor(Color(255, 255, 255, 100));

    // set the game over image:
    Texture texture;
    try {
        if (!texture.loadFromFile("background/game_over.png")) 
            throw 0;
    } catch (int e) {
        if (e == 0)
            cout << "Sorry, game over image not found!" << endl;
    }

    // create a game over sprite itself:
    Sprite game_over;
    game_over.setTexture(texture);
    game_over.setScale((float) window.getSize().x / texture.getSize().x, (float) window.getSize().y / texture.getSize().y / 1.2);
    game_over.setColor(Color(255, 255, 255, 100));

    // Initialize restart button:
    Text restart;
    restart = create_button(font, "Restart", button_size,
        Vector2f((window.getSize().x - restart.getGlobalBounds().width) / 2,
            (window.getSize().y - number_of_buttons * (button_size + 15) - 15)));

    // Initialize choose complexity button:
    Text choose_complexity = create_button(font, "Choose Complexity", button_size,
        Vector2f(window.getSize().x / 2,
            (window.getSize().y - number_of_buttons * (button_size + 15) - 15) + button_size + 15));
    
    // Initialize to_main_menu button:
    Text to_main_menu = create_button(font, "Main Menu", button_size,
        Vector2f(window.getSize().x / 2,
            (window.getSize().y - number_of_buttons * (button_size + 15) - 15) + (button_size + 15) * 2));
    
    // Initialize exit button:
    Text exit = create_button(font, "Exit", button_size,
        Vector2f(window.getSize().x / 2, 
            (window.getSize().y - number_of_buttons * (button_size + 15) - 15) + (button_size + 15) * 3));

    while (window.isOpen()) {
        Event event;

        while (window.pollEvent(event)) {
            switch (event.type) {
                // close window:  
                case Event::Closed:
                    window.close();
                    break;
                // when we are moving mouse:
                case Event::MouseMoved:
                    // unfocus all the buttons:
                    restart.setFillColor(Color(144, 12, 63, 255));
                    restart.setOutlineColor(Color(218, 247, 166, 255));
                    choose_complexity.setFillColor(Color(144, 12, 63, 255));
                    choose_complexity.setOutlineColor(Color(218, 247, 166, 255));
                    to_main_menu.setFillColor(Color(144, 12, 63, 255));
                    to_main_menu.setOutlineColor(Color(218, 247, 166, 255));
                    exit.setFillColor(Color(144, 12, 63, 255));
                    exit.setOutlineColor(Color(218, 247, 166, 255));

                    // If appropriate mouse position was captured:
                    if (captured_button(window, restart)) {
                        // focus restart button:
                        restart.setFillColor(Color(255, 195, 0, 255));
                        restart.setOutlineColor(Color(8, 0, 93, 255));
                        focused_button_counter = 1;
                    } else if (captured_button(window, choose_complexity)) {
                        // focus new choose_complexity game button:
                        choose_complexity.setFillColor(Color(255, 195, 0, 255));
                        choose_complexity.setOutlineColor(Color(8, 0, 93, 255));
                        focused_button_counter = 2;
                    } else if (captured_button(window, to_main_menu)) {
                        // focus to_main_menu button:
                        to_main_menu.setFillColor(Color(255, 195, 0, 255));
                        to_main_menu.setOutlineColor(Color(8, 0, 93, 255));
                        focused_button_counter = 3;
                    } else if (captured_button(window, exit)) {
                        // focus exit button:
                        exit.setFillColor(Color(255, 195, 0, 255));
                        exit.setOutlineColor(Color(8, 0, 93, 255));
                        focused_button_counter = 4;
                    }
                    break;
                case Event::MouseButtonPressed:
                    switch (event.key.code) {
                        case Mouse::Left:
                            // If appropriate mouse position was captured:
                            // 1) Restart game:
                            if (captured_button(window, restart))
                                game(window, background, font, game_board->get_complexity());
                            // 2) Go to cemplexity menu:
                            else if (captured_button(window, choose_complexity))
                                complexity_menu(window, background, font);
                            // 3) Go to main menu:
                            else if (captured_button(window, to_main_menu))
                                main_menu(window, background, font);
                            // 4) Exit:
                            else if (captured_button(window, exit))
                                window.close();
                            break;
                        default:
                            break;
                    }
                case Event::KeyPressed:
                    switch (event.key.code) {
                        // if we want to focus (Tab) or push (Enter) some button using keyboard:
                        case Keyboard::Tab:
                        case Keyboard::Return:
                            // focus or push the button according to 
                            // the current focused_button_counter value:
                            switch (focused_button_counter) {
                                // if it is the first press of Tab:
                                case 0:
                                    // in this case, Enter won't do nothing:
                                    if (event.key.code == Keyboard::Return)
                                        break;

                                    // focus restart button:
                                    restart.setFillColor(Color(255, 195, 0, 255));
                                    restart.setOutlineColor(Color(8, 0, 93, 255));
                                    focused_button_counter++;
                                    break;
                                case 1:
                                    // if we have pressed Enter:
                                    if (event.key.code == Keyboard::Return)
                                        // execute restart button:
                                        game(window, background, font, game_board->get_complexity());

                                    // unfocus restart button:
                                    restart.setFillColor(Color(144, 12, 63, 255));
                                    restart.setOutlineColor(Color(218, 247, 166, 255));
                                    // focus choose_complexity button:
                                    choose_complexity.setFillColor(Color(255, 195, 0, 255));
                                    choose_complexity.setOutlineColor(Color(8, 0, 93, 255));
                                    focused_button_counter++;
                                    break;
                                case 2:
                                    // if we have pressed Enter:
                                    if (event.key.code == Keyboard::Return)
                                        // execute choose_complexity button:
                                        complexity_menu(window, background, font);

                                    // unfocus choose_complexity button:
                                    choose_complexity.setFillColor(Color(144, 12, 63, 255));
                                    choose_complexity.setOutlineColor(Color(218, 247, 166, 255));
                                    // focus to_main_menu button:
                                    to_main_menu.setFillColor(Color(255, 195, 0, 255));
                                    to_main_menu.setOutlineColor(Color(8, 0, 93, 255));
                                    focused_button_counter++;
                                    break;
                                case 3:
                                    // if we have pressed Enter:
                                    if (event.key.code == Keyboard::Return)
                                        // execute to_main_menu button:
                                        main_menu(window, background, font);

                                    // unfocus to_main_menu button:
                                    to_main_menu.setFillColor(Color(144, 12, 63, 255));
                                    to_main_menu.setOutlineColor(Color(218, 247, 166, 255));
                                    // focus exit button:
                                    exit.setFillColor(Color(255, 195, 0, 255));
                                    exit.setOutlineColor(Color(8, 0, 93, 255));
                                    focused_button_counter++;
                                    break;
                                case 4:
                                    // if we have pressed Enter:
                                    if (event.key.code == Keyboard::Return)
                                        // execute exit button:
                                        window.close();

                                    // unfocus exit button:
                                    exit.setFillColor(Color(144, 12, 63, 255));
                                    exit.setOutlineColor(Color(218, 247, 166, 255));
                                    // focus restart button:
                                    restart.setFillColor(Color(255, 195, 0, 255));
                                    restart.setOutlineColor(Color(8, 0, 93, 255));
                                    focused_button_counter = 1;
                                    break;
                                default:
                                    break;
                            }
                        default:
                            break;
                    }
                // if we have changed window's size:
                case Event::Resized:
                    // set minimal window size:
                    if ((window.getSize().y < 600) || (window.getSize().x < 400))
                        window.setSize(Vector2u(400, 600));
            
                    // update button size:
                    button_size = min(min(60.0, 3.5 * (window.getSize().x - 10.0) / 16), 
                        min((window.getSize().y - 50.0 - 15.0 * (number_of_buttons - 1)) / number_of_buttons / 3, 3.75 * (0.27 * window.getSize().x - 5.0f) / 10.0));

                    // update view:
                    window.setView(View(FloatRect(0.0f, 0.0f, (float) window.getSize().x, (float) window.getSize().y)));

                    // update game over background size:
                    game_over_background.setSize(Vector2f(window.getSize().x, window.getSize().y));

                    // update game over sprite size:
                    game_over.setScale((float) window.getSize().x / texture.getSize().x, (float) window.getSize().y / texture.getSize().y / 1.2);

                    // update all the buttons and their positions:
                    restart.setCharacterSize(5 * button_size / 6);
                    restart.setOutlineThickness(button_size / 6);
                    restart.setPosition((window.getSize().x - restart.getGlobalBounds().width) / 2, 
                            (window.getSize().y - number_of_buttons * (button_size + 15) - 15));
                    
                    choose_complexity.setCharacterSize(5 * button_size / 6);
                    choose_complexity.setOutlineThickness(button_size / 6);
                    choose_complexity.setPosition((window.getSize().x - choose_complexity.getGlobalBounds().width) / 2, 
                            (window.getSize().y - number_of_buttons * (button_size + 15) - 15) + button_size + 15);
                    
                    to_main_menu.setCharacterSize(5 * button_size / 6);
                    to_main_menu.setOutlineThickness(button_size / 6);
                    to_main_menu.setPosition((window.getSize().x - to_main_menu.getGlobalBounds().width) / 2, 
                            (window.getSize().y - number_of_buttons * (button_size + 15) - 15) + (button_size + 15) * 2);
                    
                    exit.setCharacterSize(5 * button_size / 6);
                    exit.setOutlineThickness(button_size / 6);
                    exit.setPosition((window.getSize().x - exit.getGlobalBounds().width) / 2, 
                            (window.getSize().y - number_of_buttons * (button_size + 15) - 15) + (button_size + 15) * 3);
                    break;
                default:
                    break;
            }
        }

        // clear game window:
        window.clear();

        // draw background:
        window.draw(background);

        // update view:
        // window.setView(View(FloatRect(0.0f, 0.0f, (float) window.getSize().x, (float) window.getSize().y)));

        // draw a board:
        game_board->print_board(window, font, 5 * button_size / 6);
        
        // draw game over:
        window.draw(game_over_background);
        window.draw(game_over);

        // draw restart button:
        window.draw(restart);
        // draw choose complexity button:
        window.draw(choose_complexity);
        // draw main menu button:
        window.draw(to_main_menu);
        // draw exit button:
        window.draw(exit);

        // display what we have just drawn:
        window.display();
    }
}

void pause_menu(RenderWindow& window, Sprite& background, Board* game_board, const Font& font) {
    // counter of the currently chosen button:
    unsigned focused_button_counter = 0;
    // number of buttons:
    const unsigned number_of_buttons = 4;
    // button size:
    double button_size = min(min(60.0, 3.5 * (window.getSize().x - 10.0) / 16), 
        min((window.getSize().y - 50.0 - 15.0 * (number_of_buttons - 1)) / number_of_buttons / 2, 3.75 * (0.27 * window.getSize().x - 5.0f) / 10.0));

    // initialize cell size according the current window size:
    Vector2f cell_size(min(min(40.0f, (float) (0.73 * window.getSize().x - 29.0f) / 10), min(40.0f, ((float) window.getSize().y - 29.0f) / 20)),
                    min(min(40.0f, (float) (0.73 * window.getSize().x - 29.0f) / 10), min(40.0f, ((float) window.getSize().y - 29.0f) / 20)));


    // initalize pause background:
    RectangleShape pause_background;
    pause_background.setSize(Vector2f(window.getSize().x, window.getSize().y));
    pause_background.setFillColor(Color(255, 255, 255, 100));

    // set the pause image:
    Texture texture;
    try {
        if (!texture.loadFromFile("background/pause.png")) 
            throw 0;
    } catch (int e) {
        if (e == 0)
            cout << "Sorry, pause image not found!" << endl;
    }

    // create a pause sprite itself:
    Sprite pause;
    pause.setTexture(texture);
    pause.setScale((float) window.getSize().x / texture.getSize().x, (float) window.getSize().y / texture.getSize().y / 1.5);
    pause.setColor(Color(255, 255, 255, 100));

    // Initialize resume button:
    Text resume = create_button(font, "Resume", button_size,
        Vector2f(window.getSize().x / 2,
            0.85 * (window.getSize().y - number_of_buttons * (button_size + 15) - 15)));

    // Initialize restart button:
    Text restart = create_button(font, "Restart", button_size,
        Vector2f(window.getSize().x / 2,
            0.85 * (window.getSize().y - number_of_buttons * (button_size + 15) - 15) + button_size + 15));

    // Initialize to_main_menu button:
    Text to_main_menu = create_button(font, "Main Menu", button_size,
        Vector2f(window.getSize().x / 2,
            0.85 * (window.getSize().y - number_of_buttons * (button_size + 15) - 15) + (button_size + 15) * 2));

    // Initialize exit button:
    Text exit = create_button(font, "Exit", button_size,
        Vector2f(window.getSize().x / 2,
            0.85 * (window.getSize().y - number_of_buttons * (button_size + 15) - 15) + (button_size + 15) * 3));

    while (window.isOpen()) {
        Event event;

        while (window.pollEvent(event)) {
            switch (event.type) {
                // close window:  
                case Event::Closed:
                    window.close();
                    break;
                // when we are moving mouse:
                case Event::MouseMoved:
                    // unfocus all the buttons:
                    resume.setFillColor(Color(144, 12, 63, 255));
                    resume.setOutlineColor(Color(218, 247, 166, 255));
                    restart.setFillColor(Color(144, 12, 63, 255));
                    restart.setOutlineColor(Color(218, 247, 166, 255));
                    to_main_menu.setFillColor(Color(144, 12, 63, 255));
                    to_main_menu.setOutlineColor(Color(218, 247, 166, 255));
                    exit.setFillColor(Color(144, 12, 63, 255));
                    exit.setOutlineColor(Color(218, 247, 166, 255));

                    // If appropriate mouse position was captured:
                    if (captured_button(window, resume)) {
                        // focus resume button:
                        resume.setFillColor(Color(255, 195, 0, 255));
                        resume.setOutlineColor(Color(8, 0, 93, 255));
                        focused_button_counter = 1;
                    } else if (captured_button(window, restart)) {
                        // focus new restart game button:
                        restart.setFillColor(Color(255, 195, 0, 255));
                        restart.setOutlineColor(Color(8, 0, 93, 255));
                        focused_button_counter = 2;
                    } else if (captured_button(window, to_main_menu)) {
                        // focus to_main_menu button:
                        to_main_menu.setFillColor(Color(255, 195, 0, 255));
                        to_main_menu.setOutlineColor(Color(8, 0, 93, 255));
                        focused_button_counter = 3;
                    } else if (captured_button(window, exit)) {
                        // focus exit button:
                        exit.setFillColor(Color(255, 195, 0, 255));
                        exit.setOutlineColor(Color(8, 0, 93, 255));
                        focused_button_counter = 4;
                    }
                    break;
                case Event::MouseButtonPressed:
                    switch (event.key.code) {
                        case Mouse::Left:
                            // If appropriate mouse position was captured:
                            // 1) Resume game:
                            if (captured_button(window, resume))
                                return;
                            // 2) Restart game:
                            else if (captured_button(window, restart))
                                game(window, background, font, game_board->get_complexity());
                            // 3) Go to main menu:
                            else if (captured_button(window, to_main_menu))
                                main_menu(window, background, font);
                            // 4) Exit:
                            else if (captured_button(window, exit))
                                window.close();
                            break;
                        default:
                            break;
                    }
                    break;
                case Event::KeyPressed:
                    switch (event.key.code) {
                        // if we want to focus (Tab) or push (Enter) some button using keyboard:
                        case Keyboard::Tab:
                        case Keyboard::Return:
                            // focus or push the button according to 
                            // the current focused_button_counter value:
                            switch (focused_button_counter) {
                                // if it is the first press of Tab:
                                case 0:
                                    // in this case, Enter won't do nothing:
                                    if (event.key.code == Keyboard::Return)
                                        break;

                                    // focus resume button:
                                    resume.setFillColor(Color(255, 195, 0, 255));
                                    resume.setOutlineColor(Color(8, 0, 93, 255));
                                    focused_button_counter++;
                                    break;
                                case 1:
                                    // if we have pressed Enter:
                                    if (event.key.code == Keyboard::Return)
                                        // execute resume button:
                                        return;

                                    // unfocus resume button:
                                    resume.setFillColor(Color(144, 12, 63, 255));
                                    resume.setOutlineColor(Color(218, 247, 166, 255));
                                    // focus restart button:
                                    restart.setFillColor(Color(255, 195, 0, 255));
                                    restart.setOutlineColor(Color(8, 0, 93, 255));
                                    focused_button_counter++;
                                    break;
                                case 2:
                                    // if we have pressed Enter:
                                    if (event.key.code == Keyboard::Return)
                                        // execute restart button:
                                        game(window, background, font, game_board->get_complexity());

                                    // unfocus restart button:
                                    restart.setFillColor(Color(144, 12, 63, 255));
                                    restart.setOutlineColor(Color(218, 247, 166, 255));
                                    // focus to_main_menu button:
                                    to_main_menu.setFillColor(Color(255, 195, 0, 255));
                                    to_main_menu.setOutlineColor(Color(8, 0, 93, 255));
                                    focused_button_counter++;
                                    break;
                                case 3:
                                    // if we have pressed Enter:
                                    if (event.key.code == Keyboard::Return)
                                        // execute to_main_menu button:
                                        main_menu(window, background, font);

                                    // unfocus to_main_menu button:
                                    to_main_menu.setFillColor(Color(144, 12, 63, 255));
                                    to_main_menu.setOutlineColor(Color(218, 247, 166, 255));
                                    // focus exit button:
                                    exit.setFillColor(Color(255, 195, 0, 255));
                                    exit.setOutlineColor(Color(8, 0, 93, 255));
                                    focused_button_counter++;
                                    break;
                                case 4:
                                    // if we have pressed Enter:
                                    if (event.key.code == Keyboard::Return)
                                        // execute exit button:
                                        window.close();

                                    // unfocus exit button:
                                    exit.setFillColor(Color(144, 12, 63, 255));
                                    exit.setOutlineColor(Color(218, 247, 166, 255));
                                    // focus resume button:
                                    resume.setFillColor(Color(255, 195, 0, 255));
                                    resume.setOutlineColor(Color(8, 0, 93, 255));
                                    focused_button_counter = 1;
                                    break;
                                default:
                                    break;
                            }
                            break;
                        // if Escape is pushed: 
                        case Keyboard::Escape:
                            // execute pause button:
                            return;
                            break;
                        default:
                            break;
                    }
                // if we have changed window's size:
                case Event::Resized:
                    // set minimal window size:
                    if ((window.getSize().y < 600) || (window.getSize().x < 400))
                        window.setSize(Vector2u(400, 600));
            
                    // update button size:
                    button_size = min(min(60.0, 3.5 * (window.getSize().x - 10.0) / 16), 
                        min((window.getSize().y - 50.0 - 15.0 * (number_of_buttons - 1)) / number_of_buttons / 2, 3.75 * (0.27 * window.getSize().x - 5.0f) / 10.0));

                    // update view:
                    window.setView(View(FloatRect(0.0f, 0.0f, (float) window.getSize().x, (float) window.getSize().y)));

                    // update cell size:
                    cell_size.x = min(min(40.0f, (float) (0.73 * window.getSize().x - 29.0f) / 10), min(40.0f, ((float) window.getSize().y - 29.0f) / 20));
                    cell_size.y = min(min(40.0f, (float) (0.73 * window.getSize().x - 29.0f) / 10), min(40.0f, ((float) window.getSize().y - 29.0f) / 20));
                    game_board->set_cell_size(cell_size);

                    // update pause background:
                    pause_background.setSize(Vector2f(window.getSize().x, window.getSize().y));

                    // update pause sprite: 
                    pause.setScale((float) window.getSize().x / texture.getSize().x, (float) window.getSize().y / texture.getSize().y / 1.5);

                    // update all the buttons and their positions:
                    resume.setCharacterSize(5 * button_size / 6);
                    resume.setOutlineThickness(button_size / 6);
                    resume.setPosition((window.getSize().x - resume.getGlobalBounds().width) / 2, 
                            0.85 * (window.getSize().y - number_of_buttons * (button_size + 15) - 15));
                    
                    restart.setCharacterSize(5 * button_size / 6);
                    restart.setOutlineThickness(button_size / 6);
                    restart.setPosition((window.getSize().x - restart.getGlobalBounds().width) / 2, 
                            0.85 * (window.getSize().y - number_of_buttons * (button_size + 15) - 15) + button_size + 15);
                    
                    to_main_menu.setCharacterSize(5 * button_size / 6);
                    to_main_menu.setOutlineThickness(button_size / 6);
                    to_main_menu.setPosition((window.getSize().x - to_main_menu.getGlobalBounds().width) / 2, 
                            0.85 * (window.getSize().y - number_of_buttons * (button_size + 15) - 15) + (button_size + 15) * 2);
                    
                    exit.setCharacterSize(5 * button_size / 6);
                    exit.setOutlineThickness(button_size / 6);
                    exit.setPosition((window.getSize().x - exit.getGlobalBounds().width) / 2, 
                            0.85 * (window.getSize().y - number_of_buttons * (button_size + 15) - 15) + (button_size + 15) * 3);
                    break;
                default:
                    break;
            }
        }

        // clear game window:
        window.clear();

        // draw background:
        window.draw(background);

        // update view:
        // window.setView(View(FloatRect(0.0f, 0.0f, (float) window.getSize().x, (float) window.getSize().y)));

        // draw a board:
        game_board->print_board(window, font, 5 * button_size / 6);

        // draw pause background:
        window.draw(pause_background);
        //draw pause sprite:
        window.draw(pause);

        // draw resume button:
        window.draw(resume);
        // draw restart button:
        window.draw(restart);
        // draw main menu button:
        window.draw(to_main_menu);
        // draw exit button:
        window.draw(exit);

        // display what we have just drawn:
        window.display();
    }
}

void main_menu(RenderWindow& window, Sprite& background, const Font& font) {
    // counter of the currently chosen button:
    unsigned focused_button_counter = 0;
    // number of buttons:
    const unsigned number_of_buttons = 6;
    // button size:
    double button_size = min(min(60.0f, 3.5f * (window.getSize().x - 10.0f) / 12), 
        (window.getSize().y - 50.0f - 15.0f * (number_of_buttons - 1)) / number_of_buttons);

    // Initialize new single game button:
    Text singleplayer = create_button(font, "Singleplayer", button_size,
        Vector2f(window.getSize().x / 2,
            (window.getSize().y - number_of_buttons * (button_size + 15) - 15) / 2));

    // Initialize new multiplayer game button:
    Text multiplayer = create_button(font, "Multiplayer", button_size,
        Vector2f(window.getSize().x / 2,
            (window.getSize().y - number_of_buttons * (button_size + 15) - 15) / 2 + button_size + 15));;
    
    // Initialize leaderboard button:
    Text leaderboard = create_button(font, "Leaderboard", button_size,
        Vector2f(window.getSize().x / 2,
            (window.getSize().y - number_of_buttons * (button_size + 15) - 15) / 2 + (button_size + 15) * 2));;
    
    // Initialize settings button:
    Text settings = create_button(font, "Settings", button_size,
        Vector2f(window.getSize().x / 2,
            (window.getSize().y - number_of_buttons * (button_size + 15) - 15) / 2 + (button_size + 15) * 3));

    // Initialize authors button:
    Text authors = create_button(font, "Authors", button_size,
        Vector2f(window.getSize().x / 2,
            (window.getSize().y - number_of_buttons * (button_size + 15) - 15) / 2 + (button_size + 15) * 4));
    
    // Initialize exit button:
    Text exit = create_button(font, "Exit", button_size,
        Vector2f(window.getSize().x / 2,
            (window.getSize().y - number_of_buttons * (button_size + 15) - 15) / 2 + (button_size + 15) * 5));
    
    while (window.isOpen()) {
        Event event;

        while (window.pollEvent(event)) {
            switch (event.type) {
                // close window:  
                case Event::Closed:
                    window.close();
                    break;
                // when we are moving mouse:
                case Event::MouseMoved:
                    // unfocus all the buttons:
                    singleplayer.setFillColor(Color(144, 12, 63, 255));
                    singleplayer.setOutlineColor(Color(218, 247, 166, 255));
                    multiplayer.setFillColor(Color(144, 12, 63, 255));
                    multiplayer.setOutlineColor(Color(218, 247, 166, 255));
                    leaderboard.setFillColor(Color(144, 12, 63, 255));
                    leaderboard.setOutlineColor(Color(218, 247, 166, 255));
                    settings.setFillColor(Color(144, 12, 63, 255));
                    settings.setOutlineColor(Color(218, 247, 166, 255));
                    authors.setFillColor(Color(144, 12, 63, 255));
                    authors.setOutlineColor(Color(218, 247, 166, 255));
                    exit.setFillColor(Color(144, 12, 63, 255));
                    exit.setOutlineColor(Color(218, 247, 166, 255));

                    // If appropriate mouse position was captured:
                    if (captured_button(window, singleplayer)) {
                        // focus new single game button:
                        singleplayer.setFillColor(Color(255, 195, 0, 255));
                        singleplayer.setOutlineColor(Color(8, 0, 93, 255));
                        focused_button_counter = 1;
                    } else if (captured_button(window, multiplayer)) {
                        // focus choose complexity button:
                        multiplayer.setFillColor(Color(255, 195, 0, 255));
                        multiplayer.setOutlineColor(Color(8, 0, 93, 255));
                        focused_button_counter = 2;
                    } else if (captured_button(window, leaderboard)) {
                        // focus leaderboard button:
                        leaderboard.setFillColor(Color(255, 195, 0, 255));
                        leaderboard.setOutlineColor(Color(8, 0, 93, 255));
                        focused_button_counter = 3;
                    } else if (captured_button(window, settings)) {
                        // focus settings button:
                        settings.setFillColor(Color(255, 195, 0, 255));
                        settings.setOutlineColor(Color(8, 0, 93, 255));
                        focused_button_counter = 4;
                    } else if (captured_button(window, authors)) {
                        // focus authors button:
                        authors.setFillColor(Color(255, 195, 0, 255));
                        authors.setOutlineColor(Color(8, 0, 93, 255));
                        focused_button_counter = 5;
                    } else if (captured_button(window, exit)) {
                        // focus exit button:
                        exit.setFillColor(Color(255, 195, 0, 255));
                        exit.setOutlineColor(Color(8, 0, 93, 255));
                        focused_button_counter = 6;
                    }
                    break;
                case Event::MouseButtonPressed:
                    switch (event.key.code) {
                        case Mouse::Left:
                            // If appropriate mouse position was captured:
                            // 1) Start new single game:
                            if (captured_button(window, singleplayer))
                                complexity_menu(window, background, font);
                            // 2) Go to multiplayer menu:
                            else if (captured_button(window, multiplayer))
                                multiplayer_menu(window, background, font);
                            // 3) Check highscores:
                            else if (captured_button(window, leaderboard))
                                break;
                            // 4) Go to settings:
                            else if (captured_button(window, settings))
                                break;
                            // 5) Who wrote this masterpiece?
                            else if (captured_button(window, authors))
                                break;
                            // 6) Exit program:
                            else if (captured_button(window, exit))
                                window.close();
                            break;
                        default:
                            break;
                    }
                case Event::KeyPressed:
                    switch (event.key.code) {
                        // if we want to focus (Tab) or push (Enter) some button using keyboard:
                        case Keyboard::Tab:
                        case Keyboard::Return:
                            // focus or push the button according to 
                            // the current focused_button_counter value:
                            switch (focused_button_counter) {
                                // if it is the first press of Tab:
                                case 0:
                                    // in this case, Enter won't do nothing:
                                    if (event.key.code == Keyboard::Return)
                                        break;

                                    // focus new single game button:
                                    singleplayer.setFillColor(Color(255, 195, 0, 255));
                                    singleplayer.setOutlineColor(Color(8, 0, 93, 255));
                                    focused_button_counter++;
                                    break;
                                case 1:
                                    // if we have pressed Enter:
                                    if (event.key.code == Keyboard::Return)
                                        // execute singleplayer button:
                                        complexity_menu(window, background, font);

                                    // unfocus singleplayer button:
                                    singleplayer.setFillColor(Color(144, 12, 63, 255));
                                    singleplayer.setOutlineColor(Color(218, 247, 166, 255));
                                    // focus multiplayer button:
                                    multiplayer.setFillColor(Color(255, 195, 0, 255));
                                    multiplayer.setOutlineColor(Color(8, 0, 93, 255));
                                    focused_button_counter++;
                                    break;
                                case 2:
                                    // if we have pressed Enter:
                                    if (event.key.code == Keyboard::Return)
                                        // Go to multiplayer menu:
                                        multiplayer_menu(window, background, font);

                                    // unfocus multiplayer button:
                                    multiplayer.setFillColor(Color(144, 12, 63, 255));
                                    multiplayer.setOutlineColor(Color(218, 247, 166, 255));
                                    // focus leaderboard button:
                                    leaderboard.setFillColor(Color(255, 195, 0, 255));
                                    leaderboard.setOutlineColor(Color(8, 0, 93, 255));
                                    focused_button_counter++;
                                    break;
                                case 3:
                                    // if we have pressed Enter:
                                    if (event.key.code == Keyboard::Return)
                                        // execute leaderboard button:
                                        break;

                                    // unfocus leaderboard button:
                                    leaderboard.setFillColor(Color(144, 12, 63, 255));
                                    leaderboard.setOutlineColor(Color(218, 247, 166, 255));
                                    // focus settings button:
                                    settings.setFillColor(Color(255, 195, 0, 255));
                                    settings.setOutlineColor(Color(8, 0, 93, 255));
                                    focused_button_counter++;
                                    break;
                                case 4:
                                    // if we have pressed Enter:
                                    if (event.key.code == Keyboard::Return)
                                        // execute settings button:
                                        break;

                                    // unfocus settings button:
                                    settings.setFillColor(Color(144, 12, 63, 255));
                                    settings.setOutlineColor(Color(218, 247, 166, 255));
                                    // focus authors button:
                                    authors.setFillColor(Color(255, 195, 0, 255));
                                    authors.setOutlineColor(Color(8, 0, 93, 255));
                                    focused_button_counter++;
                                    break;
                                case 5:
                                    // if we have pressed Enter:
                                    if (event.key.code == Keyboard::Return)
                                        // execute authors button:
                                        break;

                                    // unfocus authors button:
                                    authors.setFillColor(Color(144, 12, 63, 255));
                                    authors.setOutlineColor(Color(218, 247, 166, 255));
                                    // focus exit button:
                                    exit.setFillColor(Color(255, 195, 0, 255));
                                    exit.setOutlineColor(Color(8, 0, 93, 255));
                                    focused_button_counter++;
                                    break;
                                case 6:
                                    // if we have pressed Enter:
                                    if (event.key.code == Keyboard::Return)
                                        // execute exit button:
                                        window.close();

                                    // unfocus exit button:
                                    exit.setFillColor(Color(144, 12, 63, 255));
                                    exit.setOutlineColor(Color(218, 247, 166, 255));
                                    // focus singleplayer button:
                                    singleplayer.setFillColor(Color(255, 195, 0, 255));
                                    singleplayer.setOutlineColor(Color(8, 0, 93, 255));
                                    focused_button_counter = 1;
                                    break;
                                default:
                                    break;
                            }
                        default:
                            break;
                    }
                // if we have changed window's size:
                case Event::Resized:
                    // set minimal window size:
                    if ((window.getSize().y < 600) || (window.getSize().x < 400))
                        window.setSize(Vector2u(400, 600));
            
                    // update button size:
                    button_size = min(min(60.0f, 3.5f * (window.getSize().x - 10.0f) / 12), 
                        (window.getSize().y - 50.0f - 15.0f * (number_of_buttons - 1)) / number_of_buttons);

                    // update view:
                    window.setView(View(FloatRect(0.0f, 0.0f, (float) window.getSize().x, (float) window.getSize().y)));

                    // update all the buttons and their positions:
                    singleplayer.setCharacterSize(5 * button_size / 6);
                    singleplayer.setOutlineThickness(button_size / 6);
                    singleplayer.setPosition((window.getSize().x - singleplayer.getGlobalBounds().width) / 2, 
                            (window.getSize().y - number_of_buttons * (button_size + 15) - 15) / 2);
                    
                    multiplayer.setCharacterSize(5 * button_size / 6);
                    multiplayer.setOutlineThickness(button_size / 6);
                    multiplayer.setPosition((window.getSize().x - multiplayer.getGlobalBounds().width) / 2, 
                            (window.getSize().y - number_of_buttons * (button_size + 15) - 15) / 2 + button_size + 15);
                    
                    leaderboard.setCharacterSize(5 * button_size / 6);
                    leaderboard.setOutlineThickness(button_size / 6);
                    leaderboard.setPosition((window.getSize().x - leaderboard.getGlobalBounds().width) / 2, 
                            (window.getSize().y - number_of_buttons * (button_size + 15) - 15) / 2 + (button_size + 15) * 2);
                    
                    settings.setCharacterSize(5 * button_size / 6);
                    settings.setOutlineThickness(button_size / 6);
                    settings.setPosition((window.getSize().x - settings.getGlobalBounds().width) / 2, 
                            (window.getSize().y - number_of_buttons * (button_size + 15) - 15) / 2 + (button_size + 15) * 3);
                    
                    authors.setCharacterSize(5 * button_size / 6);
                    authors.setOutlineThickness(button_size / 6);
                    authors.setPosition((window.getSize().x - authors.getGlobalBounds().width) / 2, 
                            (window.getSize().y - number_of_buttons * (button_size + 15) - 15) / 2 + (button_size + 15) * 4);
                    
                    exit.setCharacterSize(5 * button_size / 6);
                    exit.setOutlineThickness(button_size / 6);
                    exit.setPosition((window.getSize().x - exit.getGlobalBounds().width) / 2, 
                            (window.getSize().y - number_of_buttons * (button_size + 15) - 15) / 2 + (button_size + 15) * 5);
                    break;
                default:
                    break;
            }
        }

        // Clear window:
        window.clear();
        
        // Draw a background:
        window.draw(background);
        
        // Update view:
        // window.setView(View(FloatRect(0.0f, 0.0f, (float) window.getSize().x, (float) window.getSize().y)));

        // Draw the buttons:
        window.draw(singleplayer);
        window.draw(multiplayer);
        window.draw(leaderboard);
        window.draw(settings);
        window.draw(authors);
        window.draw(exit);
        window.display();
    }    
}

void multiplayer_menu(RenderWindow& window, Sprite& background, const Font& font) { 
    // counter of the currently chosen button:
    unsigned focused_button_counter = 0;
    // number of buttons:
    const unsigned number_of_buttons = 4;
    // button size:
    double button_size = min(min(60.0f, 3.5f * (window.getSize().x - 10.0f) / 12), 
        (window.getSize().y - 50.0f - 15.0f * (number_of_buttons - 1)) / number_of_buttons);
    
    // Vector2u window_size;

    // Initialize multiplayer menu title:
    Text multiplayer_title = create_button(font, "Multiplayer", button_size,
        Vector2f(window.getSize().x / 2 - 10.0f,
            (window.getSize().y - number_of_buttons * (button_size + 15) - 25) / 2), false);

    // Initialize new_session button:
    Text new_session = create_button(font, "New Session", button_size,
        Vector2f(window.getSize().x / 2,
            (window.getSize().y - number_of_buttons * (button_size + 15) - 25) / 2 + button_size + 20.0f));
    
    // Initialize find_server button:
    Text find_server = create_button(font, "Find Server", button_size,
        Vector2f(window.getSize().x / 2,
            (window.getSize().y - number_of_buttons * (button_size + 15) - 25) / 2 + (button_size + 15.0f) * 2 + 5.0f));
      
    // Initialize back button:
    Text back = create_button(font, "Back", button_size,
        Vector2f(window.getSize().x / 2,
            (window.getSize().y - number_of_buttons * (button_size + 15) - 25) / 2 + (button_size + 15.0f) * 3 + 10.0f));
   
    while (window.isOpen()) {
        Event event;

        while (window.pollEvent(event)) {
            switch (event.type) {
                // close window:  
                case Event::Closed:
                    window.close();
                    break;
                // when we are moving mouse:
                case Event::MouseMoved:
                    // unfocus all the buttons:
                    new_session.setFillColor(Color(144, 12, 63, 255));
                    new_session.setOutlineColor(Color(218, 247, 166, 255));
                    find_server.setFillColor(Color(144, 12, 63, 255));
                    find_server.setOutlineColor(Color(218, 247, 166, 255));
                    back.setFillColor(Color(144, 12, 63, 255));
                    back.setOutlineColor(Color(218, 247, 166, 255));

                    // If appropriate mouse position was captured:
                    if (captured_button(window, new_session)) {
                        // focus new_session button:
                        new_session.setFillColor(Color(255, 195, 0, 255));
                        new_session.setOutlineColor(Color(8, 0, 93, 255));
                        focused_button_counter = 1;
                    } else if (captured_button(window, find_server)) {
                        // focus find_server button:
                        find_server.setFillColor(Color(255, 195, 0, 255));
                        find_server.setOutlineColor(Color(8, 0, 93, 255));
                        focused_button_counter = 2;  
                    } else if (captured_button(window, back)) {
                        // focus back button:
                        back.setFillColor(Color(255, 195, 0, 255));
                        back.setOutlineColor(Color(8, 0, 93, 255));
                        focused_button_counter = 3;
                    }
                    break;
                case Event::MouseButtonPressed:
                    switch (event.key.code) {
                        case Mouse::Left:
                            // If appropriate mouse position was captured:
                            // 1) Starting to play in the easy mode:
                            if (captured_button(window, new_session))
                                game(window, background, font, 1);
                            // 2) Starting to play in the normal mode:
                            else if (captured_button(window, find_server))
                                game(window, background, font, 2);
                            // 3) Go back:
                            else if (captured_button(window, back))
                                return;
                            break;
                        default:
                            break;
                    }
                case Event::KeyPressed:
                    switch (event.key.code) {
                        // if we want to focus (Tab) or push (Enter) some button using keyboard:
                        case Keyboard::Tab:
                        case Keyboard::Return:
                            // focus or push the button according to 
                            // the current focused_button_counter value:
                            switch (focused_button_counter) {
                                // if it is the first press of Tab:
                                case 0:
                                    // in this case, Enter won't do nothing:
                                    if (event.key.code == Keyboard::Return)
                                        break;

                                    // focus new_session button:
                                    new_session.setFillColor(Color(255, 195, 0, 255));
                                    new_session.setOutlineColor(Color(8, 0, 93, 255));
                                    focused_button_counter++;
                                    break;
                                case 1:
                                    // if we have pressed Enter:
                                    if (event.key.code == Keyboard::Return)
                                        // execute new_session button:
                                        game(window, background, font, 1);

                                    // unfocus new_session button:
                                    new_session.setFillColor(Color(144, 12, 63, 255));
                                    new_session.setOutlineColor(Color(218, 247, 166, 255));
                                    // focus find_server button:
                                    find_server.setFillColor(Color(255, 195, 0, 255));
                                    find_server.setOutlineColor(Color(8, 0, 93, 255));
                                    focused_button_counter++;
                                    break;
                                case 2:
                                    // if we have pressed Enter:
                                    if (event.key.code == Keyboard::Return)
                                        // execute find_server button:
                                        game(window, background, font, 2);

                                    // unfocus find_server button:
                                    find_server.setFillColor(Color(144, 12, 63, 255));
                                    find_server.setOutlineColor(Color(218, 247, 166, 255));
                                    // focus back button:
                                    back.setFillColor(Color(255, 195, 0, 255));
                                    back.setOutlineColor(Color(8, 0, 93, 255));
                                    focused_button_counter++;
                                    break;
                                case 3:
                                    // if we have pressed Enter:
                                    if (event.key.code == Keyboard::Return)
                                        // execute back button:
                                        main_menu(window, background, font);

                                    // unfocus back button:
                                    back.setFillColor(Color(144, 12, 63, 255));
                                    back.setOutlineColor(Color(218, 247, 166, 255));
                                    // focus new_session button:
                                    new_session.setFillColor(Color(255, 195, 0, 255));
                                    new_session.setOutlineColor(Color(8, 0, 93, 255));
                                    focused_button_counter = 1;
                                    break;
                                default:
                                    break;
                            }
                        default:
                            break;
                    }
                // if we have changed window's size:
                case Event::Resized:
                    // set minimal window size:
                    if (window.getSize().y < 600)
                        window.setSize(Vector2u (window.getSize().x, 600));

                    // update button size:
                    button_size = min(min(60.0f, 3.5f * (window.getSize().x - 10.0f) / 18), 
                        (window.getSize().y - 50.0f - 15.0f * (number_of_buttons - 1)) / number_of_buttons);

                    // update view:
                    window.setView(View(FloatRect(0.0f, 0.0f, (float) window.getSize().x, (float) window.getSize().y)));

                    // update all the buttons and their positions:
                    multiplayer_title.setCharacterSize(5 * button_size / 6);
                    multiplayer_title.setPosition((window.getSize().x - multiplayer_title.getGlobalBounds().width) / 2 - 10.0f, 
                            (window.getSize().y - number_of_buttons * (button_size + 15) - 25) / 2);

                    new_session.setCharacterSize(5 * button_size / 6);
                    new_session.setOutlineThickness(button_size / 6);
                    new_session.setPosition((window.getSize().x - new_session.getGlobalBounds().width) / 2, 
                            (window.getSize().y - number_of_buttons * (button_size + 15) - 25) / 2 + button_size + 20.0f);
                    
                    find_server.setCharacterSize(5 * button_size / 6);
                    find_server.setOutlineThickness(button_size / 6);
                    find_server.setPosition((window.getSize().x - find_server.getGlobalBounds().width) / 2, 
                            (window.getSize().y - number_of_buttons * (button_size + 15) - 25) / 2 + (button_size + 15.0f) * 2 + 5.0f);
                    
                    back.setCharacterSize(5 * button_size / 6);
                    back.setOutlineThickness(button_size / 6);
                    back.setPosition((window.getSize().x - back.getGlobalBounds().width) / 2, 
                            (window.getSize().y - number_of_buttons * (button_size + 15) - 25) / 2 + (button_size + 15.0f) * 3 + 10.0f);
                    break;
                default:
                    break;
            }
        }

        // Clear window:
        window.clear();

        // Draw background:
        window.draw(background);

        // Update view:
        // window.setView(View(FloatRect(0.0f, 0.0f, (float) window.getSize().x, (float) window.getSize().y)));

        // Draw a menu:
        window.draw(multiplayer_title);
        window.draw(new_session);
        window.draw(find_server);
        window.draw(back);
        window.display();
    }
};

void complexity_menu(RenderWindow& window, Sprite& background, const Font& font) {
    // counter of the currently chosen button:
    unsigned focused_button_counter = 0;
    // number of buttons:
    const unsigned number_of_buttons = 5;
    // button size:
    double button_size = min(min(60.0f, 3.5f * (window.getSize().x - 10.0f) / 12), 
        (window.getSize().y - 50.0f - 15.0f * (number_of_buttons - 1)) / number_of_buttons);
    
    // Vector2u window_size;

    // Initialize complexity title:
    Text complexity_title = create_button(font, "Choose complexity", button_size,
        Vector2f(window.getSize().x / 2 - 15.0f,
            (window.getSize().y - number_of_buttons * (button_size + 15) - 25) / 2), false);

    // Initialize mechanics button:
    Text mechanics = create_button(font, "Mechanics", button_size,
        Vector2f(window.getSize().x / 2,
            (window.getSize().y - number_of_buttons * (button_size + 15) - 25) / 2 + button_size + 20.0f));
    
    // Initialize STIC button:
    Text STIC = create_button(font, "STIC", button_size,
        Vector2f(window.getSize().x / 2,
            (window.getSize().y - number_of_buttons * (button_size + 15) - 25) / 2 + (button_size + 15.0f) * 2 + 5.0f));
    
    // Initialize applied mathematics button:
    Text applied_mathematics = create_button(font, "Applied Mathematics", button_size,
        Vector2f(window.getSize().x / 2,
            (window.getSize().y - number_of_buttons * (button_size + 15) - 25) / 2 + (button_size + 15.0f) * 3 + 5.0f));
    
    // Initialize back button:
    Text back = create_button(font, "Back", button_size,
        Vector2f(window.getSize().x / 2,
            (window.getSize().y - number_of_buttons * (button_size + 15) - 25) / 2 + (button_size + 15.0f) * 4 + 10.0f));
   
    while (window.isOpen()) {
        Event event;

        while (window.pollEvent(event)) {
            switch (event.type) {
                // close window:  
                case Event::Closed:
                    window.close();
                    break;
                // when we are moving mouse:
                case Event::MouseMoved:
                    // unfocus all the buttons:
                    mechanics.setFillColor(Color(144, 12, 63, 255));
                    mechanics.setOutlineColor(Color(218, 247, 166, 255));
                    STIC.setFillColor(Color(144, 12, 63, 255));
                    STIC.setOutlineColor(Color(218, 247, 166, 255));
                    applied_mathematics.setFillColor(Color(144, 12, 63, 255));
                    applied_mathematics.setOutlineColor(Color(218, 247, 166, 255));
                    back.setFillColor(Color(144, 12, 63, 255));
                    back.setOutlineColor(Color(218, 247, 166, 255));

                    // If appropriate mouse position was captured:
                    if (captured_button(window, mechanics)) {
                        // focus mechanics button:
                        mechanics.setFillColor(Color(255, 195, 0, 255));
                        mechanics.setOutlineColor(Color(8, 0, 93, 255));
                        focused_button_counter = 1;
                    } else if (captured_button(window, STIC)) {
                        // focus STIC button:
                        STIC.setFillColor(Color(255, 195, 0, 255));
                        STIC.setOutlineColor(Color(8, 0, 93, 255));
                        focused_button_counter = 2;
                    } else if (captured_button(window, applied_mathematics)) {
                        // focus applied mathematics button:
                        applied_mathematics.setFillColor(Color(255, 195, 0, 255));
                        applied_mathematics.setOutlineColor(Color(8, 0, 93, 255));
                        focused_button_counter = 3;             
                    } else if (captured_button(window, back)) {
                        // focus back button:
                        back.setFillColor(Color(255, 195, 0, 255));
                        back.setOutlineColor(Color(8, 0, 93, 255));
                        focused_button_counter = 4;
                    }
                    break;
                case Event::MouseButtonPressed:
                    switch (event.key.code) {
                        case Mouse::Left:
                            // If appropriate mouse position was captured:
                            // 1) Starting to play in the easy mode:
                            if (captured_button(window, mechanics))
                                game(window, background, font, 1);
                            // 2) Starting to play in the normal mode:
                            else if (captured_button(window, STIC))
                                game(window, background, font, 2);
                            // 3) Starting to play in the hard mode:
                            else if (captured_button(window, applied_mathematics))
                                game(window, background, font, 3);
                            // 4) Go back:
                            else if (captured_button(window, back))
                                return;
                            break;
                        default:
                            break;
                    }
                case Event::KeyPressed:
                    switch (event.key.code) {
                        // if we want to focus (Tab) or push (Enter) some button using keyboard:
                        case Keyboard::Tab:
                        case Keyboard::Return:
                            // focus or push the button according to 
                            // the current focused_button_counter value:
                            switch (focused_button_counter) {
                                // if it is the first press of Tab:
                                case 0:
                                    // in this case, Enter won't do nothing:
                                    if (event.key.code == Keyboard::Return)
                                        break;

                                    // focus mechanics button:
                                    mechanics.setFillColor(Color(255, 195, 0, 255));
                                    mechanics.setOutlineColor(Color(8, 0, 93, 255));
                                    focused_button_counter++;
                                    break;
                                case 1:
                                    // if we have pressed Enter:
                                    if (event.key.code == Keyboard::Return)
                                        // execute mechanics button:
                                        game(window, background, font, 1);

                                    // unfocus mechanics button:
                                    mechanics.setFillColor(Color(144, 12, 63, 255));
                                    mechanics.setOutlineColor(Color(218, 247, 166, 255));
                                    // focus STIC button:
                                    STIC.setFillColor(Color(255, 195, 0, 255));
                                    STIC.setOutlineColor(Color(8, 0, 93, 255));
                                    focused_button_counter++;
                                    break;
                                case 2:
                                    // if we have pressed Enter:
                                    if (event.key.code == Keyboard::Return)
                                        // execute STIC button:
                                        game(window, background, font, 2);

                                    // unfocus STIC button:
                                    STIC.setFillColor(Color(144, 12, 63, 255));
                                    STIC.setOutlineColor(Color(218, 247, 166, 255));
                                    // focus applied mathematics button:
                                    applied_mathematics.setFillColor(Color(255, 195, 0, 255));
                                    applied_mathematics.setOutlineColor(Color(8, 0, 93, 255));
                                    focused_button_counter++;
                                    break;
                                case 3:
                                    // if we have pressed Enter:
                                    if (event.key.code == Keyboard::Return)
                                        // execute applied mathematics button:
                                        game(window, background, font, 3);

                                    // unfocus applied mathematics button:
                                    applied_mathematics.setFillColor(Color(144, 12, 63, 255));
                                    applied_mathematics.setOutlineColor(Color(218, 247, 166, 255));
                                    // focus back button:
                                    back.setFillColor(Color(255, 195, 0, 255));
                                    back.setOutlineColor(Color(8, 0, 93, 255));
                                    focused_button_counter++;
                                    break;
                                case 4:
                                    // if we have pressed Enter:
                                    if (event.key.code == Keyboard::Return)
                                        // execute back button:
                                        main_menu(window, background, font);

                                    // unfocus back button:
                                    back.setFillColor(Color(144, 12, 63, 255));
                                    back.setOutlineColor(Color(218, 247, 166, 255));
                                    // focus mechanics button:
                                    mechanics.setFillColor(Color(255, 195, 0, 255));
                                    mechanics.setOutlineColor(Color(8, 0, 93, 255));
                                    focused_button_counter = 1;
                                    break;
                                default:
                                    break;
                            }
                        default:
                            break;
                    }
                // if we have changed window's size:
                case Event::Resized:
                    // set minimal window size:
                    if (window.getSize().y < 600)
                        window.setSize(Vector2u (window.getSize().x, 600));

                    // update button size:
                    button_size = min(min(60.0f, 3.5f * (window.getSize().x - 10.0f) / 18), 
                        (window.getSize().y - 50.0f - 15.0f * (number_of_buttons - 1)) / number_of_buttons);

                    // update view:
                    window.setView(View(FloatRect(0.0f, 0.0f, (float) window.getSize().x, (float) window.getSize().y)));

                    // update all the buttons and their positions:
                    complexity_title.setCharacterSize(5 * button_size / 6);
                    complexity_title.setPosition((window.getSize().x - complexity_title.getGlobalBounds().width) / 2 - 15.0f, 
                            (window.getSize().y - number_of_buttons * (button_size + 15) - 25) / 2);

                    mechanics.setCharacterSize(5 * button_size / 6);
                    mechanics.setOutlineThickness(button_size / 6);
                    mechanics.setPosition((window.getSize().x - mechanics.getGlobalBounds().width) / 2, 
                            (window.getSize().y - number_of_buttons * (button_size + 15) - 25) / 2 + button_size + 20.0f);
                    
                    STIC.setCharacterSize(5 * button_size / 6);
                    STIC.setOutlineThickness(button_size / 6);
                    STIC.setPosition((window.getSize().x - STIC.getGlobalBounds().width) / 2, 
                            (window.getSize().y - number_of_buttons * (button_size + 15) - 25) / 2 + (button_size + 15.0f) * 2 + 5.0f);
                    
                    applied_mathematics.setCharacterSize(5 * button_size / 6);
                    applied_mathematics.setOutlineThickness(button_size / 6);
                    applied_mathematics.setPosition((window.getSize().x - applied_mathematics.getGlobalBounds().width) / 2, 
                            (window.getSize().y - number_of_buttons * (button_size + 15) - 25) / 2 + (button_size + 15.0f) * 3 + 5.0f);
                    
                    back.setCharacterSize(5 * button_size / 6);
                    back.setOutlineThickness(button_size / 6);
                    back.setPosition((window.getSize().x - back.getGlobalBounds().width) / 2, 
                            (window.getSize().y - number_of_buttons * (button_size + 15) - 25) / 2 + (button_size + 15.0f) * 4 + 10.0f);
                    break;
                default:
                    break;
            }
        }

        // Clear window:
        window.clear();

        // Draw background:
        window.draw(background);

        // Update view:
        // window.setView(View(FloatRect(0.0f, 0.0f, (float) window.getSize().x, (float) window.getSize().y)));

        // Draw a menu:
        window.draw(complexity_title);
        window.draw(mechanics);
        window.draw(STIC);
        window.draw(applied_mathematics);
        window.draw(back);
        window.display();
    }
};