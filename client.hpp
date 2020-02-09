#ifndef clientHPP
#define clientHPP

#include "board.hpp"
#include "player.hpp"

#define MAX_SEARCH_TIME 10
#define MAX_CONNECTION_TIME 10
#define MAX_RESPONSE_TIME 1

using namespace ports_number;

class Client: public Player
{
private:
    //IPv4 address of the game server:
    server_data _server_data;
    //Vector of game servers available to connect:
    vector<server_data> servers;
    UdpSocket socket;
public:
    Client(): Player() {
        connect_udp_socket();
    };
    
    Client(const bool& client_status, const string& client_nickname): Player(client_status, client_nickname) {
        connect_udp_socket();
    };

    ~Client() {
        disconnect_udp_socket();
    }

    server_data get_server_data() const;
    vector<server_data> get_servers() const;

    //Connect to Socket UDP
    void connect_udp_socket();
    //Disconnect to Socket UDP
    void disconnect_udp_socket();
    //Search for available game servers:
    void search_servers(request_status& status);
    //Connect to a specific game server from the available list:
    void connect_server(const unsigned pos, request_status& status);
    //Disconnect from the game server:
    void disconnect_server(request_status& status);
    //Listen the server
    void listen_server(request_status& status);
    //Ready or not ready to play
    void ready(bool);
    //Sending board data
    void send_board_data(Board&);
    //Listen the server during the game
    void listen_game(request_status& status);
    //Sending pause
    void pause(bool);
    //Sending game over
    void game_over();
};

#endif