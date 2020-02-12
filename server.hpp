#ifndef serverHPP
#define serverHPP

#include "player.hpp"

#define MAX_CLIENTS 4

using namespace ports_number;

/**
 * @brief Class that implements the server (host player, session owner) and his functions. 
 * 
 */
class Server: public Player
{
private:
    Uint32 max_clients;
    Uint32 level;

    // IPv4 addresses of clients:
    vector<client_data> clients;
    IpAddress local_ip_address;
public:
    Server(): Player(), max_clients(MAX_CLIENTS), level(0) {
        local_ip_address = IpAddress::getLocalAddress();
        clients.emplace_back(client_data{local_ip_address, "", STATUS_NOT_READY});
        connect_udp_socket();
    };

    Server(const bool& new_player_status, const string& server_nickname, Uint32 max_cli, Uint32 lvl): 
        Player(new_player_status, server_nickname), max_clients(max_cli), level(lvl) 
    {
        local_ip_address = IpAddress::getLocalAddress();
        clients.emplace_back(client_data{local_ip_address, server_nickname, STATUS_NOT_READY});
        connect_udp_socket();
    };
    
    ~Server(){
        disconnect_udp_socket();
    };

    void set_max_clients(const Uint32&);
    void set_level(const Uint32&);

    vector<client_data> get_clients() const;
    Uint32 get_max_clients() const;
    Uint32 get_level() const;

    //Connect to Socket UDP
    void connect_udp_socket();
    //Disconnect to Socket UDP
    void disconnect_udp_socket();
    /**
     * @brief Function to listen and find some new potential clients and their IP addresses.
     * 
     * @param status 
     */
    void listen_clients(request_status& status);
    //Disconnected server
    /**
     * @brief Inform all the clients about disconnection.
     * 
     */
    void disconnect();
    //Server ready or not ready to play
    void ready(bool is_ready);
    /**
     * @brief Send an information about new game start to all the clients.
     * 
     * @return true 
     * @return false 
     */
    bool start();
    /**
     * @brief Send a game info from each client to all the clients. 
     * 
     * @param board 
     */
    void send_clients_board_data(Board& board);
    //Listen clients during the game
    void listen_game(Board& game_board, request_status& status);
    //Sending pause
    void pause(bool);
    //Sending game over
    void game_over();
    //restart game
    void restart_game();
};

#endif