#ifndef clientHPP
#define clientHPP

#include "board.hpp"
#include "player.hpp"

#define MAX_SEARCH_TIME 10
#define MAX_CONNECTION_TIME 10
#define MAX_RESPONSE_TIME 1

using namespace ports_number;

/**
 * @brief Class that implements the client (session guest) and his functions. 
 * 
 */
class Client: public Player
{
private:
    //IPv4 address of the game server:
    server_data _server_data;
    //Vector of game servers available to connect:
    vector<server_data> servers;
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
    /**
     * @brief Function that is looking for available servers to play with. 
     * 
     * @param status 
     */
    void search_servers(request_status& status);
    //Connect to a specific game server from the available list:
    void connect_server(const unsigned pos, request_status& status);
    //Disconnect from the game server:
    void disconnect_server(request_status& status);
    /**
     * @brief Function that receives all necessary info from server before the game start.
     * 
     * @param status 
     */
    void listen_server(request_status& status);
    //Ready or not ready to play
    void ready(bool);
    /**
     * @brief Sends current client's game info th the server.
     * 
     */
    void send_board_data(Board&);
    //Listen the server during the game
    void listen_game(Board& game_board, request_status& status);
    //Sending pause
    void pause(bool);
    //Sending game over
    void game_over();
};

#endif