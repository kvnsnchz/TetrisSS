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
    //IPv4 addresses of clients:
    vector<IpAddress> clients_address;
public:
    Server() {};
    //Listen for new customer connections:
    void listen_clients();
};

