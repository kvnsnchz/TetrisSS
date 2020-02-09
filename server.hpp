#ifndef serverHPP
#define serverHPP

#include "player.hpp"

#define MAX_CLIENTS 4

using namespace ports_number;

class Server: public Player
{
private:
    Uint32 max_clients;
    Uint32 level;

    // IPv4 addresses of clients:
    vector<client_data> clients;
    UdpSocket socket;
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
    // Listen for some new client connections:
    void listen_clients(request_status& status);
    //Disconnected server
    void disconnect();
    //Server ready or not ready to play
    void ready(bool is_ready);
    //Game start
    bool start();
    //Sending clients board data
    void send_clients_board_data(Board& board);
    //Listen clients during the game
    void listen_game(request_status& status);
    //Sending pause
    void pause(bool);
};

#endif