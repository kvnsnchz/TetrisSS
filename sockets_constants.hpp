#ifndef SOCKETS_CONSTANTS
#define SOCKETS_CONSTANTS

#include <SFML/Network.hpp>
#include <string>
#include "board.hpp"

using namespace std;
using namespace sf;

/**
 * @brief All possible types of data that could be sent or received.
 * 
 */
enum datatype {
    SERVER_INFO_REQUEST,
    SERVER_INFO_RESPONSE,
    SERVER_CONN_REQUEST,
    SERVER_CONN_RESPONSE_SUCCESS,
    SERVER_CONN_RESPONSE_ERROR,
    SERVER_DISCONNECTION,
    CLIENT_DISCONNECTION,
    CLIENT_NOT_READY,
    CLIENT_NOT_READY_SUCCESS,
    CLIENT_READY,
    CLIENT_READY_SUCCESS,
    NEW_CLIENT_INFO,
    UPDATE_CLIENT_INFO,
    DELETE_CLIENT_INFO,
    SERVER_GAME_START,
    CLIENT_GAME_UPDATE,
    SERVER_GAME_UPDATE,
    CLIENT_GAME_PAUSE,
    CLIENT_GAME_RESUME,
    CLIENT_GAME_OVER,
};

/**
 * @brief All possible ttransmission statuses.
 * 
 */
enum request_status {
    UNASSIGNED,
    NOT_READY,
    SUCCESS,
    ERROR,
    SERVER_DISCONNECTED,
    READY_SUCCESS,
    READY_ERROR,
    NOT_READY_SUCCESS,
    NOT_READY_ERROR,
    NOT_CHANGED,
    CHANGED,
    GAME_START,
};

/**
 * @brief All possible client statuses.
 * 
 */
enum client_status {
    STATUS_READY,
    STATUS_NOT_READY,
    STATUS_PAUSED,
    STATUS_GAME_OVER,
};

/**
 * @brief Structure gathering all necessary client's game info to transmit.
 * 
 */
struct client_data {
    IpAddress address;
    string nickname;
    client_status status;
    Int64 score;
    Uint32 complexity;
    unsigned map[BOARD_GRID_WIDTH][BOARD_GRID_HEIGHT + FIGURE_GRID_HEIGHT];
    bool operator == (client_data other_client){
       return address == other_client.address;
    };
    friend Packet& operator <<(sf::Packet& packet, client_data& _client_data);
    friend Packet& operator >>(sf::Packet& packet, client_data& _client_data);
};

/**
 * @brief Structure gathering all necessary server's data.
 * 
 */
struct server_data {
    IpAddress address;
    string name;
    Uint32 clients_quantity;
    Uint32 level;
    Uint32 max_clients;
    vector<client_data> clients;
    bool operator == (server_data other_server){
        return address == other_server.address;
    };
    friend Packet& operator <<(sf::Packet& packet, server_data& _server_data);
    friend Packet& operator >>(sf::Packet& packet, server_data& _server_data);
};


/**
 * @brief Not exhaustive list of the port numbers.
 * 
 */
namespace ports_number {
    const unsigned short SERVER_PORT = 2004;
    const unsigned short CLIENT_PORT = 2006;
    const unsigned short CLIENT_TCP_PORT = 3006;
}

#endif
