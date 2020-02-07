#include "client.hpp"

server_data Client::get_server_data() const {
    return _server_data;
}

vector<server_data> Client::get_servers() const {
    return servers;
}
//Connect to udp socket
void Client::connect_udp_socket(){
    if (socket.bind(CLIENT_PORT) != sf::Socket::Done)
    {
        cout << "Client: Connection error" << endl;
        return;
    }
}
//Disconnect to udp socket
void Client::disconnect_udp_socket(){
    socket.unbind();
}
//Search for available game servers:
void Client::search_servers(){
    //Filling send buffer:
    Packet packet_send;
    //Server information request:
    packet_send << SERVER_INFO_REQUEST;
    socket.setBlocking(true);

    //Sending broadcast message for search available servers: 
    if (socket.send(packet_send, IpAddress::Broadcast, SERVER_PORT) != sf::Socket::Done)
    {
        cout << "Client: Send error" << endl;
        return;
    }

    chrono::time_point<chrono::system_clock> start = chrono::system_clock::now();
    chrono::duration<double> elapsed_seconds;
    //Setting non blocking socket:
    socket.setBlocking(false);
    
    do{
        Packet packet_recv;
        IpAddress sender;
        unsigned short port;
        //Check if there is a message:
        if (socket.receive(packet_recv, sender, port) == Socket::Done)
        {
            datatype _datatype;
            Uint32 datatype_value;
            //Get the buffer information:
            packet_recv >> datatype_value;
            _datatype = (datatype) datatype_value;
            //Check if the message is a response of server information:
            if(_datatype == SERVER_INFO_RESPONSE){
                //Adding server information to server list:
                vector<server_data>::iterator it = find(servers.begin(), servers.end(), server_data{
                    sender, "", 0, 0, 0, vector<client_data>()
                });

                if(it == servers.end()){
                    server_data _server_data;
                    _server_data.address = sender;
                    packet_recv >> _server_data.name;
                    packet_recv >> _server_data.clients_quantity;
                    packet_recv >> _server_data.level;
                    packet_recv >> _server_data.max_clients;

                    servers.emplace_back(_server_data);
                    
                    cout << "Server: " << servers.back().name << " address = " << servers.back().address << " clients_quantity = " << servers.back().clients_quantity << endl;
                
                }
            }
        }
        elapsed_seconds = chrono::system_clock::now() - start;
        //Waiting for servers information for MAX_SEARCH_TIME
    } while(elapsed_seconds.count() <= MAX_SEARCH_TIME);
    
}

//Connect to a specific game server from the available list:
void Client::connect_server(const unsigned pos, request_status& status){
    //Checking that the selected server is in the list:

    if(pos > servers.size()){
        status = ERROR;
        return;
    }

    status = NOT_READY;
    //Filling send buffer:
    Packet packet_send;
    //Server connection request:
    packet_send << SERVER_CONN_REQUEST;
    
    //Sending server connection request: 
    if (socket.send(packet_send, servers[pos].address, SERVER_PORT) != sf::Socket::Done)
    {
        status = ERROR;
        cout << "Client: Send error" << endl;
        return;
    }

    chrono::time_point<chrono::system_clock> start = chrono::system_clock::now();
    chrono::duration<double> elapsed_seconds;
    //Setting non blocking socket:
    socket.setBlocking(false);
    
    do{
        Packet packet_recv;
        IpAddress sender;
        unsigned short port;
        //Check if there is a message:
        if (socket.receive(packet_recv, sender, port) == Socket::Done)
        {
            //Checking that the address of the sender 
            //is the same as that of the selected server
            if(sender == servers[pos].address){
                datatype _datatype;
                Uint32 datatype_value;
                //Get the buffer information:
                packet_recv >> datatype_value;
                _datatype = (datatype) datatype_value;
                
                switch ((unsigned)_datatype)
                {
                //Check if the message is a server connection response error:
                case SERVER_CONN_RESPONSE_ERROR:
                    servers.erase(servers.begin() + pos);
                    status = ERROR;
                    return;
                //Check if the message is a server connection response success:
                case SERVER_CONN_RESPONSE_SUCCESS:
                    _server_data.address = servers[pos].address;
                    packet_recv >> _server_data.name;
                    packet_recv >> _server_data.clients_quantity;
                    packet_recv >> _server_data.level;
                    packet_recv >> _server_data.max_clients;
                
                    for(unsigned i = 0; i < _server_data.clients_quantity; i ++){
                        string client_address;
                        bool client_status;
                        packet_recv >> client_address;
                        packet_recv >> client_status;
                        _server_data.clients.emplace_back(client_data{client_address, client_status});
                    }
                    status = SUCCESS;
                    cout << "Connected to the Server " << _server_data.address << endl;
                    return;
                }
            }
        }
        elapsed_seconds = chrono::system_clock::now() - start;
        //Waiting for server response for MAX_CONNECTION_TIME
    } while(elapsed_seconds.count() <= MAX_CONNECTION_TIME);

    status = ERROR;
    return;
}

//Disconnect from the game server:
void Client::disconnect_server(request_status& status){
    //Filling send buffer:
    Packet packet_send;
    //client disconnection request:
    packet_send << CLIENT_DISCONNECTION;
    
    //Sending client disconnection: 
    if (socket.send(packet_send, _server_data.address, SERVER_PORT) != sf::Socket::Done)
    {
        cout << "Client: Send error" << endl;
        status = ERROR;
        return;
    }

    _server_data.address = IpAddress::None;
    cout << "Disconnected to the Server " << _server_data.address << endl;
    status = SUCCESS;
}

