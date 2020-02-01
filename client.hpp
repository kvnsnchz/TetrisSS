#include <SFML/Network.hpp>
#include <iostream>
#include <thread>
#include <cstring>
#include <chrono>
#include <boost/algorithm/string.hpp>
#include "sockets_constants.hpp"

#define MAX_SEARCH_TIME 10
#define MAX_CONNECTION_TIME 2
#define MAX_RESPONSE_TIME 1

using namespace sf;
using namespace std;
using namespace ports_number;

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
    //Listen the server
    void listen_sever();
    //Ready to play
    bool ready();
};

