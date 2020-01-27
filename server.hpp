#include <SFML/Network.hpp>
#include <iostream>
#include <thread>
#include <boost/algorithm/string.hpp>
#include "sockets_constants.hpp"

#define MAX_CLIENTS 4

using namespace sf;
using namespace std;
using namespace ports_number;
using namespace datatype;

class Server
{
private:
    string server_name;
    Uint32 max_clients;
    Uint32 level;

    // IPv4 addresses of clients:
    vector<IpAddress> clients_address;
public:
    Server(): server_name(""), max_clients(MAX_CLIENTS), level(0) {};
    Server(string s_name, Uint32 max_cli, Uint32 lvl): 
        server_name(s_name), max_clients(max_cli), level(lvl) 
    {};

    void set_server_name(const string&);
    void set_max_clients(const Uint32&);
    void set_level(const Uint32&);

    string get_server_name() const;
    Uint32 get_max_clients() const;
    Uint32 get_level() const;
    
    // Listen for some new client connections:
    void listen_clients();
};

