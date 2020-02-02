#include <unistd.h>
#include "board.hpp"
#include "server.hpp"
#include "client.hpp"

#define DEF_COU_CHA_FIG 2 

enum state_figure {
    DESCEND_FIGURE,
    STOP_FIGURE,
    CHANGE_FIGURE
};

// new button initialization function (for code reduction):
Text create_button(const Font& font, const string title, const double& button_size, Vector2f position, const bool& outline = true, const unsigned& center_coefficient = 2);

// captured button identifier:
bool captured_button(RenderWindow& window, Text& button);

void game(RenderWindow& window, Sprite& background, const Font& font, const unsigned& complexity);

void game_over_menu(RenderWindow& window, Sprite& background, Board* game_board, const Font& font);

void pause_menu(RenderWindow& window, Sprite& background, Board* game_board, const Font& font);

void main_menu(RenderWindow& window, Sprite& background, const Font& font);

void multiplayer_menu(RenderWindow& window, Sprite& background, const Font& font);

// Set up and create a new session as a server:
void create_session(RenderWindow& window, Sprite& background, const Font& font);

// Manage just created session as a server:
void session_menu(RenderWindow& window, Sprite& background, const Font& font, Server* current_session, Client* current_client);

// Find servers function:
void find_servers(RenderWindow& window, Sprite& background, const Font& font);

void complexity_menu(RenderWindow& window, Sprite& background, const Font& font);