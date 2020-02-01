#include "server.hpp"

void Server::set_server_name(const string& server_name) {
    this->server_name = server_name;
}

void Server::set_max_clients(const Uint32& max_clients) {
    this->max_clients = max_clients;
}

void Server::set_level(const Uint32& level) {
    this->level = level;
}

string Server::get_server_name() const{
    return server_name;
}

Uint32 Server::get_max_clients() const{
    return max_clients;
}

Uint32 Server::get_level() const{
    return level;
}

//Listen for new customer connections:
void Server::listen_clients() {
    //UDP socket
    UdpSocket socket;
    socket.setBlocking(false);
    
    //Port connection: 
    if (socket.bind(SERVER_PORT) != Socket::Done)
    {
        cout << "Server: Connection error" << endl;
        return;
    }

    Packet packet_recv;;
    IpAddress sender;
    unsigned short port;

    //Check if there is a message:
    while(1)
    {if (socket.receive(packet_recv, sender, port) == Socket::Done)
    {
        string _datatype;
        Packet packet_send;

        //Get the buffer information:
        packet_recv >> _datatype;
        
        //Check if the message is a request for information:
        if(!_datatype.compare(SERVER_INFO_REQUEST)){
            if(clients_address.size() <= max_clients){
                //Filling send buffer:
                packet_send << SERVER_INFO_RESPONSE;
                packet_send << server_name;
                packet_send << (Uint32)clients_address.size();
                packet_send << level;

                //Send server information to client 
                if (socket.send(packet_send, sender, CLIENT_PORT) != sf::Socket::Done)
                    cout << "Server: Send error" << endl;

                cout << "Client " << _datatype << endl;
            }
        }
        else{
            //Check if the message is a request of connection:
            if(!_datatype.compare(SERVER_CONN_REQUEST)){
                //Check if I can receive more clients:
                if(clients_address.size() == max_clients){
                    //Buffer filling with error message:
                    packet_send << SERVER_CONN_RESPONSE_ERROR;
                }
                else{
                    //Adding the new client to the client list:
                    if(find(clients_address.begin(), clients_address.end(), sender) == clients_address.end())
                        clients_address.emplace_back(sender);
                    //Buffer filling with success message
                    packet_send << SERVER_CONN_RESPONSE_SUCCESS;
                }
                //Send information to client
                if (socket.send(packet_send, sender, CLIENT_PORT) != sf::Socket::Done)
                    cout << "Server: Send error" << endl;
                
                cout << "New client " << sender << endl;
            }
            else{
                //Check if the message is a request of disconnection:
                if(!_datatype.compare(SERVER_DISCONNECTION)){
                    vector<IpAddress>::iterator it = find(clients_address.begin(), clients_address.end(), sender);
                    if(it != clients_address.end()){
                        //Removing the client of the client list:
                        clients_address.erase(it);
                        cout << "Delete client " << sender << endl;
                    }
                }
            }
        }
        std::cout << "Received bytes from " << sender << " on port " << port << std::endl;
    }}
}