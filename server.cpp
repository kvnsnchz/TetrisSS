#define endHPP

#include "server.hpp"

void Server::set_max_clients(const Uint32& max_clients) {
    this->max_clients = max_clients;
}

void Server::set_level(const Uint32& level) {
    this->level = level;
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
    //Filling send buffer:
    Packet packet_send;
    //Server disconnection request:
    packet_send << SERVER_DISCONNECTION;
    
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

//Server ready or not ready to play
void Server::ready(bool is_ready){
    vector<client_data>::iterator it = find(clients.begin(), clients.end(), client_data{local_ip_address, "", STATUS_NOT_READY});
    if(it == clients.end())
        return;
        
    (*it).status = is_ready ? STATUS_READY : STATUS_NOT_READY;
    //Filling send buffer:
    Packet packet_send;

    packet_send << UPDATE_CLIENT_INFO;
    packet_send << (Uint32)(it - clients.begin());
    packet_send << (*it);

    for(unsigned i = 0; i < clients.size(); i++){
        if(clients[i].address != local_ip_address)
            if (socket.send(packet_send, clients[i].address, CLIENT_PORT) != sf::Socket::Done)
                cout << "Server: Send error" << endl;
    }
    
}

// Start multiplayer game
bool Server::start() {
    // Check everyone's readiness:
    for (unsigned i = 0; i < clients.size(); i++) {
        if (clients[i].status != STATUS_READY) {
            cout << "Can't start a game, some players aren't ready!!!" << endl;
            return false;
        }
    }
    
    // Filling send buffer:
    Packet packet_send;

    packet_send << SERVER_GAME_START;

    for (unsigned i = 0; i < clients.size(); i++) {
        if (clients[i].address != local_ip_address)
            if (socket.send(packet_send, clients[i].address, CLIENT_PORT) != sf::Socket::Done)
                cout << "Server: Send error" << endl;
    }

    return true;
}

// Listen for new customer connections:
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
                    server_data _server_data{
                        local_ip_address, player_nickname, (Uint32)clients.size(), level, max_clients, vector<client_data>()
                    };

                    packet_send << SERVER_INFO_RESPONSE;
                    packet_send << _server_data;                   
                    //Send server information to client 
                    if (socket.send(packet_send, sender, CLIENT_PORT) != sf::Socket::Done)
                        cout << "Server: Send error" << endl;

                    cout << "Client " << _datatype << endl;
                }
                break;
            //Check if the message is a request of connection:
            case SERVER_CONN_REQUEST:
            {    
                string client_nickname = "";
                //Check if I can receive more clients:
                if(clients.size() == max_clients){
                    //Buffer filling with error message:
                    packet_send << SERVER_CONN_RESPONSE_ERROR;
                }
                else{
                    packet_recv >> client_nickname;
                    //Adding the new client to the client list:
                    client_data _client_data{sender, client_nickname, STATUS_NOT_READY};
                    if(find(clients.begin(), clients.end(), _client_data) == clients.end())
                        clients.emplace_back(_client_data);
                    //Buffer filling with success message
                    server_data _server_data{
                        local_ip_address, player_nickname, (Uint32)clients.size(), level, max_clients, vector<client_data>()
                    };

                    packet_send << SERVER_CONN_RESPONSE_SUCCESS;
                    packet_send << _server_data;

                    gen_packet_send << NEW_CLIENT_INFO;
                    gen_packet_send << (Uint32)clients.size();
                    
                    for(unsigned i = 0; i < clients.size(); i ++){
                        packet_send << clients[i];

                        gen_packet_send << clients[i];
                    }
                    
                }
                //Send information to client
                if (socket.send(packet_send, sender, CLIENT_PORT) != sf::Socket::Done)
                    cout << "Server: Send error" << endl;
                    
                if(gen_packet_send.getDataSize() > 0){
                    for(unsigned i = 0; i < clients.size(); i ++){
                        if(clients[i].address != sender && clients[i].address != local_ip_address)
                            if (socket.send(gen_packet_send, clients[i].address, CLIENT_PORT) != sf::Socket::Done)
                                cout << "Server: Send error" << endl;
                    }
                    status = CHANGED;
                    cout << "New client " << client_nickname << endl;
                    break;
                }

                cout << "Error new client " << sender << endl;
                
                break;
            }
            //Check if the message is a request of disconnection:
            case CLIENT_DISCONNECTION:
            {
                vector<client_data>::iterator it = find(clients.begin(), clients.end(), client_data{sender, "", STATUS_NOT_READY});
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
            {    //Buffer filling with success message
                vector<client_data>::iterator it = find(clients.begin(), clients.end(), client_data{sender, "", STATUS_NOT_READY});
                if(it == clients.end())
                    break;
                (*it).status = STATUS_READY;
                packet_send << UPDATE_CLIENT_INFO;
                packet_send << (Uint32)(it - clients.begin());
                packet_send << (*it);

                for(unsigned i = 0; i < clients.size(); i++){
                    if(clients[i].address != sender && clients[i].address != local_ip_address)
                        if (socket.send(packet_send, clients[i].address, CLIENT_PORT) != sf::Socket::Done)
                            cout << "Server: Send error" << endl;
                }

                packet_send.clear();
                packet_send << CLIENT_READY_SUCCESS;
                if (socket.send(packet_send, sender, CLIENT_PORT) != sf::Socket::Done)
                    cout << "Server: Send error" << endl;
                
                status = CHANGED;
                cout << "Client " << sender << "is ready" << endl;
                break;
            }
            case CLIENT_NOT_READY:
                //Buffer filling with success message
                vector<client_data>::iterator it = find(clients.begin(), clients.end(), client_data{sender, "", STATUS_NOT_READY});
                if(it == clients.end())
                    break;
                (*it).status = STATUS_NOT_READY;
                packet_send << UPDATE_CLIENT_INFO;
                packet_send << (Uint32)(it - clients.begin());
                packet_send << (*it);
                
                for(unsigned i = 0; i < clients.size(); i++){
                    if(clients[i].address != sender && clients[i].address != local_ip_address)
                        if (socket.send(packet_send, clients[i].address, CLIENT_PORT) != sf::Socket::Done)
                            cout << "Server: Send error" << endl;
                }

                packet_send.clear();
                packet_send << CLIENT_NOT_READY_SUCCESS;
                if (socket.send(packet_send, sender, CLIENT_PORT) != sf::Socket::Done)
                    cout << "Server: Send error" << endl;
                
                status = CHANGED;
                cout << "Client " << sender << "is not ready" << endl;
                break;

            }
           
            std::cout << "Received bytes from " << sender << " on port " << port << std::endl;
        }
    }
}

