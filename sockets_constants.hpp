#include <SFML/Network.hpp>
#include <string>

using namespace std;
using namespace sf;

#ifndef SOCKETS_CONSTANTS
#define SOCKETS_CONSTANTS

enum datatype {
    SERVER_INFO_REQUEST,
    SERVER_INFO_RESPONSE,
    SERVER_CONN_REQUEST,
    SERVER_CONN_RESPONSE_SUCCESS,
    SERVER_CONN_RESPONSE_ERROR,
    SERVER_DISCONNECTION,
    CLIENT_READY,
    CLIENT_READY_SUCCESS,
    CLIENT_UPDATE_INFO,
};

struct server_data {
    IpAddress address;
    string name;
    Uint32 clients_quantity;
    Uint32 level;
    vector<IpAddress> clients_address;
}; 

struct client_data {
    IpAddress address;
    bool status;
};

namespace ports_number {
    const unsigned short SERVER_PORT = 2004;
    const unsigned short CLIENT_PORT = 2006;
}

#endif
