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
    socket.setBlocking(true);

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
    socket.setBlocking(true);
    //Filling send buffer:
    Packet packet_send;
    //Server connection request:
    packet_send << SERVER_DISCONNECTION;
    
    //Sending server connection request: 
    if (socket.send(packet_send, _server_data.address, SERVER_PORT) != sf::Socket::Done)
    {
        cout << "Client: Send error" << endl;
        status = ERROR;
        return;
    }

    _server_data.address = IpAddress::None;
    cout << "Disconnected to the Server " << _server_data.address << endl;
    status = SUCCESS;
    return;
}

void Client::listen_sever(){
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
                break;
            }
            //Check if the message is of delete client info:
            case DELETE_CLIENT_INFO:
                unsigned pos_client;
                packet_recv >> pos_client;
                _server_data.clients.erase(_server_data.clients.begin() + pos_client);
            break;
            }
            std::cout << "Received bytes from " << sender << " on port " << port << std::endl;
        }
    }
}

void Client::ready(request_status& status){
    socket.setBlocking(true);
    
    //Filling send buffer:
    Packet packet_send;
    //Client ready:
    packet_send << CLIENT_READY;
    
    //Sending ready client message: 
    if (socket.send(packet_send, _server_data.address, SERVER_PORT) != sf::Socket::Done)
    {
        cout << "Client: Send error" << endl;
        status = ERROR;
        return;
    }

    chrono::time_point<chrono::system_clock> start = chrono::system_clock::now();
    chrono::duration<double> elapsed_seconds;
    //Setting non blocking socket:
    socket.setBlocking(false);
    status = NOT_READY;
    do{
        Packet packet_recv;
        IpAddress sender;
        unsigned short port;
        //Check if there is a message:
        if (socket.receive(packet_recv, sender, port) == Socket::Done)
        {
            //Checking that the address of the sender 
            //is the same as that of the selected server
            if(sender == _server_data.address){
                datatype _datatype;
                Uint32 datatype_value;
                //Get the buffer information:
                packet_recv >> datatype_value;
                _datatype = (datatype) datatype_value;
                if(_datatype == CLIENT_READY_SUCCESS){
                    vector<client_data>::iterator it = find(_server_data.clients.begin(), _server_data.clients.end(), client_data{IpAddress::getLocalAddress(), false});
                    if(it == _server_data.clients.end()){
                        status = ERROR;
                        break;
                    }
                        
                    (*it).status = true;
                
                    cout << "Ready " << endl;
                    status = SUCCESS;
                    return;
                }
            }
        }
        elapsed_seconds = chrono::system_clock::now() - start;
        //Waiting for server response for MAX_RESPONSE_TIME
    } while(elapsed_seconds.count() <= MAX_RESPONSE_TIME);

    status = ERROR;
}