//Sending clients board data
void Server::send_clients_board_data(Board& board) {
    //Filling send buffer:
    Packet packet_send;
    
    packet_send << SERVER_GAME_UPDATE;

    for(unsigned i = 0; i < clients.size(); i ++){
        if(clients[i].address == local_ip_address)
            clients[i].score = board.get_score();

        packet_send << (Uint32)clients[i].status;
        packet_send << clients[i].score;

        for(unsigned j = 0; j < BOARD_GRID_WIDTH; j ++){
            for(unsigned k = 0; k < BOARD_GRID_HEIGHT + FIGURE_GRID_HEIGHT; k ++){
                if(clients[i].address == local_ip_address)
                    clients[i].map[j][k] = board.get_map()[j][k];
                packet_send << (Uint32)clients[i].map[j][k];
            }
        }
    }
    
    for(unsigned i = 0; i < clients.size(); i ++){
        if(clients[i].address != local_ip_address)
            //Sending board data message: 
            if (socket.send(packet_send, clients[i].address, CLIENT_PORT) != sf::Socket::Done)
            {
                cout << "Client: Send error" << endl;
            }
    }
    
}

//Listen clients during the game
void Server::listen_game(Board& game_board, request_status& status){
    status = NOT_CHANGED;
    socket.setBlocking(false);
    
    Packet packet_recv;
    IpAddress sender;
    unsigned short port;

    while(true){
        //Check if there is a message:
        if (socket.receive(packet_recv, sender, port) == Socket::Done){
            datatype _datatype;
            Uint32 datatype_value;
            //Get the buffer information:
            packet_recv >> datatype_value;
            _datatype = (datatype) datatype_value;
            
            switch ((unsigned)_datatype)
            {
                case CLIENT_GAME_UPDATE:
                {    
                    vector<client_data>::iterator it = find(clients.begin(), clients.end(), client_data{sender, "", STATUS_NOT_READY});
                    if(it == clients.end())
                        break;

                    packet_recv >> (*it).score;

                    for(unsigned i = 0; i < BOARD_GRID_WIDTH; i++){
                        for(unsigned j = 0; j < BOARD_GRID_HEIGHT + FIGURE_GRID_HEIGHT; j++){
                            Int32 value;
                            packet_recv >> value;
                            (*it).map[i][j] = value;
                        }
                    }
                    break;
                }
                case CLIENT_GAME_PAUSE:
                {
                    vector<client_data>::iterator it = find(clients.begin(), clients.end(), client_data{sender, "", STATUS_NOT_READY});
                    
                    if(it != clients.end())
                        (*it).status = STATUS_PAUSED;

                    send_clients_board_data(game_board);
                    break;
                }
                case CLIENT_GAME_RESUME:
                    for(unsigned i = 0; i < clients.size(); i++){
                        clients[i].status = STATUS_READY;
                    }
                    send_clients_board_data(game_board);
                    break;
                case CLIENT_GAME_OVER:
                {  
                    vector<client_data>::iterator it = find(clients.begin(), clients.end(), client_data{sender, "", STATUS_NOT_READY});

                    if(it != clients.end())
                        (*it).status = STATUS_GAME_OVER;
                    break;
                }
                //Check if the message is a request of disconnection:
                case CLIENT_DISCONNECTION:
                {   
                    Packet packet_send;
                    vector<client_data>::iterator it = find(clients.begin(), clients.end(), client_data{sender, "", STATUS_NOT_READY});
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
            }
        }
    }
}

void Server::pause(bool is_pause) {
    if(is_pause){
        vector<client_data>::iterator it = find(clients.begin(), clients.end(), client_data{local_ip_address, "", STATUS_NOT_READY});

        if(it != clients.end())
            (*it).status = STATUS_PAUSED;
    }
    else{
        for(unsigned i = 0; i < clients.size(); i++){
            clients[i].status = STATUS_READY;
        }
    }   
}

void Server::game_over() {
    vector<client_data>::iterator it = find(clients.begin(), clients.end(), client_data{local_ip_address, "", STATUS_NOT_READY});

    if(it != clients.end())
        (*it).status = STATUS_GAME_OVER;
}

void Server::restart_game() {
    for(unsigned i = 0; i < clients.size(); i++){
        clients[i].status = STATUS_NOT_READY;
    }
}

