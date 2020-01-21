#include "game.hpp"

void game(RenderWindow& window, Sprite& background, const Font& font, const unsigned& complexity) {
    // counter of the currently chosen button:
    unsigned focused_button_counter = 0;
    // button size:
    double button_size = min(60.0f, 3.5f * (window.getSize().x - 425.0f) / 10);

    // initialize cell size according the current window size:
    Vector2f cell_size(min(min(40.0f, (float) (window.getSize().x - 3.5 * button_size / 10 - 24.0f) / 10), min(40.0f, ((float) window.getSize().y - 29.0f) / 20)),
                    min(min(40.0f, (float) (window.getSize().x - 3.5 * button_size / 10 - 24.0f) / 10), min(40.0f, ((float) window.getSize().y - 29.0f) / 20)));
    // create the game board: 
    Board *game_board = new Board(window, cell_size);

    // Initialize to main menu button:
    Text to_main_menu;
    to_main_menu.setFont(font);
    to_main_menu.setString("Main menu");
    to_main_menu.setCharacterSize(5 * button_size / 6);
    to_main_menu.setOutlineThickness(button_size / 6);
    to_main_menu.setStyle(Text::Bold);
    to_main_menu.setFillColor(Color(144, 12, 63, 255));
    to_main_menu.setOutlineColor(Color(218, 247, 166, 255));
    to_main_menu.setPosition((cell_size.x + 1) * 10 + 19.0f, 20 * button_size / 6 + 125.0f);
    
    state_figure _state_figure = DESCEND_FIGURE;
    int count_change_figure = DEF_COU_CHA_FIG;

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
                    to_main_menu.setFillColor(Color(144, 12, 63, 255));
                    to_main_menu.setOutlineColor(Color(218, 247, 166, 255));

                    // If appropriate mouse position was captured:
                    if ((Mouse::getPosition(window).x - to_main_menu.getGlobalBounds().left >= 0) &&
                        (Mouse::getPosition(window).y - to_main_menu.getGlobalBounds().top >= 0) &&
                        (Mouse::getPosition(window).x - to_main_menu.getGlobalBounds().left <= to_main_menu.getGlobalBounds().width) &&
                        (Mouse::getPosition(window).y - to_main_menu.getGlobalBounds().top <= to_main_menu.getGlobalBounds().height)) {
                        // focus new single game button:
                        to_main_menu.setFillColor(Color(255, 195, 0, 255));
                        to_main_menu.setOutlineColor(Color(8, 0, 93, 255));
                        focused_button_counter = 1;
                    }
                    break;
                case Event::MouseButtonPressed:
                    switch (event.key.code) {
                        case Mouse::Left:
                            // If appropriate mouse position was captured:
                            // 1) to_main_menu game:
                            if ((Mouse::getPosition(window).x - to_main_menu.getGlobalBounds().left <= to_main_menu.getGlobalBounds().width) &&
                                (Mouse::getPosition(window).y - to_main_menu.getGlobalBounds().top <= to_main_menu.getGlobalBounds().height))
                                main_menu(window, background, font);
                            break;
                        default:
                            break;
                    }
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
                                    to_main_menu.setFillColor(Color(255, 195, 0, 255));
                                    to_main_menu.setOutlineColor(Color(8, 0, 93, 255));
                                    focused_button_counter++;
                                    break;
                                case 1:
                                    // if we have pressed Enter:
                                    if (event.key.code == Keyboard::Return)
                                        // execute to_main_menu button:
                                        main_menu(window, background, font);

                                    // unfocus main menu button:
                                    to_main_menu.setFillColor(Color(144, 12, 63, 255));
                                    to_main_menu.setOutlineColor(Color(218, 247, 166, 255));
                                    focused_button_counter = 0;
                                    break;
                                default:
                                    break;
                            }
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
                // if we have changed window's size:
                case Event::Resized:
                    // set minimal window size:
                    if ((window.getSize().y < 600) || (window.getSize().x < 400))
                        window.setSize(Vector2u(400, 600));
                        
                    // button size:
                    button_size = min(60.0f, 3.5f * (window.getSize().x - 425.0f) / 10);
                    
                    // update view:
                    window.setView(View(FloatRect(0.0f, 0.0f, (float) window.getSize().x, (float) window.getSize().y)));

                    // update cell size:
                    cell_size.x = min(min(40.0f, (float) (window.getSize().x - 3.5 * button_size / 10 - 24.0f) / 10), min(40.0f, ((float) window.getSize().y - 29.0f) / 20));
                    cell_size.y = min(min(40.0f, (float) (window.getSize().x - 3.5 * button_size / 10 - 24.0f) / 10), min(40.0f, ((float) window.getSize().y - 29.0f) / 20));
                    game_board->set_cell_size(cell_size);

                    // update all the buttons and their positions:
                    to_main_menu.setCharacterSize(5 * button_size / 6);
                    to_main_menu.setOutlineThickness(button_size / 6);
                    to_main_menu.setPosition((cell_size.x + 1) * 10 + 19.0f, 4 * button_size + 125.0f);
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
        window.setView(View(FloatRect(0.0f, 0.0f, (float) window.getSize().x, (float) window.getSize().y)));

        // draw a board:
        game_board->print_board(window, font, 5 * button_size / 6);
        // draw to main menu button:
        window.draw(to_main_menu);
        // display what we have just drawed:
        window.display();
        
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
            game_board->insert_figure_current();
            game_board->erase_lines(complexity);
                    
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

        // set delay according to the complexity:
        usleep(300000 / complexity);
    }

    delete game_board;
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
    Text singleplayer;
    singleplayer.setFont(font);
    singleplayer.setString("Singleplayer");
    singleplayer.setCharacterSize(5 * button_size / 6);
    singleplayer.setOutlineThickness(button_size / 6);
    singleplayer.setStyle(Text::Bold); 
    singleplayer.setFillColor(Color(144, 12, 63, 255));
    singleplayer.setOutlineColor(Color(218, 247, 166, 255));
    singleplayer.setPosition((window.getSize().x - singleplayer.getGlobalBounds().width) / 2, 
                            (window.getSize().y - number_of_buttons * (button_size + 15) - 15) / 2);

    // Initialize new multiplayer game button:
    Text multiplayer;
    multiplayer.setFont(font);
    multiplayer.setString("Multiplayer");
    multiplayer.setCharacterSize(5 * button_size / 6);
    multiplayer.setOutlineThickness(button_size / 6);
    multiplayer.setStyle(Text::Bold); 
    multiplayer.setFillColor(Color(144, 12, 63, 255));
    multiplayer.setOutlineColor(Color(218, 247, 166, 255));
    multiplayer.setPosition((window.getSize().x - multiplayer.getGlobalBounds().width) / 2, 
                            (window.getSize().y - number_of_buttons * (button_size + 15) - 15) / 2 + button_size + 15);

    // Initialize leaderboard button:
    Text leaderboard;
    leaderboard.setFont(font);
    leaderboard.setString("Leaderboard");
    leaderboard.setCharacterSize(5 * button_size / 6);
    leaderboard.setOutlineThickness(button_size / 6);
    leaderboard.setStyle(Text::Bold);
    leaderboard.setFillColor(Color(144, 12, 63, 255));
    leaderboard.setOutlineColor(Color(218, 247, 166, 255));
    leaderboard.setPosition((window.getSize().x - leaderboard.getGlobalBounds().width) / 2, 
                            (window.getSize().y - number_of_buttons * (button_size + 15) - 15) / 2 + (button_size + 15) * 2);

    // Initialize settings button:
    Text settings;
    settings.setFont(font);
    settings.setString("Settings");
    settings.setCharacterSize(5 * button_size / 6);
    settings.setOutlineThickness(button_size / 6);
    settings.setStyle(Text::Bold);
    settings.setFillColor(Color(144, 12, 63, 255));
    settings.setOutlineColor(Color(218, 247, 166, 255));
    settings.setPosition((window.getSize().x - settings.getGlobalBounds().width) / 2, 
                            (window.getSize().y - number_of_buttons * (button_size + 15) - 15) / 2 + (button_size + 15) * 3);

    // Initialize authors button:
    Text authors;
    authors.setFont(font);
    authors.setString("Authors");
    authors.setCharacterSize(5 * button_size / 6);
    authors.setOutlineThickness(button_size / 6);
    authors.setStyle(Text::Bold);
    authors.setFillColor(Color(144, 12, 63, 255));
    authors.setOutlineColor(Color(218, 247, 166, 255));
    authors.setPosition((window.getSize().x - authors.getGlobalBounds().width) / 2, 
                            (window.getSize().y - number_of_buttons * (button_size + 15) - 15) / 2 + (button_size + 15) * 4);
    

    // Initialize exit button:
    Text exit;
    exit.setFont(font);
    exit.setString("Exit");
    exit.setCharacterSize(5 * button_size / 6);
    exit.setOutlineThickness(button_size / 6);
    exit.setStyle(Text::Bold);
    exit.setFillColor(Color(144, 12, 63, 255));
    exit.setOutlineColor(Color(218, 247, 166, 255));
    exit.setPosition((window.getSize().x - exit.getGlobalBounds().width) / 2, 
                            (window.getSize().y - number_of_buttons * (button_size + 15) - 15) / 2 + (button_size + 15) * 5);

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
                    if ((Mouse::getPosition(window).x - singleplayer.getGlobalBounds().left >= 0) &&
                        (Mouse::getPosition(window).y - singleplayer.getGlobalBounds().top >= 0) &&
                        (Mouse::getPosition(window).x - singleplayer.getGlobalBounds().left <= singleplayer.getGlobalBounds().width) &&
                        (Mouse::getPosition(window).y - singleplayer.getGlobalBounds().top <= singleplayer.getGlobalBounds().height)) {
                        // focus new single game button:
                        singleplayer.setFillColor(Color(255, 195, 0, 255));
                        singleplayer.setOutlineColor(Color(8, 0, 93, 255));
                        focused_button_counter = 1;
                    } else if ((Mouse::getPosition(window).x - multiplayer.getGlobalBounds().left >= 0) &&
                               (Mouse::getPosition(window).y - multiplayer.getGlobalBounds().top >= 0) &&
                               (Mouse::getPosition(window).x - multiplayer.getGlobalBounds().left <= multiplayer.getGlobalBounds().width) &&
                               (Mouse::getPosition(window).y - multiplayer.getGlobalBounds().top <= multiplayer.getGlobalBounds().height)) {
                        // focus new multiplayer game button:
                        multiplayer.setFillColor(Color(255, 195, 0, 255));
                        multiplayer.setOutlineColor(Color(8, 0, 93, 255));
                        focused_button_counter = 2;
                    } else if ((Mouse::getPosition(window).x - leaderboard.getGlobalBounds().left >= 0) &&
                               (Mouse::getPosition(window).y - leaderboard.getGlobalBounds().top >= 0) &&
                               (Mouse::getPosition(window).x - leaderboard.getGlobalBounds().left <= leaderboard.getGlobalBounds().width) &&
                               (Mouse::getPosition(window).y - leaderboard.getGlobalBounds().top <= leaderboard.getGlobalBounds().height)) {
                        // focus leaderboard button:
                        leaderboard.setFillColor(Color(255, 195, 0, 255));
                        leaderboard.setOutlineColor(Color(8, 0, 93, 255));
                        focused_button_counter = 3;
                    } else if ((Mouse::getPosition(window).x - settings.getGlobalBounds().left >= 0) &&
                               (Mouse::getPosition(window).y - settings.getGlobalBounds().top >= 0) &&
                               (Mouse::getPosition(window).x - settings.getGlobalBounds().left <= settings.getGlobalBounds().width) &&
                               (Mouse::getPosition(window).y - settings.getGlobalBounds().top <= settings.getGlobalBounds().height)) {
                        // focus settings button:
                        settings.setFillColor(Color(255, 195, 0, 255));
                        settings.setOutlineColor(Color(8, 0, 93, 255));
                        focused_button_counter = 4;
                    } else if ((Mouse::getPosition(window).x - authors.getGlobalBounds().left >= 0) &&
                               (Mouse::getPosition(window).y - authors.getGlobalBounds().top >= 0) &&
                               (Mouse::getPosition(window).x - authors.getGlobalBounds().left <= authors.getGlobalBounds().width) &&
                               (Mouse::getPosition(window).y - authors.getGlobalBounds().top <= authors.getGlobalBounds().height)) {
                        // focus authors button:
                        authors.setFillColor(Color(255, 195, 0, 255));
                        authors.setOutlineColor(Color(8, 0, 93, 255));
                        focused_button_counter = 5;
                    } else if ((Mouse::getPosition(window).x - exit.getGlobalBounds().left >= 0) &&
                               (Mouse::getPosition(window).y - exit.getGlobalBounds().top >= 0) &&
                               (Mouse::getPosition(window).x - exit.getGlobalBounds().left <= exit.getGlobalBounds().width) &&
                               (Mouse::getPosition(window).y - exit.getGlobalBounds().top <= exit.getGlobalBounds().height)) {
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
                            if ((Mouse::getPosition(window).x - singleplayer.getGlobalBounds().left >= 0) &&
                                (Mouse::getPosition(window).y - singleplayer.getGlobalBounds().top >= 0) &&
                                (Mouse::getPosition(window).x - singleplayer.getGlobalBounds().left <= singleplayer.getGlobalBounds().width) &&
                                (Mouse::getPosition(window).y - singleplayer.getGlobalBounds().top <= singleplayer.getGlobalBounds().height))
                                complexity_menu(window, background, font);
                            // 2) Start new multiplayer game:
                            else if ((Mouse::getPosition(window).x - multiplayer.getGlobalBounds().left >= 0) &&
                                     (Mouse::getPosition(window).y - multiplayer.getGlobalBounds().top >= 0) &&
                                     (Mouse::getPosition(window).x - multiplayer.getGlobalBounds().left <= multiplayer.getGlobalBounds().width) &&
                                     (Mouse::getPosition(window).y - multiplayer.getGlobalBounds().top <= multiplayer.getGlobalBounds().height))
                                // update view:
                                window.setView(View(FloatRect(0.0f, 0.0f, (float) window.getSize().x, (float) window.getSize().y)));
                            // 3) Check highscores:
                            else if ((Mouse::getPosition(window).x - leaderboard.getGlobalBounds().left >= 0) &&
                                     (Mouse::getPosition(window).y - leaderboard.getGlobalBounds().top >= 0) &&
                                     (Mouse::getPosition(window).x - leaderboard.getGlobalBounds().left <= leaderboard.getGlobalBounds().width) &&
                                     (Mouse::getPosition(window).y - leaderboard.getGlobalBounds().top <= leaderboard.getGlobalBounds().height))
                                // update view:
                                window.setView(View(FloatRect(0.0f, 0.0f, (float) window.getSize().x, (float) window.getSize().y)));
                            // 4) Go to settings:
                            else if ((Mouse::getPosition(window).x - settings.getGlobalBounds().left >= 0) &&
                                     (Mouse::getPosition(window).y - settings.getGlobalBounds().top >= 0) &&
                                     (Mouse::getPosition(window).x - settings.getGlobalBounds().left <= settings.getGlobalBounds().width) &&
                                     (Mouse::getPosition(window).y - settings.getGlobalBounds().top <= settings.getGlobalBounds().height))
                                // update view:
                                window.setView(View(FloatRect(0.0f, 0.0f, (float) window.getSize().x, (float) window.getSize().y)));
                            // 5) Who wrote this masterpiece?
                            else if ((Mouse::getPosition(window).x - authors.getGlobalBounds().left >= 0) &&
                                     (Mouse::getPosition(window).y - authors.getGlobalBounds().top >= 0) &&
                                     (Mouse::getPosition(window).x - authors.getGlobalBounds().left <= authors.getGlobalBounds().width) &&
                                     (Mouse::getPosition(window).y - authors.getGlobalBounds().top <= authors.getGlobalBounds().height))
                                // update view:
                                window.setView(View(FloatRect(0.0f, 0.0f, (float) window.getSize().x, (float) window.getSize().y)));
                            // 6) Exit program:
                            else if ((Mouse::getPosition(window).x - exit.getGlobalBounds().left >= 0) &&
                                     (Mouse::getPosition(window).y - exit.getGlobalBounds().top >= 0) &&
                                     (Mouse::getPosition(window).x - exit.getGlobalBounds().left <= exit.getGlobalBounds().width) &&
                                     (Mouse::getPosition(window).y - exit.getGlobalBounds().top <= exit.getGlobalBounds().height))
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
                                        // execute multiplayer button:
                                        break;

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
        window.setView(View(FloatRect(0.0f, 0.0f, (float) window.getSize().x, (float) window.getSize().y)));

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
    Text complexity_title;
    complexity_title.setFont(font);
    complexity_title.setString("Choose complexity");
    complexity_title.setCharacterSize(5 * button_size / 6);
    complexity_title.setStyle(Text::Bold);
    complexity_title.setFillColor(Color(144, 12, 63, 255));
    complexity_title.setOutlineColor(Color(218, 247, 166, 255));
    complexity_title.setPosition((window.getSize().x - complexity_title.getGlobalBounds().width) / 2, 
                            (window.getSize().y - number_of_buttons * (button_size + 15) - 25) / 2);
    
    // Initialize mechanics button:
    Text mechanics;
    mechanics.setFont(font);
    mechanics.setString("Mechanics");
    mechanics.setCharacterSize(5 * button_size / 6);
    mechanics.setOutlineThickness(button_size / 6);
    mechanics.setStyle(Text::Bold);
    mechanics.setFillColor(Color(144, 12, 63, 255));
    mechanics.setOutlineColor(Color(218, 247, 166, 255));
    mechanics.setPosition((window.getSize().x - mechanics.getGlobalBounds().width) / 2, 
                            (window.getSize().y - number_of_buttons * (button_size + 15) - 25) / 2 + button_size + 20.0f);

    // Initialize STIC button:
    Text STIC;
    STIC.setFont(font);
    STIC.setString("STIC");
    STIC.setCharacterSize(5 * button_size / 6);
    STIC.setOutlineThickness(button_size / 6);
    STIC.setStyle(Text::Bold); 
    STIC.setFillColor(Color(144, 12, 63, 255));
    STIC.setOutlineColor(Color(218, 247, 166, 255));
    STIC.setPosition((window.getSize().x - STIC.getGlobalBounds().width) / 2, 
                            (window.getSize().y - number_of_buttons * (button_size + 15) - 25) / 2 + (button_size + 15.0f) * 2 + 5.0f);

    // Initialize applied mathematics button:
    Text applied_mathematics;
    applied_mathematics.setFont(font);
    applied_mathematics.setString("Applied Mathematics");
    applied_mathematics.setCharacterSize(5 * button_size / 6);
    applied_mathematics.setOutlineThickness(button_size / 6);
    applied_mathematics.setStyle(Text::Bold);
    applied_mathematics.setFillColor(Color(144, 12, 63, 255));
    applied_mathematics.setOutlineColor(Color(218, 247, 166, 255));
    applied_mathematics.setPosition((window.getSize().x - applied_mathematics.getGlobalBounds().width) / 2, 
                            (window.getSize().y - number_of_buttons * (button_size + 15) - 25) / 2 + (button_size + 15.0f) * 3 + 5.0f);

    // Initialize back button:
    Text back;
    back.setFont(font);
    back.setString("Back");
    back.setCharacterSize(5 * button_size / 6);
    back.setOutlineThickness(button_size / 6);
    back.setStyle(Text::Bold);
    back.setFillColor(Color(144, 12, 63, 255));
    back.setOutlineColor(Color(218, 247, 166, 255));
    back.setPosition((window.getSize().x - back.getGlobalBounds().width) / 2, 
                            (window.getSize().y - number_of_buttons * (button_size + 15) - 25) / 2 + (button_size + 15.0f) * 4 + 10.0f);

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
                    if ((Mouse::getPosition(window).x - mechanics.getGlobalBounds().left >= 0) &&
                        (Mouse::getPosition(window).y - mechanics.getGlobalBounds().top >= 0) &&
                        (Mouse::getPosition(window).x - mechanics.getGlobalBounds().left <= mechanics.getGlobalBounds().width) &&
                        (Mouse::getPosition(window).y - mechanics.getGlobalBounds().top <= mechanics.getGlobalBounds().height)) {
                        // focus mechanics button:
                        mechanics.setFillColor(Color(255, 195, 0, 255));
                        mechanics.setOutlineColor(Color(8, 0, 93, 255));
                        focused_button_counter = 1;
                    } else if ((Mouse::getPosition(window).x - STIC.getGlobalBounds().left >= 0) &&
                               (Mouse::getPosition(window).y - STIC.getGlobalBounds().top >= 0) &&
                               (Mouse::getPosition(window).x - STIC.getGlobalBounds().left <= STIC.getGlobalBounds().width) &&
                               (Mouse::getPosition(window).y - STIC.getGlobalBounds().top <= STIC.getGlobalBounds().height)) {
                        // focus STIC button:
                        STIC.setFillColor(Color(255, 195, 0, 255));
                        STIC.setOutlineColor(Color(8, 0, 93, 255));
                        focused_button_counter = 2;
                    } else if ((Mouse::getPosition(window).x - applied_mathematics.getGlobalBounds().left >= 0) &&
                               (Mouse::getPosition(window).y - applied_mathematics.getGlobalBounds().top >= 0) &&
                               (Mouse::getPosition(window).x - applied_mathematics.getGlobalBounds().left <= applied_mathematics.getGlobalBounds().width) &&
                               (Mouse::getPosition(window).y - applied_mathematics.getGlobalBounds().top <= applied_mathematics.getGlobalBounds().height)) {
                        // focus applied mathematics button:
                        applied_mathematics.setFillColor(Color(255, 195, 0, 255));
                        applied_mathematics.setOutlineColor(Color(8, 0, 93, 255));
                        focused_button_counter = 3;             
                    } else if ((Mouse::getPosition(window).x - back.getGlobalBounds().left >= 0) &&
                               (Mouse::getPosition(window).y - back.getGlobalBounds().top >= 0) &&
                               (Mouse::getPosition(window).x - back.getGlobalBounds().left <= back.getGlobalBounds().width) &&
                               (Mouse::getPosition(window).y - back.getGlobalBounds().top <= back.getGlobalBounds().height)) {
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
                            if ((Mouse::getPosition(window).x - mechanics.getGlobalBounds().left >= 0) &&
                                (Mouse::getPosition(window).y - mechanics.getGlobalBounds().top >= 0) &&
                                (Mouse::getPosition(window).x - mechanics.getGlobalBounds().left <= mechanics.getGlobalBounds().width) &&
                                (Mouse::getPosition(window).y - mechanics.getGlobalBounds().top <= mechanics.getGlobalBounds().height))
                                game(window, background, font, 1);
                            // 2) Starting to play in the normal mode:
                            else if ((Mouse::getPosition(window).x - STIC.getGlobalBounds().left >= 0) &&
                                     (Mouse::getPosition(window).y - STIC.getGlobalBounds().top >= 0) &&
                                     (Mouse::getPosition(window).x - STIC.getGlobalBounds().left <= STIC.getGlobalBounds().width) &&
                                     (Mouse::getPosition(window).y - STIC.getGlobalBounds().top <= STIC.getGlobalBounds().height))
                                game(window, background, font, 2);
                            // 3) Starting to play in the hard mode:
                            else if ((Mouse::getPosition(window).x - applied_mathematics.getGlobalBounds().left >= 0) &&
                                     (Mouse::getPosition(window).y - applied_mathematics.getGlobalBounds().top >= 0) &&
                                     (Mouse::getPosition(window).x - applied_mathematics.getGlobalBounds().left <= applied_mathematics.getGlobalBounds().width) &&
                                     (Mouse::getPosition(window).y - applied_mathematics.getGlobalBounds().top <= applied_mathematics.getGlobalBounds().height))
                                game(window, background, font, 3);
                            // 4) Go back:
                            else if ((Mouse::getPosition(window).x - back.getGlobalBounds().left >= 0) &&
                                     (Mouse::getPosition(window).y - back.getGlobalBounds().top >= 0) &&
                                     (Mouse::getPosition(window).x - back.getGlobalBounds().left <= back.getGlobalBounds().width) &&
                                     (Mouse::getPosition(window).y - back.getGlobalBounds().top <= back.getGlobalBounds().height))
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
                    complexity_title.setPosition((window.getSize().x - complexity_title.getGlobalBounds().width) / 2, 
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
        window.setView(View(FloatRect(0.0f, 0.0f, (float) window.getSize().x, (float) window.getSize().y)));

        // Draw a menu:
        window.draw(complexity_title);
        window.draw(mechanics);
        window.draw(STIC);
        window.draw(applied_mathematics);
        window.draw(back);
        window.display();
    }
};