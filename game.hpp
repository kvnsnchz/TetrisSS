#include <unistd.h>
#include "board.hpp"

#define DEF_COU_CHA_FIG 2 

enum state_figure {
    DESCEND_FIGURE,
    STOP_FIGURE,
    CHANGE_FIGURE
};

void game(RenderWindow& window, Sprite& background, const Font& font, const unsigned& complexity);

void main_menu(RenderWindow& window, Sprite& background, const Font& font);

void complexity_menu(RenderWindow& window, Sprite& background, const Font& font);