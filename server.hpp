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
public:
    Server(): server_name(""), max_clients(MAX_CLIENTS), level(0) {
        clients.emplace_back(client_data{IpAddress::getLocalAddress(), false});
    };
    Server(string s_name, Uint32 max_cli, Uint32 lvl): 
        server_name(s_name), max_clients(max_cli), level(lvl) 
    {
        clients.emplace_back(client_data{IpAddress::getLocalAddress(), false});
    };

    void set_server_name(const string&);
    void set_max_clients(const Uint32&);
    void set_level(const Uint32&);

    string get_server_name() const;
    Uint32 get_max_clients() const;
    Uint32 get_level() const;
    
    // Listen for some new client connections:
    void listen_clients();
};

