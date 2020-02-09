#define menuCPP

#include "menu.hpp"

// new button initialization function (for code reduction):
Text create_button(Font& font, const string title, const double& button_size,
    Vector2f position, const bool& outline, const unsigned& center_coefficient) {
    // Initialize new button:
    Text new_button;
    new_button.setFont(font);
    new_button.setCharacterSize(5 * button_size / 6);
    new_button.setStyle(Text::Bold);
    new_button.setString(title);
    
    if (outline) {
        new_button.setOutlineThickness(button_size / 6);
        new_button.setOutlineColor(COLOR_LIGHT_GREEN);
        new_button.setFillColor(COLOR_DARK_VIOLET);
    } else
        new_button.setFillColor(COLOR_LIGHT_GREEN);
    if (center_coefficient != 0)
        position.x -= new_button.getGlobalBounds().width / center_coefficient;
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

void Menu::game(const unsigned& complexity) {
    // counter of the currently chosen button:
    unsigned focused_button_counter = 0;
    // button size:
    double button_size = min(60.0, 3.75 * (0.27 * window.getSize().x - 5.0f) / BOARD_GRID_WIDTH);

    // initialize cell size according the current window size:
    Vector2f cell_size(min(min(40.0f, (float) (0.73 * window.getSize().x - 29.0f) / BOARD_GRID_WIDTH), min(40.0f, ((float) window.getSize().y - 29.0f) / 20)),
                    min(min(40.0f, (float) (0.73 * window.getSize().x - 29.0f) / BOARD_GRID_WIDTH), min(40.0f, ((float) window.getSize().y - 29.0f) / 20)));

    // create the game board: 
    Board* game_board = new Board(window, complexity, cell_size);

    // Initialize pause button:
    Text pause = create_button(font, "Pause", button_size,
        Vector2f((cell_size.x + 1) * game_board->get_x_dim() + 29.0f, 4 * button_size + 125.0f), true, 0);

    figure_state _figure_state = DESCEND_FIGURE;
    int count_change_figure = DEF_COU_CHA_FIG;

    unsigned descend_counter = 0;

    // Using a thread to fall down:
    game_board->set_descend_thread(new Thread([&] () {
        for ( ; window.isOpen(); descend_counter++) {
            if (descend_counter >= 30 / complexity || _figure_state == CHANGE_FIGURE) {
                if(_figure_state == STOP_FIGURE)
                    count_change_figure--;
                if(count_change_figure <= 0)
                    _figure_state = CHANGE_FIGURE;
                if(_figure_state == DESCEND_FIGURE)
                    _figure_state = game_board->step_down() ? DESCEND_FIGURE : STOP_FIGURE; 
                // if we can't move down no more:
                if (_figure_state == CHANGE_FIGURE) {

                    count_change_figure = DEF_COU_CHA_FIG;
                    _figure_state = DESCEND_FIGURE;
                    // check for the full lines:
                    game_board->fix_current_figure();
                    game_board->erase_lines(complexity);

                    // putting next figure into a current figure:
                    game_board->set_current_figure(game_board->get_next_figure());
                    // adding new current figure on the board:
                    game_board->add_figure();
                    // creating the next figure:
                    game_board->set_next_figure(game_board->create_figure());
                }

                descend_counter = 0;
            }

            sf::sleep(seconds(0.015f));
        }
    }));

    game_board->get_descend_thread()->launch();

    // We are using descend counter to manage the figures' fall rate:
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
                    pause.setFillColor(COLOR_DARK_VIOLET);
                    pause.setOutlineColor(COLOR_LIGHT_GREEN);

                    // If appropriate mouse position was captured:
                    if (captured_button(window, pause)) {
                        // focus pause button:
                        pause.setFillColor(COLOR_YELLOW);
                        pause.setOutlineColor(COLOR_DARK_BLUE);
                        focused_button_counter = 1;
                    }
                    break;
                case Event::MouseButtonPressed:
                    switch (event.key.code) {
                        case Mouse::Left:
                            // If appropriate mouse position was captured:
                            // 1) pause game:
                            if (captured_button(window, pause)) {
                                game_board->get_descend_thread()->terminate();
                                // execute pause button:
                                pause_menu(game_board);
                                game_board->get_descend_thread()->launch();
                                        
                                // update cell size:
                                cell_size.x = min(min(40.0f, (float) (0.73 * window.getSize().x - 29.0f) / BOARD_GRID_WIDTH), min(40.0f, ((float) window.getSize().y - 29.0f) / 20));
                                cell_size.y = min(min(40.0f, (float) (0.73 * window.getSize().x - 29.0f) / BOARD_GRID_WIDTH), min(40.0f, ((float) window.getSize().y - 29.0f) / 20));
                                game_board->set_cell_size(cell_size);

                                // update button size:
                                button_size = min(60.0, 3.75 * (0.27 * window.getSize().x - 5.0f) / BOARD_GRID_WIDTH);

                                // update all the buttons and their positions:
                                pause.setCharacterSize(5 * button_size / 6);
                                pause.setOutlineThickness(button_size / 6);
                                pause.setPosition((cell_size.x + 1) * game_board->get_x_dim() + 29.0f, 4 * button_size + 125.0f);
                        
                                // unfocus pause button:
                                pause.setFillColor(COLOR_DARK_VIOLET);
                                pause.setOutlineColor(COLOR_LIGHT_GREEN);
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

                                    // focus pause button:
                                    pause.setFillColor(COLOR_YELLOW);
                                    pause.setOutlineColor(COLOR_DARK_BLUE);
                                    focused_button_counter++;
                                    break;
                                case 1:
                                    // if we have pressed Enter:
                                    if (event.key.code == Keyboard::Return) {
                                        game_board->get_descend_thread()->terminate();
                                        // execute pause button:
                                        pause_menu(game_board);
                                        game_board->get_descend_thread()->launch();

                                        // update cell size:
                                        cell_size.x = min(min(40.0f, (float) (0.73 * window.getSize().x - 29.0f) / BOARD_GRID_WIDTH), min(40.0f, ((float) window.getSize().y - 29.0f) / 20));
                                        cell_size.y = min(min(40.0f, (float) (0.73 * window.getSize().x - 29.0f) / BOARD_GRID_WIDTH), min(40.0f, ((float) window.getSize().y - 29.0f) / 20));
                                        game_board->set_cell_size(cell_size);

                                        // update button size:
                                        button_size = min(60.0, 3.75 * (0.27 * window.getSize().x - 5.0f) / BOARD_GRID_WIDTH);
                                    
                                        // update all the buttons and their positions:
                                        pause.setCharacterSize(5 * button_size / 6);
                                        pause.setOutlineThickness(button_size / 6);
                                        pause.setPosition((cell_size.x + 1) * game_board->get_x_dim() + 29.0f, 4 * button_size + 125.0f);
                                    }

                                    // unfocus pause button:
                                    pause.setFillColor(COLOR_DARK_VIOLET);
                                    pause.setOutlineColor(COLOR_LIGHT_GREEN);
                                    focused_button_counter = 0;
                                    break;
                                default:
                                    break;
                            }
                            break;
                        // if Escape is pushed: 
                        case Keyboard::Escape:
                            game_board->get_descend_thread()->terminate();
                            // execute pause button:
                            pause_menu(game_board);
                            game_board->get_descend_thread()->launch();

                            // update cell size:
                            cell_size.x = min(min(40.0f, (float) (0.73 * window.getSize().x - 29.0f) / BOARD_GRID_WIDTH), min(40.0f, ((float) window.getSize().y - 29.0f) / 20));
                            cell_size.y = min(min(40.0f, (float) (0.73 * window.getSize().x - 29.0f) / BOARD_GRID_WIDTH), min(40.0f, ((float) window.getSize().y - 29.0f) / 20));
                            game_board->set_cell_size(cell_size);

                            // update button size:
                            button_size = min(60.0, 3.75 * (0.27 * window.getSize().x - 5.0f) / BOARD_GRID_WIDTH);

                            // update all the buttons and their positions:
                            pause.setCharacterSize(5 * button_size / 6);
                            pause.setOutlineThickness(button_size / 6);
                            pause.setPosition((cell_size.x + 1) * game_board->get_x_dim() + 29.0f, 4 * button_size + 125.0f);
                            break;
                        // while we want to move a current figure to the right:
                        case Keyboard::D:
                        case Keyboard::Right:
                            if(_figure_state == STOP_FIGURE){
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
                            if(_figure_state == STOP_FIGURE){
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
                            if (_figure_state != STOP_FIGURE)
                                game_board->step_down();
                            break;
                        case Keyboard::Space:
                            if(_figure_state != STOP_FIGURE){
                                game_board->hard_drop();
                                _figure_state = CHANGE_FIGURE;
                            }
                            break;
                        case Keyboard::G:
                        case Keyboard::Up:
                            if (_figure_state != STOP_FIGURE)
                                game_board->rotate(false);
                            break;
                        case Keyboard::H:
                            if (_figure_state != STOP_FIGURE)
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
                    cell_size.x = min(min(40.0f, (float) (0.73 * window.getSize().x - 29.0f) / BOARD_GRID_WIDTH), min(40.0f, ((float) window.getSize().y - 29.0f) / 20));
                    cell_size.y = min(min(40.0f, (float) (0.73 * window.getSize().x - 29.0f) / BOARD_GRID_WIDTH), min(40.0f, ((float) window.getSize().y - 29.0f) / 20));
                    game_board->set_cell_size(cell_size);

                    // update button size:
                    button_size = min(60.0, 3.75 * (0.27 * window.getSize().x - 5.0f) / BOARD_GRID_WIDTH);

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

        // draw a board:
        game_board->print_board(window, font, 5 * button_size / 6);
        // draw pause button:
        window.draw(pause);

        // if we have reached game over condition:
        if (game_board->game_over()) {
            game_board->get_descend_thread()->terminate();
            game_over_menu(game_board);
        }

        // display what we have just drawn:
        window.display();
    }

    delete game_board;
}

void Menu::game_over_menu(Board* game_board) {
    // counter of the currently chosen button:
    unsigned focused_button_counter = 0;
    // number of buttons:
    const unsigned number_of_buttons = 4;
    // button size:
    double button_size = min(min(60.0, 3.5 * (window.getSize().x - 10.0) / 16), 
        min((window.getSize().y - 50.0 - 15.0 * (number_of_buttons - 1)) / number_of_buttons / 3, 3.75 * (0.27 * window.getSize().x - 5.0f) / BOARD_GRID_WIDTH));

    // initalize the game over background:
    RectangleShape game_over_background;
    game_over_background.setSize(Vector2f(window.getSize().x, window.getSize().y));
    game_over_background.setFillColor(Color(255, 255, 255, 100));

    // set the game over image:
    Texture texture;
    try {
        if (!texture.loadFromFile("images/game_over.png")) 
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
                    restart.setFillColor(COLOR_DARK_VIOLET);
                    restart.setOutlineColor(COLOR_LIGHT_GREEN);
                    choose_complexity.setFillColor(COLOR_DARK_VIOLET);
                    choose_complexity.setOutlineColor(COLOR_LIGHT_GREEN);
                    to_main_menu.setFillColor(COLOR_DARK_VIOLET);
                    to_main_menu.setOutlineColor(COLOR_LIGHT_GREEN);
                    exit.setFillColor(COLOR_DARK_VIOLET);
                    exit.setOutlineColor(COLOR_LIGHT_GREEN);

                    // If appropriate mouse position was captured:
                    if (captured_button(window, restart)) {
                        // focus restart button:
                        restart.setFillColor(COLOR_YELLOW);
                        restart.setOutlineColor(COLOR_DARK_BLUE);
                        focused_button_counter = 1;
                    } else if (captured_button(window, choose_complexity)) {
                        // focus new choose_complexity game button:
                        choose_complexity.setFillColor(COLOR_YELLOW);
                        choose_complexity.setOutlineColor(COLOR_DARK_BLUE);
                        focused_button_counter = 2;
                    } else if (captured_button(window, to_main_menu)) {
                        // focus to_main_menu button:
                        to_main_menu.setFillColor(COLOR_YELLOW);
                        to_main_menu.setOutlineColor(COLOR_DARK_BLUE);
                        focused_button_counter = 3;
                    } else if (captured_button(window, exit)) {
                        // focus exit button:
                        exit.setFillColor(COLOR_YELLOW);
                        exit.setOutlineColor(COLOR_DARK_BLUE);
                        focused_button_counter = 4;
                    }
                    break;
                case Event::MouseButtonPressed:
                    switch (event.key.code) {
                        case Mouse::Left:
                            // If appropriate mouse position was captured:
                            // 1) Restart game:
                            if (captured_button(window, restart))
                                game(game_board->get_complexity());
                            // 2) Go to cemplexity menu:
                            else if (captured_button(window, choose_complexity))
                                complexity_menu();
                            // 3) Go to main menu:
                            else if (captured_button(window, to_main_menu))
                                main_menu();
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

                                    // focus restart button:
                                    restart.setFillColor(COLOR_YELLOW);
                                    restart.setOutlineColor(COLOR_DARK_BLUE);
                                    focused_button_counter++;
                                    break;
                                case 1:
                                    // if we have pressed Enter:
                                    if (event.key.code == Keyboard::Return)
                                        // execute restart button:
                                        game(game_board->get_complexity());

                                    // unfocus restart button:
                                    restart.setFillColor(COLOR_DARK_VIOLET);
                                    restart.setOutlineColor(COLOR_LIGHT_GREEN);
                                    // focus choose_complexity button:
                                    choose_complexity.setFillColor(COLOR_YELLOW);
                                    choose_complexity.setOutlineColor(COLOR_DARK_BLUE);
                                    focused_button_counter++;
                                    break;
                                case 2:
                                    // if we have pressed Enter:
                                    if (event.key.code == Keyboard::Return)
                                        // execute choose_complexity button:
                                        complexity_menu();

                                    // unfocus choose_complexity button:
                                    choose_complexity.setFillColor(COLOR_DARK_VIOLET);
                                    choose_complexity.setOutlineColor(COLOR_LIGHT_GREEN);
                                    // focus to_main_menu button:
                                    to_main_menu.setFillColor(COLOR_YELLOW);
                                    to_main_menu.setOutlineColor(COLOR_DARK_BLUE);
                                    focused_button_counter++;
                                    break;
                                case 3:
                                    // if we have pressed Enter:
                                    if (event.key.code == Keyboard::Return)
                                        // execute to_main_menu button:
                                        main_menu();

                                    // unfocus to_main_menu button:
                                    to_main_menu.setFillColor(COLOR_DARK_VIOLET);
                                    to_main_menu.setOutlineColor(COLOR_LIGHT_GREEN);
                                    // focus exit button:
                                    exit.setFillColor(COLOR_YELLOW);
                                    exit.setOutlineColor(COLOR_DARK_BLUE);
                                    focused_button_counter++;
                                    break;
                                case 4:
                                    // if we have pressed Enter:
                                    if (event.key.code == Keyboard::Return)
                                        // execute exit button:
                                        window.close();

                                    // unfocus exit button:
                                    exit.setFillColor(COLOR_DARK_VIOLET);
                                    exit.setOutlineColor(COLOR_LIGHT_GREEN);
                                    // focus restart button:
                                    restart.setFillColor(COLOR_YELLOW);
                                    restart.setOutlineColor(COLOR_DARK_BLUE);
                                    focused_button_counter = 1;
                                    break;
                                default:
                                    break;
                            }
                        default:
                            break;
                    }
                    break;
                // if we have changed window's size:
                case Event::Resized:
                    // set minimal window size:
                    if ((window.getSize().y < 600) || (window.getSize().x < 400))
                        window.setSize(Vector2u(400, 600));
            
                    // update button size:
                    button_size = min(min(60.0, 3.5 * (window.getSize().x - 10.0) / 16), 
                        min((window.getSize().y - 50.0 - 15.0 * (number_of_buttons - 1)) / number_of_buttons / 3, 3.75 * (0.27 * window.getSize().x - 5.0f) / BOARD_GRID_WIDTH));

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

void Menu::pause_menu(Board* game_board) {
    // counter of the currently chosen button:
    unsigned focused_button_counter = 0;
    // number of buttons:
    const unsigned number_of_buttons = 4;
    // button size:
    double button_size = min(min(60.0, 3.5 * (window.getSize().x - 10.0) / 16), 
        min((window.getSize().y - 50.0 - 15.0 * (number_of_buttons - 1)) / number_of_buttons / 2, 3.75 * (0.27 * window.getSize().x - 5.0f) / BOARD_GRID_WIDTH));

    // initialize cell size according the current window size:
    Vector2f cell_size(min(min(40.0f, (float) (0.73 * window.getSize().x - 29.0f) / BOARD_GRID_WIDTH), min(40.0f, ((float) window.getSize().y - 29.0f) / 20)),
                    min(min(40.0f, (float) (0.73 * window.getSize().x - 29.0f) / BOARD_GRID_WIDTH), min(40.0f, ((float) window.getSize().y - 29.0f) / 20)));


    // initalize pause background:
    RectangleShape pause_background;
    pause_background.setSize(Vector2f(window.getSize().x, window.getSize().y));
    pause_background.setFillColor(Color(255, 255, 255, 100));

    // set the pause image:
    Texture texture;
    try {
        if (!texture.loadFromFile("images/pause.png")) 
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
                    resume.setFillColor(COLOR_DARK_VIOLET);
                    resume.setOutlineColor(COLOR_LIGHT_GREEN);
                    restart.setFillColor(COLOR_DARK_VIOLET);
                    restart.setOutlineColor(COLOR_LIGHT_GREEN);
                    to_main_menu.setFillColor(COLOR_DARK_VIOLET);
                    to_main_menu.setOutlineColor(COLOR_LIGHT_GREEN);
                    exit.setFillColor(COLOR_DARK_VIOLET);
                    exit.setOutlineColor(COLOR_LIGHT_GREEN);

                    // If appropriate mouse position was captured:
                    if (captured_button(window, resume)) {
                        // focus resume button:
                        resume.setFillColor(COLOR_YELLOW);
                        resume.setOutlineColor(COLOR_DARK_BLUE);
                        focused_button_counter = 1;
                    } else if (captured_button(window, restart)) {
                        // focus new restart game button:
                        restart.setFillColor(COLOR_YELLOW);
                        restart.setOutlineColor(COLOR_DARK_BLUE);
                        focused_button_counter = 2;
                    } else if (captured_button(window, to_main_menu)) {
                        // focus to_main_menu button:
                        to_main_menu.setFillColor(COLOR_YELLOW);
                        to_main_menu.setOutlineColor(COLOR_DARK_BLUE);
                        focused_button_counter = 3;
                    } else if (captured_button(window, exit)) {
                        // focus exit button:
                        exit.setFillColor(COLOR_YELLOW);
                        exit.setOutlineColor(COLOR_DARK_BLUE);
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
                            else if (captured_button(window, restart)) {
                                game_board->get_descend_thread()->terminate();
                                game(game_board->get_complexity());
                            // 3) Go to main menu:
                            } else if (captured_button(window, to_main_menu)) {
                                game_board->get_descend_thread()->terminate();
                                main_menu();
                            // 4) Exit:
                            } else if (captured_button(window, exit)) {
                                game_board->get_descend_thread()->terminate();
                                window.close();
                            }
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
                                    resume.setFillColor(COLOR_YELLOW);
                                    resume.setOutlineColor(COLOR_DARK_BLUE);
                                    focused_button_counter++;
                                    break;
                                case 1:
                                    // if we have pressed Enter:
                                    if (event.key.code == Keyboard::Return)
                                        // execute resume button:
                                        return;

                                    // unfocus resume button:
                                    resume.setFillColor(COLOR_DARK_VIOLET);
                                    resume.setOutlineColor(COLOR_LIGHT_GREEN);
                                    // focus restart button:
                                    restart.setFillColor(COLOR_YELLOW);
                                    restart.setOutlineColor(COLOR_DARK_BLUE);
                                    focused_button_counter++;
                                    break;
                                case 2:
                                    // if we have pressed Enter:
                                    if (event.key.code == Keyboard::Return) {
                                        game_board->get_descend_thread()->terminate();
                                        // execute restart button:
                                        game(game_board->get_complexity());
                                    }

                                    // unfocus restart button:
                                    restart.setFillColor(COLOR_DARK_VIOLET);
                                    restart.setOutlineColor(COLOR_LIGHT_GREEN);
                                    // focus to_main_menu button:
                                    to_main_menu.setFillColor(COLOR_YELLOW);
                                    to_main_menu.setOutlineColor(COLOR_DARK_BLUE);
                                    focused_button_counter++;
                                    break;
                                case 3:
                                    // if we have pressed Enter:
                                    if (event.key.code == Keyboard::Return) {
                                        game_board->get_descend_thread()->terminate();
                                        // execute to_main_menu button:
                                        main_menu();
                                    }

                                    // unfocus to_main_menu button:
                                    to_main_menu.setFillColor(COLOR_DARK_VIOLET);
                                    to_main_menu.setOutlineColor(COLOR_LIGHT_GREEN);
                                    // focus exit button:
                                    exit.setFillColor(COLOR_YELLOW);
                                    exit.setOutlineColor(COLOR_DARK_BLUE);
                                    focused_button_counter++;
                                    break;
                                case 4:
                                    // if we have pressed Enter:
                                    if (event.key.code == Keyboard::Return) {
                                        game_board->get_descend_thread()->terminate();
                                        // execute exit button:
                                        window.close();
                                    }

                                    // unfocus exit button:
                                    exit.setFillColor(COLOR_DARK_VIOLET);
                                    exit.setOutlineColor(COLOR_LIGHT_GREEN);
                                    // focus resume button:
                                    resume.setFillColor(COLOR_YELLOW);
                                    resume.setOutlineColor(COLOR_DARK_BLUE);
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
                    break;
                // if we have changed window's size:
                case Event::Resized:
                    // set minimal window size:
                    if ((window.getSize().y < 600) || (window.getSize().x < 400))
                        window.setSize(Vector2u(400, 600));
            
                    // update button size:
                    button_size = min(min(60.0, 3.5 * (window.getSize().x - 10.0) / 16), 
                        min((window.getSize().y - 50.0 - 15.0 * (number_of_buttons - 1)) / number_of_buttons / 2, 3.75 * (0.27 * window.getSize().x - 5.0f) / BOARD_GRID_WIDTH));

                    // update view:
                    window.setView(View(FloatRect(0.0f, 0.0f, (float) window.getSize().x, (float) window.getSize().y)));

                    // update cell size:
                    cell_size.x = min(min(40.0f, (float) (0.73 * window.getSize().x - 29.0f) / BOARD_GRID_WIDTH), min(40.0f, ((float) window.getSize().y - 29.0f) / 20));
                    cell_size.y = min(min(40.0f, (float) (0.73 * window.getSize().x - 29.0f) / BOARD_GRID_WIDTH), min(40.0f, ((float) window.getSize().y - 29.0f) / 20));
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

// Multiplayer game function:
void Menu::multiplayer_game(Server* current_session, Client* current_client) {
    // counter of the currently chosen button:
    unsigned focused_button_counter = 0;
    // own button size:
    double button_size = min(60.0, 3.75 * (0.27 * window.getSize().x - 5.0f) / BOARD_GRID_WIDTH);

    // initialize own cell size according the current window size:
    Vector2f own_cell_size(min(min(40.0f, (float) (0.73 * window.getSize().x - 29.0f) / BOARD_GRID_WIDTH), min(40.0f, ((float) window.getSize().y - 29.0f) / 20)),
                    min(min(40.0f, (float) (0.73 * window.getSize().x - 29.0f) / BOARD_GRID_WIDTH), min(40.0f, ((float) window.getSize().y - 29.0f) / 20)));

    // initialize other player's cell size:
    Vector2f other_cell_size(own_cell_size.x / 2, own_cell_size.y / 2);

    request_status status = NOT_CHANGED;
    // Initialize client-server communication:
    Thread* listen_thread = nullptr;    
    if (current_client == nullptr) {
        // Look for the clients:
        listen_thread = new Thread([&] () { current_session->listen_game(status); });

        listen_thread->launch();
    } else if (current_session == nullptr) {
        // Look for the clients:
        listen_thread = new Thread([&] () { current_client->listen_game(status); });

        listen_thread->launch();
    }

    // Create a list of clients and initialize server name and complexity:
    vector<client_data> *player_list = nullptr;
    string server_name = "";
    unsigned complexity = 0;
    if (current_client == nullptr) {
        player_list = new vector<client_data>(current_session->get_clients());
        server_name = current_session->get_player_nickname();
        complexity = current_session->get_level();
    } else if (current_session == nullptr) {
        player_list = new vector<client_data>(current_client->get_server_data().clients);
        server_name = current_client->get_server_data().name;
        complexity = current_client->get_server_data().level;
    }

    // Initialize the number of players in the current session:
    unsigned number_of_players = player_list->size();

    window.setSize(Vector2u ((own_cell_size.x + 1) * (BOARD_GRID_WIDTH + FIGURE_GRID_WIDTH) + (number_of_players - 1) * (BOARD_GRID_WIDTH) * (other_cell_size.x + 1) + (number_of_players + 2) * 5 - number_of_players,
        (own_cell_size.y + 1) * (BOARD_GRID_HEIGHT - FIGURE_GRID_HEIGHT) + 9));

    // Initialize the global game over identifier:
    bool global_game_over = false;

    // Initialize game over counter:
    unsigned game_over_counter = 0;

    // initalize the game over background:
    RectangleShape game_over_background;
    game_over_background.setSize(Vector2f(window.getSize().x, window.getSize().y));
    game_over_background.setFillColor(Color(255, 255, 255, 100));

    // Initialize the index of current player:
    unsigned current_player_index = 0;
    for (unsigned i = 0; i < player_list->size(); i++)
        if (player_list->at(i).address == IpAddress::getLocalAddress()) {
            current_player_index = i;
            break;   
        }

    // create the game board: 
    Board* game_board = new Board(window, complexity, own_cell_size);

    // create game boards of the other players:
    vector<Board*> other_game_boards;
    for (unsigned i = 0; i < number_of_players - 1; i++)
        other_game_boards.emplace_back(new Board(window, complexity, other_cell_size));

    // Initialize pause button:
    Text pause = create_button(font, "Pause", button_size,
        Vector2f((own_cell_size.x + 1) * game_board->get_x_dim() + 29.0f, 4 * button_size + 125.0f), true, 0);

    figure_state _figure_state = DESCEND_FIGURE;
    int count_change_figure = DEF_COU_CHA_FIG;

    unsigned descend_counter = 0;

    // Using a thread to fall down:
    game_board->set_descend_thread(new Thread([&] () {
        for ( ; window.isOpen(); descend_counter++) {
            // Update other boards:
            player_list->clear();
            if (current_client == nullptr) {
                current_session->send_clients_board_data(*game_board);
                player_list = new vector<client_data>(current_session->get_clients());
            } else if (current_session == nullptr) {
                current_client->send_board_data(*game_board);
                player_list = new vector<client_data>(current_client->get_server_data().clients);
            }

            for (unsigned i = 0; i < number_of_players - 1; i++) {
                if (i >= current_player_index) {
                    other_game_boards[i]->set_map(player_list->at(i + 1).map);
                    other_game_boards[i]->set_score(player_list->at(i + 1).score);
                } else {
                    other_game_boards[i]->set_map(player_list->at(i).map);
                    other_game_boards[i]->set_score(player_list->at(i).score);
                }
            }

            // If someone's pressed pause:
            for (unsigned i = 0; i < player_list->size(); i++) {
                if (player_list->at(i).status == CLIENT_GAME_PAUSE) {
                    game_board->get_descend_thread()->terminate();
                    // execute pause button:
                    if (current_client == nullptr) {
                        current_session->pause(true);
                        multiplayer_pause_menu(game_board, other_game_boards, current_session, nullptr);
                    } else if (current_session == nullptr) {
                        current_client->pause(true);
                        multiplayer_pause_menu(game_board, other_game_boards, nullptr, current_client);
                    }
                    game_board->get_descend_thread()->launch();
                }
            }
        
            if (descend_counter >= 30 / complexity || _figure_state == CHANGE_FIGURE) {
                if(_figure_state == STOP_FIGURE)
                    count_change_figure--;
                if(count_change_figure <= 0)
                    _figure_state = CHANGE_FIGURE;
                if(_figure_state == DESCEND_FIGURE)
                    _figure_state = game_board->step_down() ? DESCEND_FIGURE : STOP_FIGURE; 
                // if we can't move down no more:
                if (_figure_state == CHANGE_FIGURE) {

                    count_change_figure = DEF_COU_CHA_FIG;
                    _figure_state = DESCEND_FIGURE;
                    // check for the full lines:
                    game_board->fix_current_figure();
                    game_board->erase_lines(complexity);

                    // putting next figure into a current figure:
                    game_board->set_current_figure(game_board->get_next_figure());
                    // adding new current figure on the board:
                    game_board->add_figure();
                    // creating the next figure:
                    game_board->set_next_figure(game_board->create_figure());
                }

                descend_counter = 0;
            }

            sf::sleep(seconds(0.015f));
        }
    }));

    game_board->get_descend_thread()->launch();

    // We are using descend counter to manage the figures' fall rate:
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
                    pause.setFillColor(COLOR_DARK_VIOLET);
                    pause.setOutlineColor(COLOR_LIGHT_GREEN);

                    // If appropriate mouse position was captured:
                    if (captured_button(window, pause)) {
                        // focus pause button:
                        pause.setFillColor(COLOR_YELLOW);
                        pause.setOutlineColor(COLOR_DARK_BLUE);
                        focused_button_counter = 1;
                    }
                    break;
                case Event::MouseButtonPressed:
                    switch (event.key.code) {
                        case Mouse::Left:
                            // If appropriate mouse position was captured:
                            // 1) pause game:
                            if (captured_button(window, pause)) {
                                game_board->get_descend_thread()->terminate();
                                // execute pause button:
                                if (current_client == nullptr) {
                                    current_session->pause(true);
                                    multiplayer_pause_menu(game_board, other_game_boards, current_session, nullptr);
                                } else if (current_session == nullptr) {
                                    current_client->pause(true);
                                    multiplayer_pause_menu(game_board, other_game_boards, nullptr, current_client);
                                }
                                game_board->get_descend_thread()->launch();
                                        
                                // update cell size:
                                own_cell_size.x = min(min(40.0f, (float) (0.73 * window.getSize().x - 29.0f) / BOARD_GRID_WIDTH), min(40.0f, ((float) window.getSize().y - 29.0f) / 20));
                                own_cell_size.y = min(min(40.0f, (float) (0.73 * window.getSize().x - 29.0f) / BOARD_GRID_WIDTH), min(40.0f, ((float) window.getSize().y - 29.0f) / 20));
                                game_board->set_cell_size(own_cell_size);

                                // update button size:
                                button_size = min(60.0, 3.75 * (0.27 * window.getSize().x - 5.0f) / BOARD_GRID_WIDTH);

                                // update all the buttons and their positions:
                                pause.setCharacterSize(5 * button_size / 6);
                                pause.setOutlineThickness(button_size / 6);
                                pause.setPosition((own_cell_size.x + 1) * game_board->get_x_dim() + 29.0f, 4 * button_size + 125.0f);
                        
                                // unfocus pause button:
                                pause.setFillColor(COLOR_DARK_VIOLET);
                                pause.setOutlineColor(COLOR_LIGHT_GREEN);
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

                                    // focus pause button:
                                    pause.setFillColor(COLOR_YELLOW);
                                    pause.setOutlineColor(COLOR_DARK_BLUE);
                                    focused_button_counter++;
                                    break;
                                case 1:
                                    // if we have pressed Enter:
                                    if (event.key.code == Keyboard::Return) {
                                        game_board->get_descend_thread()->terminate();
                                        // execute pause button:
                                        if (current_client == nullptr) {
                                            current_session->pause(true);
                                            multiplayer_pause_menu(game_board, other_game_boards, current_session, nullptr);
                                        } else if (current_session == nullptr) {
                                            current_client->pause(true);
                                            multiplayer_pause_menu(game_board, other_game_boards, nullptr, current_client);
                                        }
                                        game_board->get_descend_thread()->launch();

                                        // update cell size:
                                        own_cell_size.x = min(min(40.0f, (float) (0.73 * window.getSize().x - 29.0f) / BOARD_GRID_WIDTH), min(40.0f, ((float) window.getSize().y - 29.0f) / 20));
                                        own_cell_size.y = min(min(40.0f, (float) (0.73 * window.getSize().x - 29.0f) / BOARD_GRID_WIDTH), min(40.0f, ((float) window.getSize().y - 29.0f) / 20));
                                        game_board->set_cell_size(own_cell_size);

                                        // update button size:
                                        button_size = min(60.0, 3.75 * (0.27 * window.getSize().x - 5.0f) / BOARD_GRID_WIDTH);
                                    
                                        // update all the buttons and their positions:
                                        pause.setCharacterSize(5 * button_size / 6);
                                        pause.setOutlineThickness(button_size / 6);
                                        pause.setPosition((own_cell_size.x + 1) * game_board->get_x_dim() + 29.0f, 4 * button_size + 125.0f);
                                    }

                                    // unfocus pause button:
                                    pause.setFillColor(COLOR_DARK_VIOLET);
                                    pause.setOutlineColor(COLOR_LIGHT_GREEN);
                                    focused_button_counter = 0;
                                    break;
                                default:
                                    break;
                            }
                            break;
                        // if Escape is pushed: 
                        case Keyboard::Escape:
                            game_board->get_descend_thread()->terminate();
                            // execute pause button:
                            if (current_client == nullptr) {
                                current_session->pause(true);
                                multiplayer_pause_menu(game_board, other_game_boards, current_session, nullptr);
                            } else if (current_session == nullptr) {
                                current_client->pause(true);
                                multiplayer_pause_menu(game_board, other_game_boards, nullptr, current_client);
                            }
                            game_board->get_descend_thread()->launch();

                            // update cell size:
                            own_cell_size.x = min(min(40.0f, (float) (0.73 * window.getSize().x - 29.0f) / BOARD_GRID_WIDTH), min(40.0f, ((float) window.getSize().y - 29.0f) / 20));
                            own_cell_size.y = min(min(40.0f, (float) (0.73 * window.getSize().x - 29.0f) / BOARD_GRID_WIDTH), min(40.0f, ((float) window.getSize().y - 29.0f) / 20));
                            game_board->set_cell_size(own_cell_size);

                            // update button size:
                            button_size = min(60.0, 3.75 * (0.27 * window.getSize().x - 5.0f) / BOARD_GRID_WIDTH);

                            // update all the buttons and their positions:
                            pause.setCharacterSize(5 * button_size / 6);
                            pause.setOutlineThickness(button_size / 6);
                            pause.setPosition((own_cell_size.x + 1) * game_board->get_x_dim() + 29.0f, 4 * button_size + 125.0f);
                            break;
                        // while we want to move a current figure to the right:
                        case Keyboard::D:
                        case Keyboard::Right:
                            if(_figure_state == STOP_FIGURE){
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
                            if(_figure_state == STOP_FIGURE){
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
                            if(_figure_state != STOP_FIGURE)
                                game_board->step_down();
                            break;
                        case Keyboard::Space:
                            if(_figure_state != STOP_FIGURE){
                                game_board->hard_drop();
                                _figure_state = CHANGE_FIGURE;
                            }
                            break;
                        case Keyboard::G:
                        case Keyboard::Up:
                            if(_figure_state != STOP_FIGURE)
                                game_board->rotate(false);
                            break;
                        case Keyboard::H:
                            if(_figure_state != STOP_FIGURE)
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

                    // update own cell size:
                    own_cell_size.x = min(min(40.0f, (float) (0.73 * window.getSize().x - 29.0f) / BOARD_GRID_WIDTH), min(40.0f, ((float) window.getSize().y - 29.0f) / 20));
                    own_cell_size.y = min(min(40.0f, (float) (0.73 * window.getSize().x - 29.0f) / BOARD_GRID_WIDTH), min(40.0f, ((float) window.getSize().y - 29.0f) / 20));
                    game_board->set_cell_size(own_cell_size);

                    // update other players' cell size:
                    other_cell_size.x = own_cell_size.x / 2;
                    other_cell_size.y = own_cell_size.y / 2;
                    for (unsigned i = 0; i < number_of_players - 1; i++)
                        other_game_boards[i]->set_cell_size(other_cell_size);

                    // update button size:
                    button_size = min(60.0, 3.75 * (0.27 * window.getSize().x - 5.0f) / BOARD_GRID_WIDTH);

                    // update all the buttons and their positions:
                    pause.setCharacterSize(5 * button_size / 6);
                    pause.setOutlineThickness(button_size / 6);
                    pause.setPosition((own_cell_size.x + 1) * game_board->get_x_dim() + 29.0f, 4 * button_size + 125.0f);
                    break;
                default:
                    break;
            }
        }

        // clear game window:
        window.clear();

        // draw background:
        window.draw(background);

        // draw a board:
        if (current_client == nullptr)
            game_board->print_board(window, font, 5 * button_size / 6, current_session->get_player_nickname());
        else
            game_board->print_board(window, font, 5 * button_size / 6, current_client->get_player_nickname());

        // draw pause button:
        window.draw(pause);
        
        // if we have reached game over condition:
        if (game_board->game_over()) {
            game_board->get_descend_thread()->terminate();
            listen_thread->terminate();
            game_over_menu(game_board);
        }

        for (unsigned i = 0; i < number_of_players - 1; i++) {
            if (current_client == nullptr)
                other_game_boards[i]->print_board(window, font, 5 * button_size / 6, current_session->get_clients()[i + 1].nickname, i + 1);
            else {
                if (current_player_index >= i)  
                    other_game_boards[i]->print_board(window, font, 5 * button_size / 6, current_client->get_server_data().clients[i + 1].nickname, i + 1);
                else
                    other_game_boards[i]->print_board(window, font, 5 * button_size / 6, current_client->get_server_data().clients[i].nickname, i + 1);
            }
        }

        // display what we have just drawn:
        window.display();
    }

    delete game_board;
    other_game_boards.clear();
};

// Multiplayer pause menu:
void Menu::multiplayer_pause_menu(Board* game_board, vector<Board*> other_boards, Server* current_session, Client* current_client) {
    // counter of the currently chosen button:
    unsigned focused_button_counter = 0;
    // number of buttons:
    const unsigned number_of_buttons = 3;
    // button size:
    double button_size = min(min(60.0, 3.5 * (window.getSize().x - 10.0) / 16), 
        min((window.getSize().y - 50.0 - 15.0 * (number_of_buttons - 1)) / number_of_buttons / 2, 3.75 * (0.27 * window.getSize().x - 5.0f) / BOARD_GRID_WIDTH));

    // initialize cell size according the current window size:
    Vector2f cell_size(min(min(40.0f, (float) (0.73 * window.getSize().x - 29.0f) / BOARD_GRID_WIDTH), min(40.0f, ((float) window.getSize().y - 29.0f) / 20)),
                    min(min(40.0f, (float) (0.73 * window.getSize().x - 29.0f) / BOARD_GRID_WIDTH), min(40.0f, ((float) window.getSize().y - 29.0f) / 20)));


    // initalize pause background:
    RectangleShape pause_background;
    pause_background.setSize(Vector2f(window.getSize().x, window.getSize().y));
    pause_background.setFillColor(Color(255, 255, 255, 100));

    // set the pause image:
    Texture texture;
    try {
        if (!texture.loadFromFile("images/pause.png")) 
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

    // Initialize disconnect button:
    Text disconnect = create_button(font, "Disconnect", button_size,
        Vector2f(window.getSize().x / 2,
            0.85 * (window.getSize().y - number_of_buttons * (button_size + 15) - 15) + (button_size + 15) * 1));

    // Initialize exit button:
    Text exit = create_button(font, "Exit", button_size,
        Vector2f(window.getSize().x / 2,
            0.85 * (window.getSize().y - number_of_buttons * (button_size + 15) - 15) + (button_size + 15) * 2));

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
                    resume.setFillColor(COLOR_DARK_VIOLET);
                    resume.setOutlineColor(COLOR_LIGHT_GREEN);
                    disconnect.setFillColor(COLOR_DARK_VIOLET);
                    disconnect.setOutlineColor(COLOR_LIGHT_GREEN);
                    exit.setFillColor(COLOR_DARK_VIOLET);
                    exit.setOutlineColor(COLOR_LIGHT_GREEN);

                    // If appropriate mouse position was captured:
                    if (captured_button(window, resume)) {
                        // focus resume button:
                        resume.setFillColor(COLOR_YELLOW);
                        resume.setOutlineColor(COLOR_DARK_BLUE);
                        focused_button_counter = 1;
                    } else if (captured_button(window, disconnect)) {
                        // focus disconnect button:
                        disconnect.setFillColor(COLOR_YELLOW);
                        disconnect.setOutlineColor(COLOR_DARK_BLUE);
                        focused_button_counter = 2;
                    } else if (captured_button(window, exit)) {
                        // focus exit button:
                        exit.setFillColor(COLOR_YELLOW);
                        exit.setOutlineColor(COLOR_DARK_BLUE);
                        focused_button_counter = 3;
                    }
                    break;
                case Event::MouseButtonPressed:
                    switch (event.key.code) {
                        case Mouse::Left:
                            // If appropriate mouse position was captured:
                            // 1) Resume game:
                            if (captured_button(window, resume)) {
                                return;
                            // 3) Go to main menu:
                            } else if (captured_button(window, disconnect)) {
                                game_board->get_descend_thread()->terminate();
                                main_menu();
                            // 4) Exit:
                            } else if (captured_button(window, exit)) {
                                game_board->get_descend_thread()->terminate();
                                window.close();
                            }
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
                                    resume.setFillColor(COLOR_YELLOW);
                                    resume.setOutlineColor(COLOR_DARK_BLUE);
                                    focused_button_counter++;
                                    break;
                                case 1:
                                    // if we have pressed Enter:
                                    if (event.key.code == Keyboard::Return)
                                        // execute resume button:
                                        return;

                                    // unfocus resume button:
                                    resume.setFillColor(COLOR_DARK_VIOLET);
                                    resume.setOutlineColor(COLOR_LIGHT_GREEN);
                                    // focus disconnect button:
                                    disconnect.setFillColor(COLOR_YELLOW);
                                    disconnect.setOutlineColor(COLOR_DARK_BLUE);
                                    focused_button_counter++;
                                    break;
                                case 2:
                                    // if we have pressed Enter:
                                    if (event.key.code == Keyboard::Return) {
                                        game_board->get_descend_thread()->terminate();
                                        // execute disconnect button:
                                        main_menu();
                                    }

                                    // unfocus disconnect button:
                                    disconnect.setFillColor(COLOR_DARK_VIOLET);
                                    disconnect.setOutlineColor(COLOR_LIGHT_GREEN);
                                    // focus exit button:
                                    exit.setFillColor(COLOR_YELLOW);
                                    exit.setOutlineColor(COLOR_DARK_BLUE);
                                    focused_button_counter++;
                                    break;
                                case 3:
                                    // if we have pressed Enter:
                                    if (event.key.code == Keyboard::Return) {
                                        game_board->get_descend_thread()->terminate();
                                        // execute exit button:
                                        window.close();
                                    }

                                    // unfocus exit button:
                                    exit.setFillColor(COLOR_DARK_VIOLET);
                                    exit.setOutlineColor(COLOR_LIGHT_GREEN);
                                    // focus resume button:
                                    resume.setFillColor(COLOR_YELLOW);
                                    resume.setOutlineColor(COLOR_DARK_BLUE);
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
                    break;
                // if we have changed window's size:
                case Event::Resized:
                    // set minimal window size:
                    if ((window.getSize().y < 600) || (window.getSize().x < 400))
                        window.setSize(Vector2u(400, 600));
            
                    // update button size:
                    button_size = min(min(60.0, 3.5 * (window.getSize().x - 10.0) / 16), 
                        min((window.getSize().y - 50.0 - 15.0 * (number_of_buttons - 1)) / number_of_buttons / 2, 3.75 * (0.27 * window.getSize().x - 5.0f) / BOARD_GRID_WIDTH));

                    // update view:
                    window.setView(View(FloatRect(0.0f, 0.0f, (float) window.getSize().x, (float) window.getSize().y)));

                    // update cell size:
                    cell_size.x = min(min(40.0f, (float) (0.73 * window.getSize().x - 29.0f) / BOARD_GRID_WIDTH), min(40.0f, ((float) window.getSize().y - 29.0f) / 20));
                    cell_size.y = min(min(40.0f, (float) (0.73 * window.getSize().x - 29.0f) / BOARD_GRID_WIDTH), min(40.0f, ((float) window.getSize().y - 29.0f) / 20));
                    game_board->set_cell_size(cell_size);

                    // update other players' cell size:
                    for (unsigned i = 0; i < other_boards.size(); i++)
                        other_boards[i]->set_cell_size(Vector2f (cell_size.x / 2, cell_size.y / 2));

                    // update pause background:
                    pause_background.setSize(Vector2f(window.getSize().x, window.getSize().y));

                    // update pause sprite: 
                    pause.setScale((float) window.getSize().x / texture.getSize().x, (float) window.getSize().y / texture.getSize().y / 1.5);

                    // update all the buttons and their positions:
                    resume.setCharacterSize(5 * button_size / 6);
                    resume.setOutlineThickness(button_size / 6);
                    resume.setPosition((window.getSize().x - resume.getGlobalBounds().width) / 2, 
                            0.85 * (window.getSize().y - number_of_buttons * (button_size + 15) - 15));
                  
                    disconnect.setCharacterSize(5 * button_size / 6);
                    disconnect.setOutlineThickness(button_size / 6);
                    disconnect.setPosition((window.getSize().x - disconnect.getGlobalBounds().width) / 2, 
                            0.85 * (window.getSize().y - number_of_buttons * (button_size + 15) - 15) + (button_size + 15) * 1);
                    
                    exit.setCharacterSize(5 * button_size / 6);
                    exit.setOutlineThickness(button_size / 6);
                    exit.setPosition((window.getSize().x - exit.getGlobalBounds().width) / 2, 
                            0.85 * (window.getSize().y - number_of_buttons * (button_size + 15) - 15) + (button_size + 15) * 2);
                    break;
                default:
                    break;
            }
        }

        // clear game window:
        window.clear();

        // draw background:
        window.draw(background);

        // draw a board:
        game_board->print_board(window, font, 5 * button_size / 6);

        for (unsigned i = 0; i < other_boards.size(); i++)
            other_boards[i]->print_board(window, font, 5 * button_size / 6);

        // draw pause background:
        window.draw(pause_background);
        //draw pause sprite:
        window.draw(pause);

        // draw resume button:
        window.draw(resume);
        // draw main menu button:
        window.draw(disconnect);
        // draw exit button:
        window.draw(exit);

        // display what we have just drawn:
        window.display();
    }
};

void Menu::main_menu(const bool& initialization) {
    if (initialization)
        window.create(VideoMode (VideoMode::getDesktopMode().width / 2, VideoMode::getDesktopMode().height),
            "TetrisSS 1.0");

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
                    singleplayer.setFillColor(COLOR_DARK_VIOLET);
                    singleplayer.setOutlineColor(COLOR_LIGHT_GREEN);
                    multiplayer.setFillColor(COLOR_DARK_VIOLET);
                    multiplayer.setOutlineColor(COLOR_LIGHT_GREEN);
                    leaderboard.setFillColor(COLOR_DARK_VIOLET);
                    leaderboard.setOutlineColor(COLOR_LIGHT_GREEN);
                    settings.setFillColor(COLOR_DARK_VIOLET);
                    settings.setOutlineColor(COLOR_LIGHT_GREEN);
                    authors.setFillColor(COLOR_DARK_VIOLET);
                    authors.setOutlineColor(COLOR_LIGHT_GREEN);
                    exit.setFillColor(COLOR_DARK_VIOLET);
                    exit.setOutlineColor(COLOR_LIGHT_GREEN);

                    // If appropriate mouse position was captured:
                    if (captured_button(window, singleplayer)) {
                        // focus new single game button:
                        singleplayer.setFillColor(COLOR_YELLOW);
                        singleplayer.setOutlineColor(COLOR_DARK_BLUE);
                        focused_button_counter = 1;
                    } else if (captured_button(window, multiplayer)) {
                        // focus choose complexity button:
                        multiplayer.setFillColor(COLOR_YELLOW);
                        multiplayer.setOutlineColor(COLOR_DARK_BLUE);
                        focused_button_counter = 2;
                    } else if (captured_button(window, leaderboard)) {
                        // focus leaderboard button:
                        leaderboard.setFillColor(COLOR_YELLOW);
                        leaderboard.setOutlineColor(COLOR_DARK_BLUE);
                        focused_button_counter = 3;
                    } else if (captured_button(window, settings)) {
                        // focus settings button:
                        settings.setFillColor(COLOR_YELLOW);
                        settings.setOutlineColor(COLOR_DARK_BLUE);
                        focused_button_counter = 4;
                    } else if (captured_button(window, authors)) {
                        // focus authors button:
                        authors.setFillColor(COLOR_YELLOW);
                        authors.setOutlineColor(COLOR_DARK_BLUE);
                        focused_button_counter = 5;
                    } else if (captured_button(window, exit)) {
                        // focus exit button:
                        exit.setFillColor(COLOR_YELLOW);
                        exit.setOutlineColor(COLOR_DARK_BLUE);
                        focused_button_counter = 6;
                    }
                    break;
                case Event::MouseButtonPressed:
                    switch (event.key.code) {
                        case Mouse::Left:
                            // If appropriate mouse position was captured:
                            // 1) Start new single game:
                            if (captured_button(window, singleplayer))
                                complexity_menu();
                            // 2) Go to multiplayer menu:
                            else if (captured_button(window, multiplayer))
                                multiplayer_menu();
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

                                    // focus new single game button:
                                    singleplayer.setFillColor(COLOR_YELLOW);
                                    singleplayer.setOutlineColor(COLOR_DARK_BLUE);
                                    focused_button_counter++;
                                    break;
                                case 1:
                                    // if we have pressed Enter:
                                    if (event.key.code == Keyboard::Return)
                                        // execute singleplayer button:
                                        complexity_menu();

                                    // unfocus singleplayer button:
                                    singleplayer.setFillColor(COLOR_DARK_VIOLET);
                                    singleplayer.setOutlineColor(COLOR_LIGHT_GREEN);
                                    // focus multiplayer button:
                                    multiplayer.setFillColor(COLOR_YELLOW);
                                    multiplayer.setOutlineColor(COLOR_DARK_BLUE);
                                    focused_button_counter++;
                                    break;
                                case 2:
                                    // if we have pressed Enter:
                                    if (event.key.code == Keyboard::Return)
                                        // Go to multiplayer menu:
                                        multiplayer_menu();

                                    // unfocus multiplayer button:
                                    multiplayer.setFillColor(COLOR_DARK_VIOLET);
                                    multiplayer.setOutlineColor(COLOR_LIGHT_GREEN);
                                    // focus leaderboard button:
                                    leaderboard.setFillColor(COLOR_YELLOW);
                                    leaderboard.setOutlineColor(COLOR_DARK_BLUE);
                                    focused_button_counter++;
                                    break;
                                case 3:
                                    // if we have pressed Enter:
                                    if (event.key.code == Keyboard::Return)
                                        // execute leaderboard button:
                                        break;

                                    // unfocus leaderboard button:
                                    leaderboard.setFillColor(COLOR_DARK_VIOLET);
                                    leaderboard.setOutlineColor(COLOR_LIGHT_GREEN);
                                    // focus settings button:
                                    settings.setFillColor(COLOR_YELLOW);
                                    settings.setOutlineColor(COLOR_DARK_BLUE);
                                    focused_button_counter++;
                                    break;
                                case 4:
                                    // if we have pressed Enter:
                                    if (event.key.code == Keyboard::Return)
                                        // execute settings button:
                                        break;

                                    // unfocus settings button:
                                    settings.setFillColor(COLOR_DARK_VIOLET);
                                    settings.setOutlineColor(COLOR_LIGHT_GREEN);
                                    // focus authors button:
                                    authors.setFillColor(COLOR_YELLOW);
                                    authors.setOutlineColor(COLOR_DARK_BLUE);
                                    focused_button_counter++;
                                    break;
                                case 5:
                                    // if we have pressed Enter:
                                    if (event.key.code == Keyboard::Return)
                                        // execute authors button:
                                        break;

                                    // unfocus authors button:
                                    authors.setFillColor(COLOR_DARK_VIOLET);
                                    authors.setOutlineColor(COLOR_LIGHT_GREEN);
                                    // focus exit button:
                                    exit.setFillColor(COLOR_YELLOW);
                                    exit.setOutlineColor(COLOR_DARK_BLUE);
                                    focused_button_counter++;
                                    break;
                                case 6:
                                    // if we have pressed Enter:
                                    if (event.key.code == Keyboard::Return)
                                        // execute exit button:
                                        window.close();

                                    // unfocus exit button:
                                    exit.setFillColor(COLOR_DARK_VIOLET);
                                    exit.setOutlineColor(COLOR_LIGHT_GREEN);
                                    // focus singleplayer button:
                                    singleplayer.setFillColor(COLOR_YELLOW);
                                    singleplayer.setOutlineColor(COLOR_DARK_BLUE);
                                    focused_button_counter = 1;
                                    break;
                                default:
                                    break;
                            }
                        default:
                            break;
                    }
                    break;
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

void Menu::multiplayer_menu() { 
    // counter of the currently chosen button:
    unsigned focused_button_counter = 0;
    // number of buttons:
    const unsigned number_of_buttons = 4;
    // button size:
    double button_size = min(min(60.0f, 3.5f * (window.getSize().x - 10.0f) / 12), 
        (window.getSize().y - 50.0f - 15.0f * (number_of_buttons - 1)) / number_of_buttons);

    // Initialize multiplayer menu title:
    Text multiplayer_title = create_button(font, "Multiplayer", button_size,
        Vector2f(window.getSize().x / 2 - 10.0f,
            (window.getSize().y - number_of_buttons * (button_size + 15) - 25) / 2), false);

    // Initialize nickname_title button:
    Text nickname_title = create_button(font, "Enter Nickname:", button_size,
        Vector2f(window.getSize().x / 4,
            (window.getSize().y - number_of_buttons * (button_size + 15) - 25) / 2 + button_size + 20.0f), true, 4);
    
    // Initialize nickname display field:
    Text nickname_display = create_button(font, "", button_size,
        Vector2f(3 * window.getSize().x / 4,
            (window.getSize().y - number_of_buttons * (button_size + 15) - 25) / 2 + button_size + 20.0f), false, 4);

    // Initialize new_session button:
    Text new_session = create_button(font, "New Session", button_size,
        Vector2f(window.getSize().x / 2,
            (window.getSize().y - number_of_buttons * (button_size + 15) - 25) / 2 + (button_size + 15.0f) * 2 + 5.0f));
    
    // Initialize find_server button:
    Text find_server = create_button(font, "Find Server", button_size,
        Vector2f(window.getSize().x / 2,
            (window.getSize().y - number_of_buttons * (button_size + 15) - 25) / 2 + (button_size + 15.0f) * 3 + 5.0f));
      
    // Initialize back button:
    Text back = create_button(font, "Back", button_size,
        Vector2f(window.getSize().x / 2,
            (window.getSize().y - number_of_buttons * (button_size + 15) - 25) / 2 + (button_size + 15.0f) * 4 + 10.0f));
   
    // Initialize nickname itself and its capture indicator:
    string nickname = "";
    bool nickname_captured = false;

    // Using a thread to display blinking text bar during the active text field:
    Thread* blink_thread = new Thread([&] () {
        for (unsigned enter_text_counter = 0; true; enter_text_counter++) {
            // Display a text bar with some delay:
            if (enter_text_counter >= 500) {
                // Display entered session name:
                if (nickname_captured) {
                    if (nickname_display.getString().getSize() == nickname.length())
                        nickname_display.setString(nickname + "|");
                    else 
                        nickname_display.setString(nickname);
                }
                
                enter_text_counter = 0;

                sf::sleep(seconds(0.2f));
            }
        }
    });

    blink_thread->launch();

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
                    nickname_title.setFillColor(COLOR_DARK_VIOLET);
                    nickname_title.setOutlineColor(COLOR_LIGHT_GREEN);
                    new_session.setFillColor(COLOR_DARK_VIOLET);
                    new_session.setOutlineColor(COLOR_LIGHT_GREEN);
                    find_server.setFillColor(COLOR_DARK_VIOLET);
                    find_server.setOutlineColor(COLOR_LIGHT_GREEN);
                    back.setFillColor(COLOR_DARK_VIOLET);
                    back.setOutlineColor(COLOR_LIGHT_GREEN);

                    // If appropriate mouse position was captured:
                    if (captured_button(window, nickname_title)) {
                        // focus nickname_title button:
                        nickname_title.setFillColor(COLOR_YELLOW);
                        nickname_title.setOutlineColor(COLOR_DARK_BLUE);
                        focused_button_counter = 1;
                    } else if (captured_button(window, new_session)) {
                        // focus new_session button:
                        new_session.setFillColor(COLOR_YELLOW);
                        new_session.setOutlineColor(COLOR_DARK_BLUE);
                        focused_button_counter = 2;
                    } else if (captured_button(window, find_server)) {
                        // focus find_server button:
                        find_server.setFillColor(COLOR_YELLOW);
                        find_server.setOutlineColor(COLOR_DARK_BLUE);
                        focused_button_counter = 3;  
                    } else if (captured_button(window, back)) {
                        // focus back button:
                        back.setFillColor(COLOR_YELLOW);
                        back.setOutlineColor(COLOR_DARK_BLUE);
                        focused_button_counter = 4;
                    }
                    break;
                case Event::MouseButtonPressed:
                    switch (event.key.code) {
                        case Mouse::Left:
                            // If appropriate mouse position was captured:
                            // 1) Focus nickname textfield:
                            if (captured_button(window, nickname_title)) {
                                nickname_captured = true;
                                focused_button_counter = 1;
                            // 2) Go to new session menu:
                            } else if (captured_button(window, new_session) && nickname != "") {
                                blink_thread->terminate();
                                create_session(nickname);
                                blink_thread->launch();
                            // 3) Find a server:
                            } else if (captured_button(window, find_server) && nickname != "") {
                                blink_thread->terminate();
                                find_servers(nickname);
                                blink_thread->launch();
                            // 4) Go back:
                            } else if (captured_button(window, back)) {
                                blink_thread->terminate();
                                main_menu();
                            }
                            break;
                        default:
                            break;
                    }
                    break;
                // If we want to type a text:
                case Event::TextEntered:
                    if (event.text.unicode < 128 && event.text.unicode != 13 
                        && event.text.unicode != 8 && event.text.unicode != 9) {
                        if (nickname_captured && nickname.length() <= MAX_NICKNAME_LENGTH) {
                            // Add a new symbol at the end of a session name's string
                            // (maximal length of which - 12 characters):
                            nickname += static_cast<char>(event.text.unicode);
                            nickname_display.setString(nickname);

                            // Resize session name:
                            nickname_title.setCharacterSize(5 * button_size / 6);
                            nickname_title.setOutlineThickness(button_size / 6);
                            nickname_title.setPosition((window.getSize().x - nickname_title.getGlobalBounds().width) / 4, 
                                (window.getSize().y - number_of_buttons * (button_size + 15) - 25) / 2 + button_size + 20.0f);
                                
                            nickname_display.setCharacterSize(5 * button_size / 6);
                            nickname_display.setPosition(3 * (window.getSize().x - nickname_display.getGlobalBounds().width) / 4, 
                                (window.getSize().y - number_of_buttons * (button_size + 15) - 25) / 2 + button_size + 20.0f);
                        }
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
                                        
                                    nickname_captured = true;

                                    // focus nickname title button:
                                    nickname_title.setFillColor(COLOR_YELLOW);
                                    nickname_title.setOutlineColor(COLOR_DARK_BLUE);
                                    focused_button_counter++;
                                    break;
                                case 1:
                                    // if we have pressed Enter:
                                    if (event.key.code == Keyboard::Return)
                                        break;

                                    nickname_display.setString(nickname);
                                    nickname_captured = false; 

                                    // unfocus nickname_title button:
                                    nickname_title.setFillColor(COLOR_DARK_VIOLET);
                                    nickname_title.setOutlineColor(COLOR_LIGHT_GREEN);
                                    // focus new session button:
                                    new_session.setFillColor(COLOR_YELLOW);
                                    new_session.setOutlineColor(COLOR_DARK_BLUE);
                                    focused_button_counter++;
                                    break;
                                case 2:
                                    // if we have pressed Enter:
                                    if (event.key.code == Keyboard::Return && nickname != "") {
                                        // execute new_session button:
                                        blink_thread->terminate();
                                        create_session(nickname);
                                        blink_thread->launch();
                                        break;
                                    } else if (event.key.code != Keyboard::Return) {
                                        // unfocus new_session button:
                                        new_session.setFillColor(COLOR_DARK_VIOLET);
                                        new_session.setOutlineColor(COLOR_LIGHT_GREEN);
                                        // focus find_server button:
                                        find_server.setFillColor(COLOR_YELLOW);
                                        find_server.setOutlineColor(COLOR_DARK_BLUE);
                                        focused_button_counter++;
                                    }
                                    break;
                                case 3:
                                    // if we have pressed Enter:
                                    if (event.key.code == Keyboard::Return && nickname != "") {
                                        // execute find_server button:
                                        blink_thread->terminate();
                                        find_servers(nickname);
                                        blink_thread->launch();
                                        break;
                                    } else if (event.key.code != Keyboard::Return) {
                                        // unfocus find_server button:
                                        find_server.setFillColor(COLOR_DARK_VIOLET);
                                        find_server.setOutlineColor(COLOR_LIGHT_GREEN);
                                        // focus back button:
                                        back.setFillColor(COLOR_YELLOW);
                                        back.setOutlineColor(COLOR_DARK_BLUE);
                                        focused_button_counter++;
                                    }
                                    break;
                                case 4:
                                    // if we have pressed Enter:
                                    if (event.key.code == Keyboard::Return) {
                                        blink_thread->terminate();
                                        // execute back button:
                                        main_menu();
                                    }
                                    
                                    nickname_captured = true; 

                                    // unfocus back button:
                                    back.setFillColor(COLOR_DARK_VIOLET);
                                    back.setOutlineColor(COLOR_LIGHT_GREEN);
                                    // focus nickname_title button:
                                    nickname_title.setFillColor(COLOR_YELLOW);
                                    nickname_title.setOutlineColor(COLOR_DARK_BLUE);
                                    focused_button_counter = 1;
                                    break;
                                default:
                                    break;
                            }
                            break;
                        // If we have pushed Backspace:
                        case Keyboard::BackSpace:
                            if (nickname_captured && nickname.length() > 0) {
                                // Erase the last symbol from the session name string:
                                nickname.erase(nickname.length() - 1);
                                nickname_display.setString(nickname);

                                // Resize session name:
                                nickname_title.setCharacterSize(5 * button_size / 6);
                                nickname_title.setOutlineThickness(button_size / 6);
                                nickname_title.setPosition((window.getSize().x - nickname_title.getGlobalBounds().width) / 4, 
                                        (window.getSize().y - number_of_buttons * (button_size + 15) - 25) / 2 + button_size + 20.0f);
                                
                                nickname_display.setCharacterSize(5 * button_size / 6);
                                nickname_display.setPosition(3 * (window.getSize().x - nickname_display.getGlobalBounds().width) / 4, 
                                        (window.getSize().y - number_of_buttons * (button_size + 15) - 25) / 2 + button_size + 20.0f);
                            }
                            break;
                        default:
                            break;
                    }
                    break;
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

                    nickname_title.setCharacterSize(5 * button_size / 6);
                    nickname_title.setOutlineThickness(button_size / 6);
                    nickname_title.setPosition((window.getSize().x - nickname_title.getGlobalBounds().width) / 4, 
                            (window.getSize().y - number_of_buttons * (button_size + 15) - 25) / 2 + button_size + 20.0f);
                        
                    nickname_display.setCharacterSize(5 * button_size / 6);
                    nickname_display.setPosition(3 * (window.getSize().x - nickname_display.getGlobalBounds().width) / 4, 
                            (window.getSize().y - number_of_buttons * (button_size + 15) - 25) / 2 + button_size + 20.0f);

                    new_session.setCharacterSize(5 * button_size / 6);
                    new_session.setOutlineThickness(button_size / 6);
                    new_session.setPosition((window.getSize().x - new_session.getGlobalBounds().width) / 2, 
                            (window.getSize().y - number_of_buttons * (button_size + 15) - 25) / 2 + (button_size + 15.0f) * 2 + 5.0f);
                    
                    find_server.setCharacterSize(5 * button_size / 6);
                    find_server.setOutlineThickness(button_size / 6);
                    find_server.setPosition((window.getSize().x - find_server.getGlobalBounds().width) / 2, 
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

        // Draw a menu:
        window.draw(multiplayer_title);
        window.draw(nickname_title);
        window.draw(nickname_display);
        window.draw(new_session);
        window.draw(find_server);
        window.draw(back);
        window.display();
    }
};

void Menu::create_session(const string& nickname) {
    // counter of the currently chosen button:
    unsigned focused_button_counter = 0;
    // number of buttons:
    const unsigned number_of_buttons = 8;
    // button size:
    double button_size = min(min(60.0f, 3.5f * (window.getSize().x - 10.0f) / 12), 
        (window.getSize().y - 50.0f - 15.0f * (number_of_buttons - 1)) / number_of_buttons);
    
    // Initialize the number of maximum players: 
    unsigned max_number_of_players = 2;
    // Initialize the game complexity:
    unsigned complexity = 0;

    // Max players capture boolean identificator:
    bool max_number_of_players_captured = false;

    // Initialize create session menu title:
    Text create_session_title = create_button(font, "Session " + nickname + " Settings", button_size,
        Vector2f(window.getSize().x / 2 - 10.0f,
            (window.getSize().y - number_of_buttons * (button_size + 15) - 25) / 2), false);
    
    // Initialize max_number_of_players_title button:
    Text max_number_of_players_title = create_button(font, "Max Players:", button_size,
        Vector2f(window.getSize().x / 4,
            (window.getSize().y - number_of_buttons * (button_size + 15) - 25) / 2 + (button_size + 15.0f) * 2 + 5.0f), true, 4);
    
    // Initialize max players field:
    Text max_number_of_players_display = create_button(font, " ", button_size,
        Vector2f(3 * window.getSize().x / 4,
            (window.getSize().y - number_of_buttons * (button_size + 15) - 25) / 2 + (button_size + 15.0f) * 2 + 5.0f), false, 4);

    // Initialize complexity title:
    Text complexity_title = create_button(font, "Choose complexity", button_size,
        Vector2f(window.getSize().x / 2 - 15.0f,
            (window.getSize().y - number_of_buttons * (button_size + 15) - 25) / 2 + (button_size + 15.0f) * 3 + 5.0f), false);

    // Initialize mechanics button:
    Text mechanics = create_button(font, "Mechanics", button_size,
        Vector2f(window.getSize().x / 2,
            (window.getSize().y - number_of_buttons * (button_size + 15) - 25) / 2 + (button_size + 15.0f) * 4 + 5.0f));
    
    // Initialize STIC button:
    Text STIC = create_button(font, "STIC", button_size,
        Vector2f(window.getSize().x / 2,
            (window.getSize().y - number_of_buttons * (button_size + 15) - 25) / 2 + (button_size + 15.0f) * 5 + 5.0f));
    
    // Initialize applied Maths button:
    Text applied_maths = create_button(font, "Applied Maths", button_size,
        Vector2f(window.getSize().x / 2,
            (window.getSize().y - number_of_buttons * (button_size + 15) - 25) / 2 + (button_size + 15.0f) * 6 + 5.0f));
    
    // Initialize back button:
    Text back = create_button(font, "Back", button_size,
        Vector2f(window.getSize().x / 4,
            (window.getSize().y - number_of_buttons * (button_size + 15) - 25) / 2 + (button_size + 15.0f) * 7 + 10.0f), true, 4);
   
    // Initialize create button:
    Text create = create_button(font, "Create", button_size,
        Vector2f(3 * window.getSize().x / 4,
            (window.getSize().y - number_of_buttons * (button_size + 15) - 25) / 2 + (button_size + 15.0f) * 7 + 10.0f), true, 4);

    // Using a thread to display blinking text bar during the active text field:
    Thread* blink_thread = new Thread([&] () {
        for (unsigned enter_text_counter = 0; true; enter_text_counter++) {
            // Display a text bar with some delay:
            if (enter_text_counter >= 500) {
                // Display entered max players number:
                if (max_number_of_players_captured) {
                    if (max_number_of_players_display.getString().getSize() == 1)
                        max_number_of_players_display.setString(to_string(max_number_of_players) + "|");
                    else
                        max_number_of_players_display.setString(to_string(max_number_of_players));
                }
                
                enter_text_counter = 0;

                sf::sleep(seconds(0.2f));
            }
        }
    });

    blink_thread->launch();

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
                    max_number_of_players_title.setFillColor(COLOR_DARK_VIOLET);
                    max_number_of_players_title.setOutlineColor(COLOR_LIGHT_GREEN);
                    if (complexity != 1) {
                        mechanics.setFillColor(COLOR_DARK_VIOLET);
                        mechanics.setOutlineColor(COLOR_LIGHT_GREEN);
                    }
                    if (complexity != 2) {
                        STIC.setFillColor(COLOR_DARK_VIOLET);
                        STIC.setOutlineColor(COLOR_LIGHT_GREEN);
                    }
                    if (complexity != 3) {
                        applied_maths.setFillColor(COLOR_DARK_VIOLET);
                        applied_maths.setOutlineColor(COLOR_LIGHT_GREEN);
                    }
                    back.setFillColor(COLOR_DARK_VIOLET);
                    back.setOutlineColor(COLOR_LIGHT_GREEN);
                    create.setFillColor(COLOR_DARK_VIOLET);
                    create.setOutlineColor(COLOR_LIGHT_GREEN);

                    // If appropriate mouse position was captured:
                    if (captured_button(window, max_number_of_players_title)) {
                        // focus max_number_of_players_title button:
                        max_number_of_players_title.setFillColor(COLOR_YELLOW);
                        max_number_of_players_title.setOutlineColor(COLOR_DARK_BLUE); 
                        focused_button_counter = 1;
                    } else if (captured_button(window, back)) {
                        // focus back button:
                        back.setFillColor(COLOR_YELLOW);
                        back.setOutlineColor(COLOR_DARK_BLUE);
                    } else if (captured_button(window, create)) {
                        // focus create button:
                        create.setFillColor(COLOR_YELLOW);
                        create.setOutlineColor(COLOR_DARK_BLUE);
                    } else if (captured_button(window, mechanics)) {
                        // focus mechanics button:
                        mechanics.setFillColor(COLOR_YELLOW);
                        mechanics.setOutlineColor(COLOR_DARK_BLUE);
                    } else if (captured_button(window, STIC)) {
                        // focus STIC button:
                        STIC.setFillColor(COLOR_YELLOW);
                        STIC.setOutlineColor(COLOR_DARK_BLUE);
                    } else if (captured_button(window, applied_maths)) {
                        // focus applied Maths button:
                        applied_maths.setFillColor(COLOR_YELLOW);
                        applied_maths.setOutlineColor(COLOR_DARK_BLUE);   
                    }
                    break;
                // If we want to type a text:
                case Event::TextEntered:
                    if (event.text.unicode < 128 && event.text.unicode != 13 
                        && event.text.unicode != 8 && event.text.unicode != 9) {
                        if (max_number_of_players_captured && event.text.unicode >= 50 && event.text.unicode <= 52) {
                            // Set a new value of max players number (between 2 and 4):
                            max_number_of_players = event.text.unicode - 48;
                            max_number_of_players_display.setString(to_string(max_number_of_players));
                        
                            // Resize max players number:
                            max_number_of_players_title.setCharacterSize(5 * button_size / 6);
                            max_number_of_players_title.setOutlineThickness(button_size / 6);
                            max_number_of_players_title.setPosition((window.getSize().x - max_number_of_players_title.getGlobalBounds().width) / 4, 
                                (window.getSize().y - number_of_buttons * (button_size + 15) - 25) / 2 + (button_size + 15.0f) * 2 + 5.0f);
                                
                            max_number_of_players_display.setCharacterSize(5 * button_size / 6);
                            max_number_of_players_display.setPosition(3 * (window.getSize().x - max_number_of_players_display.getGlobalBounds().width) / 4, 
                                (window.getSize().y - number_of_buttons * (button_size + 15) - 25) / 2  + (button_size + 15.0f) * 2 + 5.0f);
                        }
                    }
                    break;
                case Event::MouseButtonPressed:
                    switch (event.key.code) {
                        case Mouse::Left:
                            // If appropriate mouse position was captured:
                            // 1) Enter a max players number:
                            if (captured_button(window, max_number_of_players_title)) {
                                max_number_of_players_captured = true;

                                focused_button_counter = 1;
                            // 2) Choose an easy mode:
                            } else if (captured_button(window, mechanics)) {
                                // focus mechanics button:
                                mechanics.setFillColor(COLOR_YELLOW);
                                mechanics.setOutlineColor(COLOR_DARK_BLUE);
                                complexity = 1;
                            // 3) Choose a normal mode:
                            } else if (captured_button(window, STIC)) {
                                // focus STIC button:
                                STIC.setFillColor(COLOR_YELLOW);
                                STIC.setOutlineColor(COLOR_DARK_BLUE);
                                complexity = 2;
                            // 4) Choose a hard mode:
                            } else if (captured_button(window, applied_maths)) {
                                // focus applied Maths button:
                                applied_maths.setFillColor(COLOR_YELLOW);
                                applied_maths.setOutlineColor(COLOR_DARK_BLUE);
                                complexity = 3;
                            // 5) Go back:
                            } else if (captured_button(window, back)) {
                                blink_thread->terminate();
                                return;
                            // 6) Create a new session (if all required information is entered):
                            } else if (captured_button(window, create) && complexity != 0 && max_number_of_players != 0) {
                                blink_thread->terminate();
                                Server* current_session = new Server(false, nickname, max_number_of_players, complexity);
                                session_menu(current_session, nullptr);
                                blink_thread->launch();
                            }
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

                                    max_number_of_players_captured = true;

                                    // focus max_number_of_players_title button:
                                    max_number_of_players_title.setFillColor(COLOR_YELLOW);
                                    max_number_of_players_title.setOutlineColor(COLOR_DARK_BLUE);
                                    focused_button_counter++;
                                    break;
                                case 1:
                                    // in this case, Enter won't do nothing:
                                    if (event.key.code == Keyboard::Return)
                                        break;

                                    // uncapture max players number field:
                                    max_number_of_players_display.setString(to_string(max_number_of_players));
                                    max_number_of_players_captured = false;

                                    // unfocus max_number_of_players_title button:
                                    max_number_of_players_title.setFillColor(COLOR_DARK_VIOLET);
                                    max_number_of_players_title.setOutlineColor(COLOR_LIGHT_GREEN);
                                    // focus mechanics button:
                                    mechanics.setFillColor(COLOR_YELLOW);
                                    mechanics.setOutlineColor(COLOR_DARK_BLUE);
                                    focused_button_counter++;
                                    break;
                                case 2:
                                    // if we have pressed Enter:
                                    if (event.key.code == Keyboard::Return) {
                                        // set complexity:
                                        complexity = 1;

                                        // unfocus STIC button:
                                        STIC.setFillColor(COLOR_DARK_VIOLET);
                                        STIC.setOutlineColor(COLOR_LIGHT_GREEN);
                                        // unfocus applied Maths button:
                                        applied_maths.setFillColor(COLOR_DARK_VIOLET);
                                        applied_maths.setOutlineColor(COLOR_LIGHT_GREEN);
                                    } else {
                                        if (complexity != 1) {
                                            // unfocus mechanics button:
                                            mechanics.setFillColor(COLOR_DARK_VIOLET);
                                            mechanics.setOutlineColor(COLOR_LIGHT_GREEN);
                                        }
                                        
                                        // focus STIC button:
                                        STIC.setFillColor(COLOR_YELLOW);
                                        STIC.setOutlineColor(COLOR_DARK_BLUE);
                                        focused_button_counter++;
                                    }
                                    break;
                                case 3:
                                    // if we have pressed Enter:
                                    if (event.key.code == Keyboard::Return) {
                                        // set complexity:
                                        complexity = 2;

                                        // unfocus mechanics button:
                                        mechanics.setFillColor(COLOR_DARK_VIOLET);
                                        mechanics.setOutlineColor(COLOR_LIGHT_GREEN);
                                        // unfocus applied Maths button:
                                        applied_maths.setFillColor(COLOR_DARK_VIOLET);
                                        applied_maths.setOutlineColor(COLOR_LIGHT_GREEN);
                                    } else {
                                        if (complexity != 2) {
                                            // unfocus STIC button:
                                            STIC.setFillColor(COLOR_DARK_VIOLET);
                                            STIC.setOutlineColor(COLOR_LIGHT_GREEN);
                                        }

                                        // focus applied Maths button:
                                        applied_maths.setFillColor(COLOR_YELLOW);
                                        applied_maths.setOutlineColor(COLOR_DARK_BLUE);
                                        focused_button_counter++;
                                    }
                                    break;
                                case 4:
                                    // if we have pressed Enter:
                                    if (event.key.code == Keyboard::Return) {
                                        // set complexity:
                                        complexity = 3;

                                        // unfocus mechanics button:
                                        mechanics.setFillColor(COLOR_DARK_VIOLET);
                                        mechanics.setOutlineColor(COLOR_LIGHT_GREEN);
                                        // unfocus STIC button:
                                        STIC.setFillColor(COLOR_DARK_VIOLET);
                                        STIC.setOutlineColor(COLOR_LIGHT_GREEN);
                                    } else {
                                        if (complexity != 3) {
                                            // unfocus applied Maths button:
                                            applied_maths.setFillColor(COLOR_DARK_VIOLET);
                                            applied_maths.setOutlineColor(COLOR_LIGHT_GREEN);
                                        }

                                        // focus back button:
                                        back.setFillColor(COLOR_YELLOW);
                                        back.setOutlineColor(COLOR_DARK_BLUE);
                                        focused_button_counter++;
                                    }
                                    break;
                                case 5:
                                    // if we have pressed Enter:
                                    if (event.key.code == Keyboard::Return) {
                                        blink_thread->terminate();
                                        // execute back button:
                                        return;
                                    }

                                    // unfocus back button:
                                    back.setFillColor(COLOR_DARK_VIOLET);
                                    back.setOutlineColor(COLOR_LIGHT_GREEN);
                                    // focus create button:
                                    create.setFillColor(COLOR_YELLOW);
                                    create.setOutlineColor(COLOR_DARK_BLUE);
                                    focused_button_counter++;
                                    break;
                                case 6:
                                    // if we have pressed Enter:
                                    if (event.key.code == Keyboard::Return && complexity != 0 && max_number_of_players != 0) {
                                        blink_thread->terminate();
                                        Server* current_session = new Server(false, nickname, max_number_of_players, complexity);
                                        session_menu(current_session, nullptr);
                                        blink_thread->launch();
                                    } else {
                                        // capture max players number field:
                                        max_number_of_players_captured = true;

                                        // unfocus create button:
                                        create.setFillColor(COLOR_DARK_VIOLET);
                                        create.setOutlineColor(COLOR_LIGHT_GREEN);
                                        // focus max_number_of_players_title button:
                                        max_number_of_players_title.setFillColor(COLOR_YELLOW);
                                        max_number_of_players_title.setOutlineColor(COLOR_DARK_BLUE);
                                        focused_button_counter = 1;
                                    }
                                    break;
                                default:
                                    break;
                            }
                            break;
                        // If we have pushed Backspace:
                        case Keyboard::BackSpace:
                            if (max_number_of_players_captured && max_number_of_players != 0) {
                                // Reset max players number to 0:
                                max_number_of_players = 0;
                                max_number_of_players_display.setString(to_string(max_number_of_players));

                                // Resize max players number:
                                max_number_of_players_title.setCharacterSize(5 * button_size / 6);
                                max_number_of_players_title.setOutlineThickness(button_size / 6);
                                max_number_of_players_title.setPosition((window.getSize().x - max_number_of_players_title.getGlobalBounds().width) / 4, 
                                        (window.getSize().y - number_of_buttons * (button_size + 15) - 25) / 2 + (button_size + 15.0f) * 2 + 5.0f);
                                
                                max_number_of_players_display.setCharacterSize(5 * button_size / 6);
                                max_number_of_players_display.setPosition(3 * (window.getSize().x - max_number_of_players_display.getGlobalBounds().width) / 4, 
                                        (window.getSize().y - number_of_buttons * (button_size + 15) - 25) / 2  + (button_size + 15.0f) * 2 + 5.0f);

                            }
                            break;
                        default:
                            break;
                    }
                    break;
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
                    create_session_title.setCharacterSize(5 * button_size / 6);
                    create_session_title.setPosition((window.getSize().x - create_session_title.getGlobalBounds().width) / 2 - 10.0f, 
                            (window.getSize().y - number_of_buttons * (button_size + 15) - 25) / 2);

                    max_number_of_players_title.setCharacterSize(5 * button_size / 6);
                    max_number_of_players_title.setOutlineThickness(button_size / 6);
                    max_number_of_players_title.setPosition((window.getSize().x - max_number_of_players_title.getGlobalBounds().width) / 4, 
                            (window.getSize().y - number_of_buttons * (button_size + 15) - 25) / 2 + button_size + 20.0f);
                    
                    max_number_of_players_display.setCharacterSize(5 * button_size / 6);
                    max_number_of_players_display.setPosition(3 * (window.getSize().x - max_number_of_players_display.getGlobalBounds().width) / 4, 
                            (window.getSize().y - number_of_buttons * (button_size + 15) - 25) / 2  + button_size + 20.0f + 5.0f);

                    complexity_title.setCharacterSize(5 * button_size / 6);
                    complexity_title.setPosition((window.getSize().x - complexity_title.getGlobalBounds().width) / 2 - 10.0f, 
                            (window.getSize().y - number_of_buttons * (button_size + 15) - 25) / 2 + (button_size + 15.0f) * 2 + 5.0f);

                    mechanics.setCharacterSize(5 * button_size / 6);
                    mechanics.setOutlineThickness(button_size / 6);
                    mechanics.setPosition((window.getSize().x - mechanics.getGlobalBounds().width) / 2, 
                            (window.getSize().y - number_of_buttons * (button_size + 15) - 25) / 2 + (button_size + 15.0f) * 3 + 5.0f);
                    
                    STIC.setCharacterSize(5 * button_size / 6);
                    STIC.setOutlineThickness(button_size / 6);
                    STIC.setPosition((window.getSize().x - STIC.getGlobalBounds().width) / 2, 
                            (window.getSize().y - number_of_buttons * (button_size + 15) - 25) / 2 + (button_size + 15.0f) * 4 + 5.0f);
                    
                    applied_maths.setCharacterSize(5 * button_size / 6);
                    applied_maths.setOutlineThickness(button_size / 6);
                    applied_maths.setPosition((window.getSize().x - applied_maths.getGlobalBounds().width) / 2, 
                            (window.getSize().y - number_of_buttons * (button_size + 15) - 25) / 2 + (button_size + 15.0f) * 5 + 6.0f);

                    back.setCharacterSize(5 * button_size / 6);
                    back.setOutlineThickness(button_size / 6);
                    back.setPosition((window.getSize().x - back.getGlobalBounds().width) / 4, 
                            (window.getSize().y - number_of_buttons * (button_size + 15) - 25) / 2 + (button_size + 15.0f) * 6 + 10.0f);
                    
                    create.setCharacterSize(5 * button_size / 6);
                    create.setOutlineThickness(button_size / 6);
                    create.setPosition(3 * (window.getSize().x - create.getGlobalBounds().width) / 4, 
                            (window.getSize().y - number_of_buttons * (button_size + 15) - 25) / 2 + (button_size + 15.0f) * 6 + 10.0f);
                    break;
                default:
                    break;
            }
        }

        // Clear window:
        window.clear();

        // Draw background:
        window.draw(background);

        // Draw a menu:
        window.draw(create_session_title);
        window.draw(max_number_of_players_title);
        window.draw(max_number_of_players_display);
        window.draw(complexity_title);
        window.draw(mechanics);
        window.draw(STIC);
        window.draw(applied_maths);
        window.draw(back);
        window.draw(create);
        window.display();
    }  
};

// Manage just created session as a server:
void Menu::session_menu(Server* current_session, Client* current_client) {
    // counter of the currently chosen button:
    unsigned focused_button_counter = 0;
    // number of buttons:
    const unsigned number_of_buttons = 7;
    // button size:
    double button_size = min(min(60.0f, 3.5f * (window.getSize().x - 10.0f) / 12), 
        (window.getSize().y - 50.0f - 15.0f * (number_of_buttons - 1)) / number_of_buttons);

    request_status status = NOT_CHANGED;
    // Initialize client-server communication:
    Thread* listen_thread = nullptr;    
    if (current_client == nullptr) {
        // Look for the clients:
        listen_thread = new Thread([&] () { current_session->listen_clients(status); });

        listen_thread->launch();
    } else if (current_session == nullptr) {
        // Look for the clients:
        listen_thread = new Thread([&] () { current_client->listen_server(status); });

        listen_thread->launch();
    }

    // Create a list of clients and initialize server name:
    vector<client_data> *player_list = nullptr;
    string server_name = "";
    if (current_client == nullptr) {
        player_list = new vector<client_data>(current_session->get_clients());
        server_name = current_session->get_player_nickname();
    } else if (current_session == nullptr) {
        player_list = new vector<client_data>(current_client->get_server_data().clients);
        server_name = current_client->get_server_data().name;
    }
    
    // Initialize the index of current player:
    unsigned current_player_index = 0;
    for (unsigned i = 0; i < player_list->size(); i++)
        if (player_list->at(i).address == IpAddress::getLocalAddress()) {
            current_player_index = i;
            break;
        }
    
    // Initialize session title:
    Text session_title = create_button(font, "Session " + server_name, button_size,
        Vector2f(window.getSize().x / 2,
            (window.getSize().y - number_of_buttons * (button_size + 15) - 25) / 2 + 5.0f), false);

    // Initialize player's title:
    Text player_title = create_button(font, "Player", button_size,
        Vector2f(window.getSize().x / 4,
            (window.getSize().y - number_of_buttons * (button_size + 15) - 25) / 2 + button_size + 15.0f + 20.0f), false, 4);

    // Initialize status title:
    Text status_title = create_button(font, "Status", button_size,
        Vector2f(3 * window.getSize().x / 4,
            (window.getSize().y - number_of_buttons * (button_size + 15) - 25) / 2 + button_size + 15.0f + 20.0f), false, 4);

    // Initialize player list:
    vector<Text> player_list_titles;
    for (unsigned i = 0; i < player_list->size(); i++) {
        player_list_titles.emplace_back(create_button(font, player_list->at(i).nickname, button_size,
            Vector2f(window.getSize().x / 4,
            (window.getSize().y - number_of_buttons * (button_size + 15) - 25) / 2 + (button_size + 15.0f) * (i + 2) + 5.0f), false, 4));
        player_list_titles.emplace_back(create_button(font, player_list->at(i).status == STATUS_READY ? "Ready" : "Not Ready", button_size,
            Vector2f(3 * window.getSize().x / 4,
            (window.getSize().y - number_of_buttons * (button_size + 15) - 25) / 2 + (button_size + 15.0f) * (i + 2) + 5.0f), false, 4));
    }

    // control button multiplier:
    unsigned button_multiplier = 2;
    if (current_client == nullptr)
        button_multiplier = 3;

    // Initialize disconnect button:
    Text disconnect = create_button(font, "Disconnect", button_size,
        Vector2f(window.getSize().x / (2 * button_multiplier),
            (window.getSize().y - number_of_buttons * (button_size + 15) - 25) / 2 + (button_size + 15.0f) * 5 + 10.0f), true, 2 * button_multiplier);
   
    // Initialize ready button:
    Text ready = create_button(font, "Ready", button_size,
        Vector2f(3 * window.getSize().x / (2 * button_multiplier),
            (window.getSize().y - number_of_buttons * (button_size + 15) - 25) / 2 + (button_size + 15.0f) * 5 + 10.0f), true, 2 * button_multiplier);

    // Server has one more button:
    Text start;
    if (button_multiplier == 3)
        // Initialize start button:
        start = create_button(font, "Start", button_size,
            Vector2f(5 * window.getSize().x / (2 * button_multiplier),
                (window.getSize().y - number_of_buttons * (button_size + 15) - 25) / 2 + (button_size + 15.0f) * 5 + 10.0f), true, 2 * button_multiplier);

    while(window.isOpen()) {
        // If we have some changes:
        if (status == CHANGED) {
            player_list->clear();
            if (current_client == nullptr)
                player_list = new vector<client_data>(current_session->get_clients());
            else if (current_session == nullptr)
                player_list = new vector<client_data>(current_client->get_server_data().clients);

            // Refresh player list:
            player_list_titles.clear();
            for (unsigned i = 0; i < player_list->size(); i++) {
                player_list_titles.emplace_back(create_button(font, player_list->at(i).nickname, button_size,
                    Vector2f(window.getSize().x / 4,
                    (window.getSize().y - number_of_buttons * (button_size + 15) - 25) / 2 + (button_size + 15.0f) * (i + 2) + 5.0f), false, 4));
                player_list_titles.emplace_back(create_button(font, player_list->at(i).status == STATUS_READY ? "Ready" : "Not Ready", button_size,
                    Vector2f(3 * window.getSize().x / 4,
                    (window.getSize().y - number_of_buttons * (button_size + 15) - 25) / 2 + (button_size + 15.0f) * (i + 2) + 5.0f), false, 4));
            }

            status = NOT_CHANGED;
        } else if (status == GAME_START && current_session == nullptr) {
            listen_thread->terminate();
            multiplayer_game(nullptr, current_client); 
        }

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
                    disconnect.setFillColor(COLOR_DARK_VIOLET);
                    disconnect.setOutlineColor(COLOR_LIGHT_GREEN);
                    ready.setFillColor(COLOR_DARK_VIOLET);
                    ready.setOutlineColor(COLOR_LIGHT_GREEN);
                    if (button_multiplier == 3) {
                        start.setFillColor(COLOR_DARK_VIOLET);
                        start.setOutlineColor(COLOR_LIGHT_GREEN);
                    }

                    // If appropriate mouse position was captured:
                    if (captured_button(window, disconnect)) {
                        // focus disconnect button:
                        disconnect.setFillColor(COLOR_YELLOW);
                        disconnect.setOutlineColor(COLOR_DARK_BLUE);
                        focused_button_counter = 1;
                    } else if (captured_button(window, ready)) {
                        // focus ready button:
                        ready.setFillColor(COLOR_YELLOW);
                        ready.setOutlineColor(COLOR_DARK_BLUE); 
                        focused_button_counter = 2;
                    } else if (button_multiplier == 3 && captured_button(window, start)) {
                        // focus start button:
                        start.setFillColor(COLOR_YELLOW);
                        start.setOutlineColor(COLOR_DARK_BLUE); 
                        focused_button_counter = 3;
                    }
                    break;
                case Event::MouseButtonPressed:
                    switch (event.key.code) {
                        case Mouse::Left:
                            // If appropriate mouse position was captured:
                            // 1) Go back:
                            if (captured_button(window, disconnect)) {
                                if (current_client == nullptr) {
                                    listen_thread->terminate();
                                    current_session->disconnect();
                                    current_session->disconnect_udp_socket();
                                    return;
                                } else if (current_session == nullptr) {
                                    listen_thread->terminate();
                                    request_status status;
                                    current_client->disconnect_server(status);
                                    current_client->disconnect_udp_socket();
                                    find_servers(current_client->get_player_nickname());
                                    return;
                                }
                                break;
                            // 2) Get ready or not:
                            } else if (captured_button(window, ready)) { 
                                if (player_list->at(current_player_index).status == STATUS_READY) {
                                    ready.setString("Ready");
                                    if (current_client == nullptr)
                                        current_session->ready(false);
                                    else if (current_session == nullptr)
                                        current_client->ready(false);
                                    player_list->at(current_player_index).status = STATUS_NOT_READY;
                                    player_list_titles[2 * current_player_index + 1].setString("Not Ready");
                                } else {
                                    ready.setString("Not Ready");
                                    if (current_client == nullptr)
                                        current_session->ready(true);
                                    else if (current_session == nullptr)
                                        current_client->ready(true);
                                    player_list->at(current_player_index).status = STATUS_READY;
                                    player_list_titles[2 * current_player_index + 1].setString("Ready");
                                }
                            // 3) Start new multiplayer game:
                            } else if (button_multiplier == 3 && captured_button(window, start) && current_session->start()) {
                                listen_thread->terminate();
                                multiplayer_game(current_session, nullptr);
                            }
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

                                    // focus disconnect button:
                                    disconnect.setFillColor(COLOR_YELLOW);
                                    disconnect.setOutlineColor(COLOR_DARK_BLUE);
                                    focused_button_counter++;
                                    break;
                                case 1:
                                    // if we have pressed Enter:
                                    if (event.key.code == Keyboard::Return) {
                                        if (current_client == nullptr) {
                                            listen_thread->terminate();
                                            current_session->disconnect();
                                            current_session->disconnect_udp_socket();
                                            return;
                                        } else if (current_session == nullptr) {
                                            request_status status;
                                            current_client->disconnect_server(status);
                                            current_client->disconnect_udp_socket();
                                            find_servers(current_client->get_player_nickname());
                                            return;
                                        }
                                        break;
                                    } else {
                                        // unfocus disconnect button:
                                        disconnect.setFillColor(COLOR_DARK_VIOLET);
                                        disconnect.setOutlineColor(COLOR_LIGHT_GREEN);
                                        // focus ready button:
                                        ready.setFillColor(COLOR_YELLOW);
                                        ready.setOutlineColor(COLOR_DARK_BLUE);
                                        focused_button_counter++;
                                    }
                                    break;
                                case 2:
                                    // if we have pressed Enter:
                                    if (event.key.code == Keyboard::Return) {
                                        // execute ready button:
                                        if (player_list->at(current_player_index).status == STATUS_READY) {
                                            ready.setString("Ready");
                                            if (current_client == nullptr)
                                                current_session->ready(false);
                                            else if (current_session == nullptr)
                                                current_client->ready(false);
                                            player_list->at(current_player_index).status = STATUS_NOT_READY;
                                            player_list_titles[2 * current_player_index + 1].setString("Not Ready");
                                        } else {
                                            ready.setString("Not Ready");
                                            if (current_client == nullptr)
                                                current_session->ready(true);
                                            else if (current_session == nullptr)
                                                current_client->ready(true);
                                            player_list->at(current_player_index).status = STATUS_READY;
                                            player_list_titles[2 * current_player_index + 1].setString("Ready");
                                        }
                                    } else {
                                        // unfocus ready button:
                                        ready.setFillColor(COLOR_DARK_VIOLET);
                                        ready.setOutlineColor(COLOR_LIGHT_GREEN);
                                        // for the client:
                                        if (button_multiplier == 2) {
                                            // focus disconnect button:
                                            disconnect.setFillColor(COLOR_YELLOW);
                                            disconnect.setOutlineColor(COLOR_DARK_BLUE);
                                            focused_button_counter = 1;
                                        // for the server:
                                        } else if (button_multiplier == 3) {
                                            // focus start button:
                                            start.setFillColor(COLOR_YELLOW);
                                            start.setOutlineColor(COLOR_DARK_BLUE);
                                            focused_button_counter++;
                                        }
                                    }
                                    break;
                                case 3:
                                    // if we have pressed Enter:
                                    if (event.key.code == Keyboard::Return && current_session->start()) {
                                        listen_thread->terminate();
                                        // execute start button:
                                        multiplayer_game(current_session, nullptr);
                                    } else if (event.key.code == Keyboard::Tab) {
                                        // unfocus start button:
                                        start.setFillColor(COLOR_DARK_VIOLET);
                                        start.setOutlineColor(COLOR_LIGHT_GREEN);
                                        
                                        // focus disconnect button:
                                        disconnect.setFillColor(COLOR_YELLOW);
                                        disconnect.setOutlineColor(COLOR_DARK_BLUE);
                                        focused_button_counter = 1;
                                    }
                                    break;
                                default:
                                    break;
                            }
                            break;
                        default:
                            break;
                    }
                    break;
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
                    session_title.setCharacterSize(5 * button_size / 6);
                    session_title.setPosition((window.getSize().x - session_title.getGlobalBounds().width) / 2, 
                            (window.getSize().y - number_of_buttons * (button_size + 15) - 25) / 2);

                    player_title.setCharacterSize(5 * button_size / 6);
                    player_title.setPosition((window.getSize().x - player_title.getGlobalBounds().width) / 4,
                        (window.getSize().y - number_of_buttons * (button_size + 15) - 25) / 2 + button_size + 15.0f + 20.0f);

                    status_title.setCharacterSize(5 * button_size / 6);
                    status_title.setPosition(3 * (window.getSize().x - status_title.getGlobalBounds().width) / 4,
                        (window.getSize().y - number_of_buttons * (button_size + 15) - 25) / 2 + button_size + 15.0f + 20.0f);

                    for (unsigned i = 0; i < player_list_titles.size() / 2; i++) {
                        player_list_titles[2 * i].setCharacterSize(5 * button_size / 6);
                        player_list_titles[2 * i].setPosition((window.getSize().x - player_list_titles[2 * i].getGlobalBounds().width) / 4,
                            (window.getSize().y - number_of_buttons * (button_size + 15) - 25) / 2 + (button_size + 15.0f) * (i + 2) + 5.0f);
                        player_list_titles[2 * i + 1].setCharacterSize(5 * button_size / 6);
                        player_list_titles[2 * i + 1].setPosition(3 * (window.getSize().x - player_list_titles[2 * i + 1].getGlobalBounds().width) / 4,
                            (window.getSize().y - number_of_buttons * (button_size + 15) - 25) / 2 + (button_size + 15.0f) * (i + 2) + 5.0f);
                    }

                    disconnect.setCharacterSize(5 * button_size / 6);
                    disconnect.setOutlineThickness(button_size / 6);
                    disconnect.setPosition((window.getSize().x - disconnect.getGlobalBounds().width) / (2 * button_multiplier),
                        (window.getSize().y - number_of_buttons * (button_size + 15) - 25) / 2 + (button_size + 15.0f) * 5 + 10.0f);
                    
                    ready.setCharacterSize(5 * button_size / 6);
                    ready.setOutlineThickness(button_size / 6);
                    ready.setPosition(3 * (window.getSize().x - ready.getGlobalBounds().width) / (2 * button_multiplier),
                        (window.getSize().y - number_of_buttons * (button_size + 15) - 25) / 2 + (button_size + 15.0f) * 5 + 10.0f);
                    
                    if (button_multiplier == 3) {
                        start.setCharacterSize(5 * button_size / 6);
                        start.setOutlineThickness(button_size / 6);
                        start.setPosition(5 * (window.getSize().x - start.getGlobalBounds().width) / (2 * button_multiplier),
                            (window.getSize().y - number_of_buttons * (button_size + 15) - 25) / 2 + (button_size + 15.0f) * 5 + 10.0f);
                    }
                    
                    break;
                default:
                    break;
            }
        }

        // Clear window:
        window.clear();

        // Draw background:
        window.draw(background);

        // Draw a menu:
        window.draw(session_title);
        window.draw(player_title);
        window.draw(status_title);
        for (unsigned i = 0; i < player_list_titles.size(); i++)
            window.draw(player_list_titles[i]);
        window.draw(disconnect);
        window.draw(ready);
        if (button_multiplier == 3)
            window.draw(start);
        window.display();
    }
};

