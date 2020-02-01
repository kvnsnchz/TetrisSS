#include <vector>
#include <SFML/System/Thread.hpp>
#include "game.hpp"

using namespace std;

int main(int argc, char const *argv[]) {
    /*if(argc == 1){
         Server server("Kevin", 4, 2);
         Thread thread([&]() {server.listen_clients();});
         thread.launch();
     }
     else{
         Client client;
         Thread thread([&]() {
             client.search_servers();
             client.connect_server(0);
             client.ready();
            // client.disconnect_server();
         });
         thread.launch();
     }*/
    // get user's resolution:
    Vector2f resolution(VideoMode::getDesktopMode().width, VideoMode::getDesktopMode().height);

    // create the game window
    // which size depends on user's screen resolution:
    RenderWindow window(VideoMode(min(41.0f * 14 + 20.0f, resolution.x / 2), min(41.0f * 20 + 9.0f, resolution.y - 100.0f)), "TetrisSS 1.0");
    
    // set window's size:
    window.setSize(Vector2u(min(41.0f * 14 + 20.0f, resolution.x / 2), min(41.0f * 20 + 9.0f, resolution.y - 100.0f)));

    // set the background image:
    Texture texture;
    try {
        if (!texture.loadFromFile("background/bg2.jpg")) 
            throw 0;
    } catch (int e) {
        if (e == 0)
            cout << "Sorry, background image is not found!" << endl;
    }

    // set the font of window's text:
    Font font;
    try {
        if (!font.loadFromFile("amatic/Amatic-Bold.ttf")) 
            throw 0;
    } catch (int e) {
        if (e == 0)
            cout << "Sorry, font file is not found!" << endl;
    }

    // create a background itself:
    Sprite background;
    background.setTexture(texture);
    background.setColor(Color(255, 255, 255, 200));

    main_menu(window, background, font);
   // threads[0].join();
    return 0;
}
