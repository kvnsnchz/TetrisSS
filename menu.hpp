#ifndef menuHPP
#define menuHPP

#include <unistd.h>
#include "server.hpp"
#include "client.hpp"

#define DEF_COU_CHA_FIG 1 
#define MAX_NICKNAME_LENGTH 12
#define LEVEL_MECHANICS 1
#define LEVEL_STIC 3
#define LEVEL_APP_MATHS 5
#define LEVEL_MAX 30
#define LINES_TO_LEVEL_UP 1

// new button initialization function (for code reduction):
Text create_button(Font& font, const string title, const double& button_size,
    Vector2f position, const bool& outline = true, const unsigned& center_coefficient = 2);

// captured button identifier:
bool captured_button(RenderWindow& window, Text& button);

// Menu class to initialize desktop hierarchy:
class Menu {
    private:
        RenderWindow window;
        Sprite background;
        Font font;

        friend Text create_button(Font& font, const string title, const double& button_size,
            Vector2f position, const bool& outline, const unsigned& center_coefficient);

        friend bool captured_button(RenderWindow& window, Text& button);
    public:
        Menu(const Sprite& new_background, const Font& new_font): 
            background(new_background), font(new_font) 
        {
            RenderWindow window();
        };
        
        // Function to play:
        void game(unsigned complexity);

        void game_over_menu(Board* game_board);

        // Pause menu:
        void pause_menu(Board* game_board);

        // Multiplayer game function:
        void multiplayer_game(Server* current_session, Client* current_client);

        void main_menu(const bool& initialization = false);

        // Menu for multiplayer:
        void multiplayer_menu();

        // Multiplayer pause menu:
        void multiplayer_pause_menu(Board* game_board, vector<Board*> other_boards, Server* current_session, Client* current_client);

        // Set up and create a new session as a server:
        void create_session(const string& nickname);

        // Manage just created session as a server:
        void session_menu(Server* current_session, Client* current_client);

        // Find servers function:
        void find_servers(const string& nickname);

        void complexity_menu();

        ~Menu() {};
};

#endif