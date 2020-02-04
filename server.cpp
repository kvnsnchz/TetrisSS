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

vector<client_data> Server::get_clients() const{
    return clients;
}

Uint32 Server::get_max_clients() const{
    return max_clients;
}

Uint32 Server::get_level() const{
    return level;
}

//Connect to udp socket
void Server::connect_udp_socket(){
    if (socket.bind(SERVER_PORT) != sf::Socket::Done)
    {
        cout << "Server: Connection error" << endl;
        return;
    }
}
//Disconnect to udp socket
void Server::disconnect_udp_socket(){
    socket.unbind();
}

void Server::disconnect(){
    socket.setBlocking(true);
    //Filling send buffer:
    Packet packet_send;
    //Server disconnection request:
    packet_send << CLIENT_DISCONNECTION;
    
    //Sending server disconnection: 
    for(unsigned i = 0; i < clients.size(); i ++){
        if (socket.send(packet_send, clients[i].address, CLIENT_PORT) != sf::Socket::Done)
        {
            cout << "Client: Send error" << endl;
            return;
        }
    }
    clients.clear();
}

//Listen for new customer connections:
void Server::listen_clients(request_status& status) {
    status = NOT_CHANGED;
    socket.setBlocking(false);
    
    Packet packet_recv;;
    IpAddress sender;
    unsigned short port;

    while(true){
        //Check if there is a message:
        if (socket.receive(packet_recv, sender, port) == Socket::Done)
        {
            Packet packet_send, gen_packet_send;
            datatype _datatype;
            Uint32 datatype_value;
            //Get the buffer information:
            packet_recv >> datatype_value;
            _datatype = (datatype) datatype_value;
            
            switch ((unsigned)_datatype)
            {
            //Check if the message is a request for information:
            case SERVER_INFO_REQUEST:
                if(clients.size() <= max_clients){
                    //Filling send buffer:
                    packet_send << SERVER_INFO_RESPONSE;
                    packet_send << server_name;
                    packet_send << (Uint32)clients.size();
                    packet_send << level;
                    packet_send << max_clients;                    
                    //Send server information to client 
                    if (socket.send(packet_send, sender, CLIENT_PORT) != sf::Socket::Done)
                        cout << "Server: Send error" << endl;

                    cout << "Client " << _datatype << endl;
                }
                break;
            //Check if the message is a request of connection:
            case SERVER_CONN_REQUEST:
                //Check if I can receive more clients:
                if(clients.size() == max_clients){
                    //Buffer filling with error message:
                    packet_send << SERVER_CONN_RESPONSE_ERROR;
                }
                else{
                    //Adding the new client to the client list:
                    client_data _client_data{sender, false};
                    if(find(clients.begin(), clients.end(), _client_data) == clients.end())
                        clients.emplace_back(_client_data);
                    //Buffer filling with success message
                    packet_send << SERVER_CONN_RESPONSE_SUCCESS;
                    packet_send << server_name;
                    packet_send << (Uint32)clients.size();
                    packet_send << level;
                    packet_send << max_clients;

                    gen_packet_send << NEW_CLIENT_INFO;
                   
                    for(unsigned i = 0; i < clients.size(); i ++){
                        packet_send << clients[i].address.toString();
                        packet_send << clients[i].status;

                        gen_packet_send << clients[i].address.toString();
                        gen_packet_send << clients[i].status;
                    }
                    
                }
                //Send information to client
                if (socket.send(packet_send, sender, CLIENT_PORT) != sf::Socket::Done)
                    cout << "Server: Send error" << endl;
                
                if(gen_packet_send.getDataSize() > 0){
                    for(unsigned i = 0; i < clients.size(); i ++){
                        if(clients[i].address != sender)
                            if (socket.send(gen_packet_send, clients[i].address, CLIENT_PORT) != sf::Socket::Done)
                                cout << "Server: Send error" << endl;
                    }
                }

                status = CHANGED;
                cout << "New client " << sender << endl;
                
                break;
            //Check if the message is a request of disconnection:
            case CLIENT_DISCONNECTION:
            {
                vector<client_data>::iterator it = find(clients.begin(), clients.end(), client_data{sender, false});
                if(it != clients.end()){
                    //Buffer filling with delete client position
                    packet_send << DELETE_CLIENT_INFO;
                    packet_send << (Uint32) (it - clients.begin());

                    //Removing the client of the client list:
                    clients.erase(it);
                    cout << "Delete client " << sender << endl;
                    status = CHANGED;
                   
                    for(unsigned i = 0; i < clients.size(); i++){
                        if (socket.send(packet_send, clients[i].address, CLIENT_PORT) != sf::Socket::Done)
                            cout << "Server: Send error" << endl;
                    }
                }
                break;
            }
            //Check if the message is as ready client message:
            case CLIENT_READY:
                //Buffer filling with success message
                vector<client_data>::iterator it = find(clients.begin(), clients.end(), client_data{sender, false});
                if(it == clients.end())
                    break;
                (*it).status = true;
                packet_send << UPDATE_CLIENT_INFO;
                packet_send << (Uint32)(it - clients.begin());
                packet_send << (*it).address.toString();
                packet_send << (*it).status;

                for(unsigned i = 0; i < clients.size(); i++){
                    if(clients[i].address != sender)
                        if (socket.send(packet_send, clients[i].address, CLIENT_PORT) != sf::Socket::Done)
                            cout << "Server: Send error" << endl;
                }

                packet_send.clear();
                packet_send << CLIENT_READY_SUCCESS;
                if (socket.send(packet_send, sender, CLIENT_PORT) != sf::Socket::Done)
                    cout << "Server: Send error" << endl;
                
                status = CHANGED;
                cout << "Client " << sender << " ready" << endl;
                break;
            }
           
            std::cout << "Received bytes from " << sender << " on port " << port << std::endl;
        }
    }
}