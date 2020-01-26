#include "client.hpp"

vector<server_data> Client::get_servers() const {
    return servers;
}

//Search for available game servers:
void Client::search_servers(){
    //UDP socket
    UdpSocket socket;
    
    //Port connection: 
    if (socket.bind(CLIENT_PORT) != sf::Socket::Done)
    {
        cout << "Client: Connection error" << endl;
        return;
    }

    //Filling send buffer:
    Packet packet_send;
    //Server information request:
    packet_send << SERVER_INFO_REQUEST;
    
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
            string _datatype;

            //Get the buffer information:
            packet_recv >> _datatype;
            //Check if the message is a response of server information:
            if(!_datatype.compare(SERVER_INFO_RESPONSE)){
                //Adding server information to server list:
                servers.emplace_back(server_data());
                servers.back().address = sender;
                //Get the buffer information:
                packet_recv >> servers.back().name;
                packet_recv >> servers.back().clients_quantity;
                packet_recv >> servers.back().level;
                cout << "Server: " << servers.back().name << " address = " << servers.back().address << " clients_quantity = " << servers.back().clients_quantity << endl;
            }
        }
        elapsed_seconds = chrono::system_clock::now() - start;
        //Waiting for servers information for MAX_SEARCH_TIME
    } while(elapsed_seconds.count() <= MAX_SEARCH_TIME);
    
}

//Connect to a specific game server from the available list:
bool Client::connect_server(const unsigned pos){
    //Checking that the selected server is in the list:
    if(pos > servers.size())
        return false;
    
    //UDP socket
    UdpSocket socket;

    //Port connection: 
    if (socket.bind(CLIENT_PORT) != sf::Socket::Done)
    {
        cout << "Client: Connection error" << endl;
        return false;
    }
    
    //Filling send buffer:
    Packet packet_send;
    //Server connection request:
    packet_send << SERVER_CONN_REQUEST;
    
    //Sending server connection request: 
    if (socket.send(packet_send, servers[pos].address, SERVER_PORT) != sf::Socket::Done)
    {
        cout << "Client: Send error" << endl;
        return false;
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
                string _datatype;
                //Get the buffer information:
                packet_recv >> _datatype;
                
                //Check if the message is a server connection response error:
                if(!_datatype.compare(SERVER_CONN_RESPONSE_ERROR)){
                    //Removing server of the server list:
                    servers.erase(servers.begin() + pos);
                    return false;
                }
                //Check if the message is a server connection response success:
                if(!_datatype.compare(SERVER_CONN_RESPONSE_SUCCESS)){
                    //Setting server address: 
                    server_address = servers[pos].address;
                    cout << "Connected to the Server " << server_address << endl;
                    return true;
                }
            }
        }
        elapsed_seconds = chrono::system_clock::now() - start;
        //Waiting for server response for MAX_CONNECTION_TIME
    } while(elapsed_seconds.count() <= MAX_CONNECTION_TIME);

    return false;
}

//Disconnect from the game server:
bool Client::disconnect_server(){
    //UDP socket
    UdpSocket socket;

    //Port connection: 
    if (socket.bind(CLIENT_PORT) != sf::Socket::Done)
    {
        cout << "Client: Connection error" << endl;
        return false;
    }
    
    //Filling send buffer:
    Packet packet_send;
    //Server connection request:
    packet_send << SERVER_DISCONNECTION;
    
    //Sending server connection request: 
    if (socket.send(packet_send, server_address, SERVER_PORT) != sf::Socket::Done)
    {
        cout << "Client: Send error" << endl;
        return false;
    }

    server_address = IpAddress::None;
    cout << "Disconnected to the Server " << server_address << endl;
    return true;
}