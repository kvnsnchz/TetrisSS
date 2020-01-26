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

struct server_t {
    IpAddress address;
    Uint32 clients_quantity;
}; 

class Client
{
private:
    IpAddress server_address;
    vector<server_t> servers;
public:
    Client() {};
    
    vector<server_t> get_servers() const;
    void search_servers();
    bool connect_server(const unsigned pos = 0);
    bool disconnect_server();
};

