#include <SFML/Network.hpp>
#include <iostream>
#include <thread>
#include "sockets_constants.hpp"

using namespace sf;
using namespace std;
using namespace ports_number;

class Server
{
private:
    IpAddress address;
    unsigned short port;
public:
    Server(): port(SERVER_PORT) {};
    Server(IpAddress addr, unsigned short prt): address(addr), port(prt) {};

    void search_clients();
};

