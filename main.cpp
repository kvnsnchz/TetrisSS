#include <vector>
#include <SFML/System/Thread.hpp>
#include "menu.hpp"

using namespace std;

int main(int argc, char const *argv[]) {
    // set the background image:
    Texture texture;
    try {
        if (!texture.loadFromFile("images/bg2.jpg")) 
            throw 0;
    } catch (int e) {
        if (e == 0)
            cout << "Sorry, background image is not found!" << endl;
    }

    // set the font of window's text:
    Font font;
    try {
        if (!font.loadFromFile("fonts/Amatic-Bold.ttf")) 
            throw 0;
    } catch (int e) {
        if (e == 0)
            cout << "Sorry, font file is not found!" << endl;
    }

    // create a background itself:
    Sprite background;
    background.setTexture(texture);
    background.setColor(Color(255, 255, 255, 200));

    // create the game menu:
    Menu* menu = new Menu(background, font);

    menu->main_menu(true);
    
    // threads[0].join();
    return 0;
}
