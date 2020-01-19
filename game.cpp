#include "game.hpp"

void game(RenderWindow& window, const Sprite& background, const Font& font, const unsigned& complexity) {
    // create the game board: 
    Board *game_board = new Board(window);

    // Initialize to main menu button:
    Text to_main_menu;
    to_main_menu.setFont(font);
    to_main_menu.setString("Main menu");
    to_main_menu.setPosition(430.0f, 350.0f);
    to_main_menu.setCharacterSize(50); 
    to_main_menu.setFillColor(Color(242, 0, 0, 255));
    to_main_menu.setOutlineColor(Color(0, 0, 0, 255));
    to_main_menu.setOutlineThickness(10);
    to_main_menu.setStyle(Text::Bold);

    while (window.isOpen()) {
        Event event;

        while (window.pollEvent(event)) {
            switch (event.type) {
                // close window:  
                case Event::Closed:
                    window.close();
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
                            break;
                        case Keyboard::G:
                        case Keyboard::Up:
                            game_board->rotate(false);
                            break;
                        case Keyboard::H:
                            game_board->rotate(true);
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
        game_board->print_board(window, font);
        // draw to main menu button:
        window.draw(to_main_menu);
        // display what we have just drawed:
        window.display();

        // if we can't move down no more:
        if (!game_board->step_down()) {
            // check for the full lines:
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

void main_menu(RenderWindow& window, const Sprite& background, const Font& font) {
    // counter of the currently chosen button:
    unsigned focused_button_counter = 0;

    // Initialize new game button:
    Text new_game;
    new_game.setFont(font);
    new_game.setString("New Game");
    new_game.setPosition(window.getSize().x / 2 - 50.0f, 25.0f);
    new_game.setCharacterSize(50); 
    new_game.setFillColor(Color(242, 0, 0, 255));
    new_game.setOutlineColor(Color(0, 0, 0, 255));
    new_game.setOutlineThickness(10);
    new_game.setStyle(Text::Bold);

    // Initialize exit button:
    Text exit;
    exit.setFont(font);
    exit.setString("Exit");
    exit.setPosition(window.getSize().x / 2 - 50.0f, 100.0f);
    exit.setCharacterSize(50); 
    exit.setFillColor(Color(242, 0, 0, 255));
    exit.setOutlineColor(Color(0, 0, 0, 255));
    exit.setOutlineThickness(10);
    exit.setStyle(Text::Bold);

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
                    // If appropriate mouse position was captured:
                    if ((Mouse::getPosition(window).x - new_game.getGlobalBounds().left >= 0) &&
                        (Mouse::getPosition(window).y - new_game.getGlobalBounds().top >= 0) &&
                        (Mouse::getPosition(window).x - new_game.getGlobalBounds().left <= new_game.getGlobalBounds().width) &&
                        (Mouse::getPosition(window).y - new_game.getGlobalBounds().top <= new_game.getGlobalBounds().height)) {
                        // focus new game button:
                        new_game.setFillColor(Color(255, 195, 0, 255));
                        new_game.setOutlineColor(Color(8, 0, 93, 255));
                    } else if ((Mouse::getPosition(window).x - exit.getGlobalBounds().left >= 0) &&
                               (Mouse::getPosition(window).y - exit.getGlobalBounds().top >= 0) &&
                               (Mouse::getPosition(window).x - exit.getGlobalBounds().left <= exit.getGlobalBounds().width) &&
                               (Mouse::getPosition(window).y - exit.getGlobalBounds().top <= exit.getGlobalBounds().height)) {
                        // focus exit button:
                        exit.setFillColor(Color(255, 195, 0, 255));
                        exit.setOutlineColor(Color(8, 0, 93, 255));
                    } else {
                        // unfocus all the buttons:
                        new_game.setFillColor(Color(242, 0, 0, 255));
                        new_game.setOutlineColor(Color(0, 0, 0, 255));
                        exit.setFillColor(Color(242, 0, 0, 255));
                        exit.setOutlineColor(Color(0, 0, 0, 255));
                    }
                    break;
                case Event::MouseButtonPressed:
                    switch (event.key.code) {
                        case Mouse::Left:
                            // If appropriate mouse position was captured:
                            // 1) Starting to play:
                            if ((Mouse::getPosition(window).x - new_game.getGlobalBounds().left >= 0) &&
                                (Mouse::getPosition(window).y - new_game.getGlobalBounds().top >= 0) &&
                                (Mouse::getPosition(window).x - new_game.getGlobalBounds().left <= new_game.getGlobalBounds().width) &&
                                (Mouse::getPosition(window).y - new_game.getGlobalBounds().top <= new_game.getGlobalBounds().height))
                                complexity_menu(window, background, font);
                            // 2) Exit program:
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

                                    // focus new game button:
                                    new_game.setFillColor(Color(255, 195, 0, 255));
                                    new_game.setOutlineColor(Color(8, 0, 93, 255));
                                    focused_button_counter++;
                                    break;
                                case 1:
                                    // if we have pressed Enter:
                                    if (event.key.code == Keyboard::Return)
                                        // execute new game button:
                                        complexity_menu(window, background, font);

                                    // unfocus new game button:
                                    new_game.setFillColor(Color(242, 0, 0, 255));
                                    new_game.setOutlineColor(Color(0, 0, 0, 255));
                                    // focus exit button:
                                    exit.setFillColor(Color(255, 195, 0, 255));
                                    exit.setOutlineColor(Color(8, 0, 93, 255));
                                    focused_button_counter++;
                                    break;
                                case 2:
                                    // if we have pressed Enter:
                                    if (event.key.code == Keyboard::Return)
                                        // execute exit button:
                                        window.close();

                                    // unfocus exit button:
                                    exit.setFillColor(Color(242, 0, 0, 255));
                                    exit.setOutlineColor(Color(0, 0, 0, 255));
                                    // focus new game button:
                                    new_game.setFillColor(Color(255, 195, 0, 255));
                                    new_game.setOutlineColor(Color(8, 0, 93, 255));
                                    focused_button_counter = 1;
                                    break;
                                default:
                                    break;
                            }
                        default:
                            break;
                    }
                default:
                    break;
            }
        }

        // Clear window:
        window.clear();
        // Draw a menu:
        window.draw(background);
        window.draw(new_game);
        window.draw(exit);
        window.display();
    }    
}

void complexity_menu(RenderWindow& window, const Sprite& background, const Font& font) {
    // counter of the currently chosen button:
    unsigned focused_button_counter = 0;
    
    // Initialize complexity title:
    Text complexity_title;
    complexity_title.setFont(font);
    complexity_title.setString("Choose complexity");
    complexity_title.setPosition(window.getSize().x / 2 - 50.0f, 25.0f);
    complexity_title.setCharacterSize(50); 
    complexity_title.setFillColor(Color(242, 0, 0, 255));
    complexity_title.setOutlineColor(Color(0, 0, 0, 255));
    complexity_title.setStyle(Text::Bold);
    
    // Initialize mechanics button:
    Text mechanics;
    mechanics.setFont(font);
    mechanics.setString("Mechanics");
    mechanics.setPosition(window.getSize().x / 2 - 50.0f, 110.0f);
    mechanics.setCharacterSize(50); 
    mechanics.setFillColor(Color(242, 0, 0, 255));
    mechanics.setOutlineColor(Color(0, 0, 0, 255));
    mechanics.setOutlineThickness(10);
    mechanics.setStyle(Text::Bold);

    // Initialize STIC button:
    Text STIC;
    STIC.setFont(font);
    STIC.setString("STIC");
    STIC.setPosition(window.getSize().x / 2 - 50.0f, 185.0f);
    STIC.setCharacterSize(50); 
    STIC.setFillColor(Color(242, 0, 0, 255));
    STIC.setOutlineColor(Color(0, 0, 0, 255));
    STIC.setOutlineThickness(10);
    STIC.setStyle(Text::Bold);

    // Initialize applied mathematics button:
    Text applied_mathematics;
    applied_mathematics.setFont(font);
    applied_mathematics.setString("Applied Mathematics");
    applied_mathematics.setPosition(window.getSize().x / 2 - 50.0f, 260.0f);
    applied_mathematics.setCharacterSize(50); 
    applied_mathematics.setFillColor(Color(242, 0, 0, 255));
    applied_mathematics.setOutlineColor(Color(0, 0, 0, 255));
    applied_mathematics.setOutlineThickness(10);
    applied_mathematics.setStyle(Text::Bold);

    // Initialize back button:
    Text back;
    back.setFont(font);
    back.setString("Back");
    back.setPosition(window.getSize().x / 2 - 50.0f, 350.0f);
    back.setCharacterSize(50); 
    back.setFillColor(Color(242, 0, 0, 255));
    back.setOutlineColor(Color(0, 0, 0, 255));
    back.setOutlineThickness(10);
    back.setStyle(Text::Bold);

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
                    // If appropriate mouse position was captured:
                    if ((Mouse::getPosition(window).x - mechanics.getGlobalBounds().left >= 0) &&
                        (Mouse::getPosition(window).y - mechanics.getGlobalBounds().top >= 0) &&
                        (Mouse::getPosition(window).x - mechanics.getGlobalBounds().left <= mechanics.getGlobalBounds().width) &&
                        (Mouse::getPosition(window).y - mechanics.getGlobalBounds().top <= mechanics.getGlobalBounds().height)) {
                        // focus mechanics button:
                        mechanics.setFillColor(Color(255, 195, 0, 255));
                        mechanics.setOutlineColor(Color(8, 0, 93, 255));
                    } else if ((Mouse::getPosition(window).x - STIC.getGlobalBounds().left >= 0) &&
                               (Mouse::getPosition(window).y - STIC.getGlobalBounds().top >= 0) &&
                               (Mouse::getPosition(window).x - STIC.getGlobalBounds().left <= STIC.getGlobalBounds().width) &&
                               (Mouse::getPosition(window).y - STIC.getGlobalBounds().top <= STIC.getGlobalBounds().height)) {
                        // focus STIC button:
                        STIC.setFillColor(Color(255, 195, 0, 255));
                        STIC.setOutlineColor(Color(8, 0, 93, 255));
                    } else if ((Mouse::getPosition(window).x - applied_mathematics.getGlobalBounds().left >= 0) &&
                               (Mouse::getPosition(window).y - applied_mathematics.getGlobalBounds().top >= 0) &&
                               (Mouse::getPosition(window).x - applied_mathematics.getGlobalBounds().left <= applied_mathematics.getGlobalBounds().width) &&
                               (Mouse::getPosition(window).y - applied_mathematics.getGlobalBounds().top <= applied_mathematics.getGlobalBounds().height)) {
                        // focus applied mathematics button:
                        applied_mathematics.setFillColor(Color(255, 195, 0, 255));
                        applied_mathematics.setOutlineColor(Color(8, 0, 93, 255));                   
                    } else if ((Mouse::getPosition(window).x - back.getGlobalBounds().left >= 0) &&
                               (Mouse::getPosition(window).y - back.getGlobalBounds().top >= 0) &&
                               (Mouse::getPosition(window).x - back.getGlobalBounds().left <= back.getGlobalBounds().width) &&
                               (Mouse::getPosition(window).y - back.getGlobalBounds().top <= back.getGlobalBounds().height)) {
                        // focus back button:
                        back.setFillColor(Color(255, 195, 0, 255));
                        back.setOutlineColor(Color(8, 0, 93, 255));
                    } else {
                        // unfocus all the buttons:
                        mechanics.setFillColor(Color(242, 0, 0, 255));
                        mechanics.setOutlineColor(Color(0, 0, 0, 255));
                        STIC.setFillColor(Color(242, 0, 0, 255));
                        STIC.setOutlineColor(Color(0, 0, 0, 255));
                        applied_mathematics.setFillColor(Color(242, 0, 0, 255));
                        applied_mathematics.setOutlineColor(Color(0, 0, 0, 255));
                        back.setFillColor(Color(242, 0, 0, 255));
                        back.setOutlineColor(Color(0, 0, 0, 255));
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
                                    mechanics.setFillColor(Color(242, 0, 0, 255));
                                    mechanics.setOutlineColor(Color(0, 0, 0, 255));
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
                                    STIC.setFillColor(Color(242, 0, 0, 255));
                                    STIC.setOutlineColor(Color(0, 0, 0, 255));
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
                                    applied_mathematics.setFillColor(Color(242, 0, 0, 255));
                                    applied_mathematics.setOutlineColor(Color(0, 0, 0, 255));
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
                                    back.setFillColor(Color(242, 0, 0, 255));
                                    back.setOutlineColor(Color(0, 0, 0, 255));
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
                default:
                    break;
            }
        }

        // Clear window:
        window.clear();
        // Draw a menu:
        window.draw(background);
        window.draw(complexity_title);
        window.draw(mechanics);
        window.draw(STIC);
        window.draw(applied_mathematics);
        window.draw(back);
        window.display();
    }
};