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
    NEW_CLIENT_INFO,
    UPDATE_CLIENT_INFO,
    DELETE_CLIENT_INFO,
};

enum request_status {
    UNASSIGNED,
    NOT_READY,
    SUCCESS,
    ERROR,
};

struct client_data {
    IpAddress address;
    bool status;
    bool operator == (client_data autre_client){
       return address == autre_client.address;
    };
};

struct server_data {
    IpAddress address;
    string name;
    Uint32 clients_quantity;
    Uint32 level;
    Uint32 max_clients;
    vector<client_data> clients;
};

namespace ports_number {
    const unsigned short SERVER_PORT = 2004;
    const unsigned short CLIENT_PORT = 2006;
    const unsigned short CLIENT_TCP_PORT = 3006;
}

#endif
