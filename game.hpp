#include <unistd.h>
#include "board.hpp"

void game(RenderWindow& window, Sprite& background, const Font& font, const unsigned& complexity);

void game_over_menu(RenderWindow& window, Sprite& background, Board* game_board, const Font& font);

void pause_menu();

void main_menu(RenderWindow& window, Sprite& background, const Font& font);

void complexity_menu(RenderWindow& window, Sprite& background, const Font& font);