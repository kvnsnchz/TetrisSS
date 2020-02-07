#include <vector>
#include <SFML/System/Thread.hpp>
#include "game.hpp"

using namespace std;

int main(int argc, char const *argv[]) {
    // if(argc == 1){
    //      Server server("Kevin", 4, 2);
    //      server.connect_udp_socket();
    //      Thread thread([&]() {server.listen_clients();});
    //      thread.launch();
    //  }
    //  else{
    //      Client client;
    //      Thread thread([&]() {
    //          client.connect_udp_socket();
    //          client.search_servers();
    //          request_status status;
    //          client.connect_server(0, status);
    //          cout << status << endl;
    //          //client.ready();
    //         // client.disconnect_server();
    //         client.disconnect_udp_socket();
    //      });
    //      thread.launch();
    //  }

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

    menu->main_menu();
    
    // threads[0].join();
    return 0;
}
