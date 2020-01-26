#include <SFML/Network.hpp>
#include <iostream>
#include <thread>
#include <cstring>
#include <chrono>
#include <boost/algorithm/string.hpp>
#include "sockets_constants.hpp"

#define MAX_SEARCH_TIME 10
#define MAX_CONNECTION_TIME 2

using namespace sf;
using namespace std;
using namespace datatype;
using namespace ports_number;

struct server_data {
    IpAddress address;
    string name;
    Uint32 clients_quantity;
    Uint32 level;
}; 

class Client
{
private:
    //IPv4 address of the game server:
    IpAddress server_address;
    //Vector of game servers available to connect:
    vector<server_data> servers;
public:
    Client() {};
    
    vector<server_data> get_servers() const;
    //Search for available game servers:
    void search_servers();
    //Connect to a specific game server from the available list:
    bool connect_server(const unsigned pos = 0);
    //Disconnect from the game server:
    bool disconnect_server();
};

