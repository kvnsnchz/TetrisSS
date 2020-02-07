#ifndef gameHPP
#define gameHPP

#include <unistd.h>
#include "server.hpp"
#include "client.hpp"

#define DEF_COU_CHA_FIG 2 
#define MAX_SESSION_NAME_LENGTH 12

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
        Menu(const Sprite& new_background, const Font& new_font): background(new_background), font(new_font) {
            RenderWindow window();
        };
        
        void game(const unsigned& complexity);

        void game_over_menu(Board* game_board);

        void pause_menu(Board* game_board);

        void main_menu(const bool& initialization = false);

        void multiplayer_menu();

        // Set up and create a new session as a server:
        void create_session();

        // Manage just created session as a server:
        void session_menu(Server* current_session, Client* current_client);

        // Find servers function:
        void find_servers();

        void complexity_menu();

        ~Menu() {};
};

#endif