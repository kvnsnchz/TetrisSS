#ifndef menuHPP
#define menuHPP

#include <unistd.h>
#include "server.hpp"
#include "client.hpp"
#include "board.hpp"

#define DEF_COU_CHA_FIG 1 
#define MAX_NICKNAME_LENGTH 12
#define LEVEL_MECHANICS 1
#define LEVEL_STIC 3
#define LEVEL_APP_MATHS 5
#define LEVEL_MAX 30
#define LEVEL_MULTIPLICATOR 5
#define LINES_TO_LEVEL_UP 2
#define GLOBAL_GAME_OVER_TICKS 6
#define DESCEND_SLEEP_SECONDS 0.005f

/**
 * @brief New button initialization function (for code reduction).
 * 
 * @param font 
 * @param title 
 * @param button_size 
 * @param position 
 * @param outline 
 * @param center_coefficient 
 * @return Text 
 */
Text create_button(Font& font, const string title, const double& button_size,
    Vector2f position, const bool& outline = true, const unsigned& center_coefficient = 2);

/**
 * @brief Captured button identifier:
 * compares current user's mouse position with 
 * the local position and global bounds of the button.
 * 
 * @param window 
 * @param button 
 * @return true 
 * @return false 
 */
bool captured_button(RenderWindow& window, Text& button);

/**
 * @brief Menu class to initialize desktop hierarchy,
 * distinguish and simplify the logical connection 
 * between different menus of the program.
 * 
 */
class Menu {
    private:
        /**
         * @brief The main sfml window of the program.
         * 
         */
        RenderWindow window;
        /**
         * @brief Background sprite of the window filled with some image.
         * 
         */
        Sprite background;
        /**
         * @brief Font that is used for all the text output of a program.
         * 
         */
        Font font;

        friend Text create_button(Font& font, const string title, const double& button_size,
            Vector2f position, const bool& outline, const unsigned& center_coefficient);

        friend bool captured_button(RenderWindow& window, Text& button);
    public:
        Menu(const Sprite& new_background, const Font& new_font): 
            background(new_background), font(new_font) {
            RenderWindow window();
        };
        
        /**
         * @brief Current function - the singleplayer game itself. 
         * Is dedicated to draw a player's board with the next figure, 
         * current score, level and descending figures on a board 
         * with certain delay and possibilities to be moved and rotated.
         * 
         * @param initial_complexity 
         */
        void game(const unsigned& increment_complexity);

        /**
         * @brief Menu after reaching game over condition by user 
         * in the singleplayer game with possibilities to restart a game, 
         * choose another initial complexity, return to main menu or quit the program.
         * 
         * @param game_board 
         * @param initial_complexity 
         */
        void game_over_menu(Board* game_board, const unsigned& initial_complexity);

        /**
         * @brief Function that provides pause during singleplayer game
         * with its own corresponding menu with possibilities to resume,
         * restart a game, return to main menu or quit the program.
         * 
         * @param game_board 
         * @param initial_complexity 
         */
        void pause_menu(Board* game_board, const unsigned& initial_complexity);

        // Multiplayer game function:
        /**
         * @brief Multiplayer game function. Draws the current player's board with the next figure, 
         * current score, level and descending figures on a board 
         * with certain delay and possibilities to be moved and rotated.
         * Also displays the boards, scores and levels
         * of other players in the session with their simultaneuos update. 
         * 
         * @param current_session 
         * @param current_client 
         */
        void multiplayer_game(Server* current_session, Client* current_client);

        /**
         * @brief The program's main menu, the first one the user interact with.
         * Allows to choose either singleplayer or multiplayer menu or quit the program.
         * 
         * @param initialization 
         */
        void main_menu(const bool& initialization = false);

        /**
         * @brief Multiplayer game menu, where the player is able to enter his nickname,
         * which will be used in further sessions, go to the menu of the new session creation
         * or to the menu of other sessions search menu. 
         * 
         * @param initial_nickname 
         */
        void multiplayer_menu(const string& initial_nickname = "");

        /**
         * @brief Provides pause during multiplayer game
         * with its own corresponding menu with pibilities to resume,
         * disconnect from the current session or quit the program.
         * 
         * @param game_board 
         * @param other_boards 
         * @param current_session 
         * @param current_client 
         * @param status 
         */
        void multiplayer_pause_menu(Board* game_board, vector<Board*> other_boards, Server* current_session, Client* current_client, request_status& status);

        /**
         * @brief New session creation menu, where user is obliged
         * to enter the maximal number of players and choose the initial game complexity.
         * Afterwards, player is able to create the session with chosen parameters
         * and go to the session menu. There is also a pssibility to return to multiplayer menu.
         * 
         * @param nickname 
         */
        void create_session(const string& nickname);

        /**
         * @brief Current fuction displays the list of players in the current session
         * with their nicknames and overyone's readiness to start a game.
         * Each player can either change his readiness or disconnect. 
         * Server is able to start a new game once all the players are ready.
         * 
         * @param current_session 
         * @param current_client 
         */
        void session_menu(Server* current_session, Client* current_client);

        // Find servers function:
        /**
         * @brief The list of currently available sessions,
         * their initial complexities and fullnesses.
         * There are the possibilities to go back
         * to the multiplayer menu or to connect to the chosen session.
         * 
         * @param nickname 
         */
        void find_servers(const string& nickname);

        /**
         * @brief Menu to choose the initial complexity of the singleplayer game.
         * There are 3 choises available:
         * - Mechanics (level 1),
         * - STIC (level 3),
         * - Applied Maths (level 5).
         * It is also possible to return to main menu.
         * 
         */
        void complexity_menu();

        ~Menu() {};
};

#endif