// Find servers function:
void Menu::find_servers(const string& nickname) {
    // counter of the currently chosen button:
    unsigned focused_button_counter = 0;

    map<unsigned, string> complexities;
    complexities[0] = "Mechanics";
    complexities[1] = "STIC";
    complexities[2] = "Applied Maths";

    // Connection request status:
    request_status status = UNASSIGNED;
    // Serch servers status:
    request_status search_status = UNASSIGNED;

    // Create a client:
    Client* current_client = new Client(false, nickname);

    // Looking for servers:
    Thread* search_thread = new Thread([&] () { current_client->search_servers(search_status); });

    search_thread->launch();

    // Create a thread to establish connection:
    Thread* connect_thread;

    // set the wait image:
    Texture texture;
    try {
        if (!texture.loadFromFile("images/wait.jpg")) 
            throw 0;
    } catch (int e) {
        if (e == 0)
            cout << "Sorry, wait image not found!" << endl;
    }

    // initalize the wait background:
    RectangleShape wait_background;
    wait_background.setSize(Vector2f(window.getSize().x, window.getSize().y));
    wait_background.setFillColor(Color(255, 255, 255, 200));

    // create a wait sprite itself:
    Sprite wait;
    wait.setTexture(texture);
    wait.setColor(Color(255, 255, 255, 100));
    wait.setPosition((window.getSize().x - wait.getGlobalBounds().width) / 2, (window.getSize().y - wait.getGlobalBounds().height) / 2);

    // Number of buttons:
    unsigned number_of_buttons = 8;
    // Button size:
    double button_size = min(min(60.0f, 3.5f * (window.getSize().x - 10.0f) / 12), 
        (window.getSize().y - 50.0f - 15.0f * (number_of_buttons - 1)) / number_of_buttons);
    
    // Currently chosen server:
    int chosen_server = -1;

    // Initialize server list title:
    Text server_list_title = create_button(font, "Server List", button_size,
        Vector2f(window.getSize().x / 2,
            (window.getSize().y - number_of_buttons * (button_size + 15) - 25) / 2 + 5.0f), false);

    // Initialize server_name's title:
    Text server_name_title = create_button(font, "Server Name", button_size,
        Vector2f(window.getSize().x / 6,
            (window.getSize().y - number_of_buttons * (button_size + 15) - 25) / 2 + button_size + 20.0f), false, 6);

    // Initialize complexity title:
    Text complexity_title = create_button(font, "Complexity", button_size,
        Vector2f(3 * window.getSize().x / 6,
            (window.getSize().y - number_of_buttons * (button_size + 15) - 25) / 2 + button_size + 20.0f), false, 6);

    // Initialize number of players title:
    Text number_of_players_title = create_button(font, "Players", button_size,
        Vector2f(5 * window.getSize().x / 6,
            (window.getSize().y - number_of_buttons * (button_size + 15) - 25) / 2 + button_size + 20.0f), false, 6);

    vector<Text> server_info;

    // Initialize back button:
    Text back = create_button(font, "Back", button_size,
        Vector2f(window.getSize().x / 6 - 10.0f,
            (window.getSize().y - number_of_buttons * (button_size + 15) - 25) / 2 + (button_size + 15.0f) * 6 + 10.0f), true, 6);
   
    // Initialize refresh button:
    Text refresh = create_button(font, "Refresh", button_size,
        Vector2f(3 * window.getSize().x / 6 - 10.0f,
            (window.getSize().y - number_of_buttons * (button_size + 15) - 25) / 2 + (button_size + 15.0f) * 6 + 10.0f), true, 6);
   
    // Initialize connect button:
    Text connect = create_button(font, "Connect", button_size,
        Vector2f(5 * window.getSize().x / 6 - 10.0f,
            (window.getSize().y - number_of_buttons * (button_size + 15) - 25) / 2 + (button_size + 15.0f) * 6 + 10.0f), true, 6);
   
   
   // update_thread->launch();

    while(window.isOpen()) {
        if (search_status == CHANGED) {
            server_info.clear();

            // Fullfill the list of servers:
            for (unsigned i = server_info.size() / 3; i < current_client->get_servers().size(); i++) {
                server_info.emplace_back(create_button(font, current_client->get_servers()[i].name, button_size,
                    Vector2f(window.getSize().x / 6,
                    (window.getSize().y - number_of_buttons * (button_size + 15) - 25) / 2 + (button_size + 15.0f) * (i + 2) + 5.0f), true, 6));
                server_info.emplace_back(create_button(font, complexities[(unsigned) current_client->get_servers()[i].level - 1], button_size,
                    Vector2f(3 * window.getSize().x / 6,
                    (window.getSize().y - number_of_buttons * (button_size + 15) - 25) / 2 + (button_size + 15.0f) * (i + 2) + 5.0f), true, 6));
                server_info.emplace_back(create_button(font, to_string(current_client->get_servers()[i].clients_quantity) + "/" + to_string(current_client->get_servers()[i].max_clients), button_size,
                    Vector2f(5 * window.getSize().x / 6,
                    (window.getSize().y - number_of_buttons * (button_size + 15) - 25) / 2 + (button_size + 15.0f) * (i + 2) + 5.0f), true, 6));
            }

            search_status = NOT_CHANGED;
        }

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
                    for (int i = 0; (unsigned) i < server_info.size(); i += 3) {
                        for (unsigned j = 0; j < 3; j++) {
                            if (chosen_server != i / 3) {
                                server_info[i + j].setFillColor(COLOR_DARK_VIOLET);
                                server_info[i + j].setOutlineColor(COLOR_LIGHT_GREEN);
                            }
                        }
                    }
                    back.setFillColor(COLOR_DARK_VIOLET);
                    back.setOutlineColor(COLOR_LIGHT_GREEN);
                    refresh.setFillColor(COLOR_DARK_VIOLET);
                    refresh.setOutlineColor(COLOR_LIGHT_GREEN);
                    connect.setFillColor(COLOR_DARK_VIOLET);
                    connect.setOutlineColor(COLOR_LIGHT_GREEN);

                    // If appropriate mouse position was captured:
                    for (unsigned i = 0; i < server_info.size(); i += 3) {
                        if (status != NOT_READY && (captured_button(window, server_info[i])
                            || captured_button(window, server_info[i + 1])
                            || captured_button(window, server_info[i + 2]))) {
                            // focus proper server buttons:
                            server_info[i].setFillColor(COLOR_YELLOW);
                            server_info[i].setOutlineColor(COLOR_DARK_BLUE);
                            server_info[i + 1].setFillColor(COLOR_YELLOW);
                            server_info[i + 1].setOutlineColor(COLOR_DARK_BLUE);
                            server_info[i + 2].setFillColor(COLOR_YELLOW);
                            server_info[i + 2].setOutlineColor(COLOR_DARK_BLUE);
                            focused_button_counter = i / 3 + 1;
                            break;
                        }
                    }
                    if (captured_button(window, back) && status != NOT_READY) {
                        // focus back button:
                        back.setFillColor(COLOR_YELLOW);
                        back.setOutlineColor(COLOR_DARK_BLUE);
                        focused_button_counter = current_client->get_servers().size() + 1;
                    } else if (captured_button(window, refresh) && status != NOT_READY) {
                        // focus refresh button:
                        refresh.setFillColor(COLOR_YELLOW);
                        refresh.setOutlineColor(COLOR_DARK_BLUE); 
                        focused_button_counter = current_client->get_servers().size() + 2;
                    } else if (captured_button(window, connect) && status != NOT_READY) {
                        // focus connect button:
                        connect.setFillColor(COLOR_YELLOW);
                        connect.setOutlineColor(COLOR_DARK_BLUE); 
                        focused_button_counter = current_client->get_servers().size() + 3;
                    } 
                    break;
                case Event::MouseButtonPressed:
                    switch (event.key.code) {
                        case Mouse::Left:
                            // If appropriate mouse position was captured:
                            for (unsigned i = 0; i < server_info.size(); i += 3) {
                                if (status != NOT_READY && (captured_button(window, server_info[i])
                                    || captured_button(window, server_info[i + 1])
                                    || captured_button(window, server_info[i + 2]))) {
                                    // i + 1) Choose a proper server:
                                    chosen_server = i / 3;
                                    break;
                                }
                            }
                            // current_client->get_servers().size() + 1) Go back:
                            if (captured_button(window, back) && status != NOT_READY) {
                                search_thread->terminate();

                                current_client->disconnect_udp_socket();
                                return;
                            // current_client->get_servers().size() + 2) Refresh list of servers:
                            } else if (captured_button(window, refresh) && status != NOT_READY) {
                                search_thread->terminate();

                                search_thread->launch();
                            // current_client->get_servers().size() + 3) Get connect or not:
                            } else if (captured_button(window, connect) && chosen_server != -1 && status != NOT_READY) {
                                search_thread->terminate();

                                connect_thread = new Thread([&] () { current_client->connect_server(chosen_server, status); });
                            
                                connect_thread->launch();
                            };
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
                            if (focused_button_counter == 0 && current_client->get_servers().size() && 
                                event.key.code != Keyboard::Return && status != NOT_READY) {
                                // in this case, Enter won't do nothing:
                                // focus proper server buttons:
                                server_info[0].setFillColor(COLOR_YELLOW);
                                server_info[0].setOutlineColor(COLOR_DARK_BLUE);
                                server_info[1].setFillColor(COLOR_YELLOW);
                                server_info[1].setOutlineColor(COLOR_DARK_BLUE);
                                server_info[2].setFillColor(COLOR_YELLOW);
                                server_info[2].setOutlineColor(COLOR_DARK_BLUE);
                                focused_button_counter++;
                                break;
                            } else if (focused_button_counter == 0 && current_client->get_servers().size() == 0 && status != NOT_READY) {
                                // in this case, Enter won't do nothing:
                                if (event.key.code == Keyboard::Return)
                                    break;

                                // focus back button:
                                back.setFillColor(COLOR_YELLOW);
                                back.setOutlineColor(COLOR_DARK_BLUE);
                                focused_button_counter++;
                                break;
                            } else if (focused_button_counter == current_client->get_servers().size() + 1 && status != NOT_READY) {
                                // if we have pressed Enter:
                                if (event.key.code == Keyboard::Return) {
                                    // execute back button:
                                    search_thread->terminate();

                                    current_client->disconnect_udp_socket();
                                    return;
                                }

                                // unfocus back button:
                                back.setFillColor(COLOR_DARK_VIOLET);
                                back.setOutlineColor(COLOR_LIGHT_GREEN);
                                // focus refresh button:
                                refresh.setFillColor(COLOR_YELLOW);
                                refresh.setOutlineColor(COLOR_DARK_BLUE);
                                focused_button_counter++;
                                break;
                            } else if (focused_button_counter == current_client->get_servers().size() + 2 && status != NOT_READY) {
                                // if we have pressed Enter:
                                if (event.key.code == Keyboard::Return) {
                                    // execute refresh button:
                                    search_thread->terminate();

                                    search_thread->launch();
                                    break;
                                } else {
                                    // unfocus refresh button:
                                    refresh.setFillColor(COLOR_DARK_VIOLET);
                                    refresh.setOutlineColor(COLOR_LIGHT_GREEN);
                                    // focus connect button:
                                    connect.setFillColor(COLOR_YELLOW);
                                    connect.setOutlineColor(COLOR_DARK_BLUE);
                                    focused_button_counter++;
                                    break;
                                }
                            } else if (focused_button_counter == current_client->get_servers().size() + 3 && status != NOT_READY) {
                                // if we have pressed Enter:
                                if (event.key.code == Keyboard::Return) {
                                    search_thread->terminate();
                                
                                    // execute connect button:
                                    connect_thread = new Thread([&] () { current_client->connect_server(chosen_server, status); });
                                
                                    connect_thread->launch();
                                } else {
                                    // unfocus connect button:
                                    connect.setFillColor(COLOR_DARK_VIOLET);
                                    connect.setOutlineColor(COLOR_LIGHT_GREEN);
                                    
                                    // if we have some servers:
                                    if (current_client->get_servers().size() != 0) {
                                        // focus proper server buttons:
                                        server_info[0].setFillColor(COLOR_YELLOW);
                                        server_info[0].setOutlineColor(COLOR_DARK_BLUE);
                                        server_info[1].setFillColor(COLOR_YELLOW);
                                        server_info[1].setOutlineColor(COLOR_DARK_BLUE);
                                        server_info[2].setFillColor(COLOR_YELLOW);
                                        server_info[2].setOutlineColor(COLOR_DARK_BLUE);
                                    } else {
                                        // focus back button:
                                        back.setFillColor(COLOR_YELLOW);
                                        back.setOutlineColor(COLOR_DARK_BLUE);
                                    }
                                    focused_button_counter = 1;
                                }
                                break;
                            }
                            for (int i = 0; (unsigned) i < server_info.size(); i+=3) {
                                if (focused_button_counter == (unsigned) i / 3 + 1 && status != NOT_READY) {
                                    // if we have pressed Enter:
                                    if (event.key.code == Keyboard::Return) {
                                        // choose a server:
                                        chosen_server = i / 3;
                                        break;
                                    }

                                    if (chosen_server != i / 3) {
                                        // unfocus current server button:
                                        server_info[i].setFillColor(COLOR_DARK_VIOLET);
                                        server_info[i].setOutlineColor(COLOR_LIGHT_GREEN);
                                        server_info[i + 1].setFillColor(COLOR_DARK_VIOLET);
                                        server_info[i + 1].setOutlineColor(COLOR_LIGHT_GREEN);
                                        server_info[i + 2].setFillColor(COLOR_DARK_VIOLET);
                                        server_info[i + 2].setOutlineColor(COLOR_LIGHT_GREEN);
                                    }
                                    
                                    // if it is not a last server:
                                    if ((unsigned) i / 3 < current_client->get_servers().size() - 1) {
                                        // focus next server button:
                                        server_info[i + 3].setFillColor(COLOR_YELLOW);
                                        server_info[i + 3].setOutlineColor(COLOR_DARK_BLUE);
                                        server_info[i + 4].setFillColor(COLOR_YELLOW);
                                        server_info[i + 4].setOutlineColor(COLOR_DARK_BLUE);
                                        server_info[i + 5].setFillColor(COLOR_YELLOW);
                                        server_info[i + 5].setOutlineColor(COLOR_DARK_BLUE);
                                    } else {
                                        // focus back button:
                                        back.setFillColor(COLOR_YELLOW);
                                        back.setOutlineColor(COLOR_DARK_BLUE);
                                    }
                                    focused_button_counter++;
                                    break;
                                }
                            }
                            break;
                        default:
                            break;
                    }
                    break;
                // if we have changed window's size:
                case Event::Resized:
                    // set minimal window size:
                    if (window.getSize().y < 600)
                        window.setSize(Vector2u (window.getSize().x, 600));

                    // update button size:
                    button_size = min(min(60.0f, 3.5f * (window.getSize().x - 10.0f) / 18), 
                        (window.getSize().y - 50.0f - 15.0f * (number_of_buttons - 1)) / number_of_buttons);

                    wait.setPosition((window.getSize().x - wait.getGlobalBounds().width) / 2, (window.getSize().y - wait.getGlobalBounds().height) / 2);

                    // update view:
                    window.setView(View(FloatRect(0.0f, 0.0f, (float) window.getSize().x, (float) window.getSize().y)));

                    // update all the buttons and their positions:
                    server_list_title.setCharacterSize(5 * button_size / 6);
                    server_list_title.setPosition((window.getSize().x - server_list_title.getGlobalBounds().width) / 2 - 10.0f, 
                            (window.getSize().y - number_of_buttons * (button_size + 15) - 25) / 2);

                    server_name_title.setCharacterSize(5 * button_size / 6);
                    server_name_title.setPosition((window.getSize().x - server_name_title.getGlobalBounds().width) / 6,
                        (window.getSize().y - number_of_buttons * (button_size + 15) - 25) / 2 + button_size + 20.0f);

                    complexity_title.setCharacterSize(5 * button_size / 6);
                    complexity_title.setPosition((3 * (window.getSize().x) - complexity_title.getGlobalBounds().width) / 6,
                        (window.getSize().y - number_of_buttons * (button_size + 15) - 25) / 2 + button_size + 20.0f);

                    number_of_players_title.setCharacterSize(5 * button_size / 6);
                    number_of_players_title.setPosition((5 * (window.getSize().x) - number_of_players_title.getGlobalBounds().width) / 6,
                        (window.getSize().y - number_of_buttons * (button_size + 15) - 25) / 2 + button_size + 20.0f);

                    for (unsigned i = 0; i < server_info.size() / 3; i++) {
                        for (unsigned j = 0; j < 3; j++) {
                            server_info[3 * i + j].setCharacterSize(5 * button_size / 6);
                            server_info[3 * i + j].setOutlineThickness(button_size / 6);
                            server_info[3 * i + j].setPosition(((j + 1) * 2 - 1) * (window.getSize().x - server_info[i + j].getGlobalBounds().width) / 6,
                                (window.getSize().y - number_of_buttons * (button_size + 15) - 25) / 2 + (button_size + 15.0f) * (i + 2) + 5.0f);        
                        }
                    }

                    back.setCharacterSize(5 * button_size / 6);
                    back.setOutlineThickness(button_size / 6);
                    back.setPosition((window.getSize().x - back.getGlobalBounds().width) / 6 - 10.0f,
                        (window.getSize().y - number_of_buttons * (button_size + 15) - 25) / 2 + (button_size + 15.0f) * 6 + 10.0f);

                    refresh.setCharacterSize(5 * button_size / 6);
                    refresh.setOutlineThickness(button_size / 6);
                    refresh.setPosition(3 * (window.getSize().x - refresh.getGlobalBounds().width) / 6 - 10.0f,
                        (window.getSize().y - number_of_buttons * (button_size + 15) - 25) / 2 + (button_size + 15.0f) * 6 + 10.0f);

                    connect.setCharacterSize(5 * button_size / 6);
                    connect.setOutlineThickness(button_size / 6);
                    connect.setPosition(5 * (window.getSize().x - connect.getGlobalBounds().width) / 6 - 10.0f,
                        (window.getSize().y - number_of_buttons * (button_size + 15) - 25) / 2 + (button_size + 15.0f) * 6 + 10.0f);
                    break;
                default:
                    break;
            }
        }

        if (status == SUCCESS)
            session_menu(nullptr, current_client);

        // Clear window:
        window.clear();

        // Draw background:
        window.draw(background);

        // Draw a menu:
        window.draw(server_list_title);
        window.draw(server_name_title);
        window.draw(complexity_title);
        window.draw(number_of_players_title);
        for (unsigned i = 0; i < server_info.size(); i++)
            window.draw(server_info[i]);
        window.draw(back);
        window.draw(refresh);
        window.draw(connect);
        if (status == NOT_READY) {
            window.draw(wait_background);
            window.draw(wait);
        }
            
        window.display();
    }
};