void Client::listen_server(request_status& status){
    status = NOT_CHANGED;
    socket.setBlocking(false);
    
    Packet packet_recv;;
    IpAddress sender;
    unsigned short port;

    while(true){
        //Check if there is a message:
        if (socket.receive(packet_recv, sender, port) == Socket::Done)
        {
            Packet packet_send;
            datatype _datatype;
            Uint32 datatype_value;
            //Get the buffer information:
            packet_recv >> datatype_value;
            _datatype = (datatype) datatype_value;
            
            switch ((unsigned)_datatype)
            {
            case CLIENT_READY_SUCCESS:
            {    
                vector<client_data>::iterator it = find(_server_data.clients.begin(), _server_data.clients.end(), client_data{IpAddress::getLocalAddress(), false});
                if(it == _server_data.clients.end()){
                    status = READY_ERROR;
                    break;
                }
                    
                (*it).status = true;
            
                cout << "Ready " << endl;
                status = READY_SUCCESS;
                
                break;
            }
            case CLIENT_NOT_READY_SUCCESS:
            {    
                vector<client_data>::iterator it = find(_server_data.clients.begin(), _server_data.clients.end(), client_data{IpAddress::getLocalAddress(), false});
                if(it == _server_data.clients.end()){
                    status = NOT_READY_ERROR;
                    break;
                }
                    
                (*it).status = false;
            
                cout << "Ready " << endl;
                status = NOT_READY_SUCCESS;
                
                break;
            }
            //Check if the message is of new client info:
            case NEW_CLIENT_INFO:
                _server_data.clients.clear();
                for(unsigned i = 0; i < _server_data.clients_quantity; i ++){
                    string client_address;
                    bool client_status;
                    packet_recv >> client_address;
                    packet_recv >> client_status;
                    _server_data.clients.emplace_back(client_data{client_address, client_status});
                }
                status = CHANGED;
            break;
            //Check if the message is of update client info:
            case UPDATE_CLIENT_INFO:
            {    
                unsigned pos_client;
                string client_address;
                bool client_status;
                packet_recv >> pos_client;
                packet_recv >> client_address;
                packet_recv >> client_status;
                _server_data.clients[pos_client].address = client_address;
                _server_data.clients[pos_client].status = client_status;
                cout << "Client update " << _server_data.clients[pos_client].address << _server_data.clients[pos_client].status << endl;
                status = CHANGED;
                break;
            }
            //Check if the message is of delete client info:
            case DELETE_CLIENT_INFO:
                unsigned pos_client;
                packet_recv >> pos_client;
                _server_data.clients.erase(_server_data.clients.begin() + pos_client);
                status = CHANGED;
            break;
            case SERVER_DISCONNECTION:
                _server_data.address = IpAddress::None;
                status = SERVER_DISCONNECTED;
                return;
            case SERVER_GAME_START:
                status = GAME_START;
                return;
            }
            std::cout << "Received bytes from " << sender << " on port " << port << std::endl;
        }
    }
}

void Client::ready(bool is_ready){
    //Filling send buffer:
    Packet packet_send;
    if(is_ready)
        //Client ready:
        packet_send << CLIENT_READY;
    else
        //Client not ready:
        packet_send << CLIENT_NOT_READY;
    
    //Sending ready/not ready client message: 
    if (socket.send(packet_send, _server_data.address, SERVER_PORT) != sf::Socket::Done)
    {
        cout << "Client: Send error" << endl;
    }

}

//Sending board data
void Client::send_board_data(Board& board){
    //Filling send buffer:
    Packet packet_send;
    
    packet_send << CLIENT_GAME_UPDATE;
    packet_send << (Int64)board.get_score();

    unsigned** map = board.get_map();
    for(unsigned i = 0; i < board.get_x_dim(); i ++){
        for(unsigned j = 0; j < board.get_y_dim(); j ++){
            packet_send << (Int32)map[i][j];
        }
    }
    
    //Sending board data message: 
    if (socket.send(packet_send, _server_data.address, SERVER_PORT) != sf::Socket::Done)
    {
        cout << "Client: Send error" << endl;
    }
} 

//Listen the server during the game
void Client::listen_game(request_status& status){
    status = NOT_CHANGED;
    socket.setBlocking(false);
    
    Packet packet_recv;;
    IpAddress sender;
    unsigned short port;

    while(true){
        //Check if there is a message:
        if (socket.receive(packet_recv, sender, port) == Socket::Done){
            //Get the buffer information:
            datatype _datatype;
            Uint32 datatype_value;

            packet_recv >> datatype_value;
            _datatype = (datatype) datatype_value;
            
            switch ((unsigned)_datatype)
            {
                case SERVER_GAME_UPDATE:
                    for(unsigned i = 0; i < _server_data.clients_quantity; i ++){
                        Int64 score;
                        packet_recv >> score;
                        _server_data.clients[i].score = (long)score;
                        
                        for(unsigned j = 0; j < BOARD_GRID_WIDTH; j ++){
                            for(unsigned k = 0; k < BOARD_GRID_HEIGHT + FIGURE_GRID_HEIGHT; k ++){
                                Int32 value;
                                packet_recv >> value;
                                _server_data.clients[i].map[j][k] = (unsigned)value;
                            }
                        }
                    }
                    status = CHANGED;
                    break;
            }
        }
    }
}