#include <unistd.h>
#include "board.hpp"

#define DEF_COU_CHA_FIG 2 

enum state_figure {
    DESCEND_FIGURE,
    STOP_FIGURE,
    CHANGE_FIGURE
};

// new button initialization function (for code reduction):
Text create_button(const Font& font, const char* title, const double& button_size, Vector2f position, const bool& outline = true, const bool& center = true);

// captured button identifier:
bool captured_button(RenderWindow& window, Text& button);

void game(RenderWindow& window, Sprite& background, const Font& font, const unsigned& complexity);

void game_over_menu(RenderWindow& window, Sprite& background, Board* game_board, const Font& font);

void pause_menu(RenderWindow& window, Sprite& background, Board* game_board, const Font& font);

void main_menu(RenderWindow& window, Sprite& background, const Font& font);

void complexity_menu(RenderWindow& window, Sprite& background, const Font& font);