void Menu::complexity_menu() {
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
    
    // Initialize applied Maths button:
    Text applied_maths = create_button(font, "Applied Maths", button_size,
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
                    mechanics.setFillColor(COLOR_DARK_VIOLET);
                    mechanics.setOutlineColor(COLOR_LIGHT_GREEN);
                    STIC.setFillColor(COLOR_DARK_VIOLET);
                    STIC.setOutlineColor(COLOR_LIGHT_GREEN);
                    applied_maths.setFillColor(COLOR_DARK_VIOLET);
                    applied_maths.setOutlineColor(COLOR_LIGHT_GREEN);
                    back.setFillColor(COLOR_DARK_VIOLET);
                    back.setOutlineColor(COLOR_LIGHT_GREEN);

                    // If appropriate mouse position was captured:
                    if (captured_button(window, mechanics)) {
                        // focus mechanics button:
                        mechanics.setFillColor(COLOR_YELLOW);
                        mechanics.setOutlineColor(COLOR_DARK_BLUE);
                        focused_button_counter = 1;
                    } else if (captured_button(window, STIC)) {
                        // focus STIC button:
                        STIC.setFillColor(COLOR_YELLOW);
                        STIC.setOutlineColor(COLOR_DARK_BLUE);
                        focused_button_counter = 2;
                    } else if (captured_button(window, applied_maths)) {
                        // focus applied Maths button:
                        applied_maths.setFillColor(COLOR_YELLOW);
                        applied_maths.setOutlineColor(COLOR_DARK_BLUE);
                        focused_button_counter = 3;             
                    } else if (captured_button(window, back)) {
                        // focus back button:
                        back.setFillColor(COLOR_YELLOW);
                        back.setOutlineColor(COLOR_DARK_BLUE);
                        focused_button_counter = 4;
                    }
                    break;
                case Event::MouseButtonPressed:
                    switch (event.key.code) {
                        case Mouse::Left:
                            // If appropriate mouse position was captured:
                            // 1) Starting to play in the easy mode:
                            if (captured_button(window, mechanics))
                                game(1);
                            // 2) Starting to play in the normal mode:
                            else if (captured_button(window, STIC))
                                game(2);
                            // 3) Starting to play in the hard mode:
                            else if (captured_button(window, applied_maths))
                                game(3);
                            // 4) Go back:
                            else if (captured_button(window, back))
                                main_menu();
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
                                    mechanics.setFillColor(COLOR_YELLOW);
                                    mechanics.setOutlineColor(COLOR_DARK_BLUE);
                                    focused_button_counter++;
                                    break;
                                case 1:
                                    // if we have pressed Enter:
                                    if (event.key.code == Keyboard::Return)
                                        // execute mechanics button:
                                        game(1);

                                    // unfocus mechanics button:
                                    mechanics.setFillColor(COLOR_DARK_VIOLET);
                                    mechanics.setOutlineColor(COLOR_LIGHT_GREEN);
                                    // focus STIC button:
                                    STIC.setFillColor(COLOR_YELLOW);
                                    STIC.setOutlineColor(COLOR_DARK_BLUE);
                                    focused_button_counter++;
                                    break;
                                case 2:
                                    // if we have pressed Enter:
                                    if (event.key.code == Keyboard::Return)
                                        // execute STIC button:
                                        game(2);

                                    // unfocus STIC button:
                                    STIC.setFillColor(COLOR_DARK_VIOLET);
                                    STIC.setOutlineColor(COLOR_LIGHT_GREEN);
                                    // focus applied Maths button:
                                    applied_maths.setFillColor(COLOR_YELLOW);
                                    applied_maths.setOutlineColor(COLOR_DARK_BLUE);
                                    focused_button_counter++;
                                    break;
                                case 3:
                                    // if we have pressed Enter:
                                    if (event.key.code == Keyboard::Return)
                                        // execute applied Maths button:
                                        game(3);

                                    // unfocus applied Maths button:
                                    applied_maths.setFillColor(COLOR_DARK_VIOLET);
                                    applied_maths.setOutlineColor(COLOR_LIGHT_GREEN);
                                    // focus back button:
                                    back.setFillColor(COLOR_YELLOW);
                                    back.setOutlineColor(COLOR_DARK_BLUE);
                                    focused_button_counter++;
                                    break;
                                case 4:
                                    // if we have pressed Enter:
                                    if (event.key.code == Keyboard::Return)
                                        // execute back button:
                                        main_menu();

                                    // unfocus back button:
                                    back.setFillColor(COLOR_DARK_VIOLET);
                                    back.setOutlineColor(COLOR_LIGHT_GREEN);
                                    // focus mechanics button:
                                    mechanics.setFillColor(COLOR_YELLOW);
                                    mechanics.setOutlineColor(COLOR_DARK_BLUE);
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
                    
                    applied_maths.setCharacterSize(5 * button_size / 6);
                    applied_maths.setOutlineThickness(button_size / 6);
                    applied_maths.setPosition((window.getSize().x - applied_maths.getGlobalBounds().width) / 2, 
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

        // Draw a menu:
        window.draw(complexity_title);
        window.draw(mechanics);
        window.draw(STIC);
        window.draw(applied_maths);
        window.draw(back);
        window.display();
    }
};