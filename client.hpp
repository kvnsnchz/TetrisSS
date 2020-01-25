#include <SFML/Network.hpp>
#include <iostream>
#include <thread>
#include "sockets_constants.hpp"

using namespace sf;
using namespace std;
using namespace datatype;
using namespace ports_number;

class Client
{
private:
    IpAddress address;
    unsigned short port;
public:
    Client(): port(CLIENT_PORT) {};
    Client(IpAddress addr, unsigned short prt): address(addr), port(prt) {};

    void search_servers();
};

