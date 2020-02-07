#include <iostream>
#include <thread>
#include <boost/algorithm/string.hpp>
#include "sockets_constants.hpp"

#define MAX_CLIENTS 4

using namespace ports_number;

class Server
{
private:
    string server_name;
    Uint32 max_clients;
    Uint32 level;

    // IPv4 addresses of clients:
    vector<client_data> clients;
    UdpSocket socket;
    IpAddress local_ip_address;
public:
    Server(): server_name(""), max_clients(MAX_CLIENTS), level(0) {
        local_ip_address = IpAddress::getLocalAddress();
        clients.emplace_back(client_data{local_ip_address, false});
        connect_udp_socket();
    };
    Server(string s_name, Uint32 max_cli, Uint32 lvl): 
        server_name(s_name), max_clients(max_cli), level(lvl) 
    {
        local_ip_address = IpAddress::getLocalAddress();
        clients.emplace_back(client_data{local_ip_address, false});
        connect_udp_socket();
    };
    ~Server(){
        disconnect_udp_socket();
    };

    void set_server_name(const string&);
    void set_max_clients(const Uint32&);
    void set_level(const Uint32&);
    
    string get_server_name() const;
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
    void start();
};

