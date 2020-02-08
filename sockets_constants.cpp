#define sockets_constantsCPP

#include "sockets_constants.hpp"

Packet& operator <<(sf::Packet& packet, server_data& _server_data)
{
    return packet << _server_data.name
        << _server_data.clients_quantity
        << _server_data.level
        << _server_data.max_clients;
}

Packet& operator >>(sf::Packet& packet, server_data& _server_data)
{
    return packet >> _server_data.name 
        >> _server_data.clients_quantity 
        >> _server_data.level
        >> _server_data.max_clients;
}

Packet& operator <<(sf::Packet& packet, client_data& _client_data)
{
    return packet << _client_data.address.toString()
        << _client_data.status
        << _client_data.score;
}

Packet& operator >>(sf::Packet& packet, client_data& _client_data)
{
    string client_address;
    packet >> client_address;
    _client_data.address = client_address;

    return packet >> _client_data.status
        >> _client_data.score;
}