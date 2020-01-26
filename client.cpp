#include "client.hpp"

vector<server_t> Client::get_servers() const {
    return servers;
}

void Client::search_servers(){
    UdpSocket socket;

    if (socket.bind(CLIENT_PORT) != sf::Socket::Done)
    {
        cout << "Client: Connection error" << endl;
        return;
    }
    
    Packet packet_send;
    packet_send << SERVER_INFO_REQUEST;
    
    if (socket.send(packet_send, IpAddress::Broadcast, SERVER_PORT) != sf::Socket::Done)
    {
        cout << "Client: Send error" << endl;
        return;
    }

    chrono::time_point<chrono::system_clock> start = chrono::system_clock::now();
    chrono::duration<double> elapsed_seconds;
    socket.setBlocking(false);
    
    do{
        Packet packet_recv;
        IpAddress sender;
        unsigned short port;
        
        if (socket.receive(packet_recv, sender, port) == Socket::Done)
        {
            string _datatype;
            
            packet_recv >> _datatype;
            
            if(!_datatype.compare(SERVER_INFO_RESPONSE)){
                servers.emplace_back(server_t());
                servers.back().address = sender;
                packet_recv >> servers.back().clients_quantity;
                cout << "Server: address = " << servers.back().address << " clients_quantity = " << servers.back().clients_quantity << endl;
            }
        }
        elapsed_seconds = chrono::system_clock::now() - start;
    } while(elapsed_seconds.count() <= MAX_SEARCH_TIME);
    
}


bool Client::connect_server(const unsigned pos){
    if(pos > servers.size())
        return false;
    
    UdpSocket socket;

    if (socket.bind(CLIENT_PORT) != sf::Socket::Done)
    {
        cout << "Client: Connection error" << endl;
        return false;
    }
    
    Packet packet_send;
    packet_send << SERVER_CONN_REQUEST;
    
    if (socket.send(packet_send, servers[pos].address, SERVER_PORT) != sf::Socket::Done)
    {
        cout << "Client: Send error" << endl;
        return false;
    }

    chrono::time_point<chrono::system_clock> start = chrono::system_clock::now();
    chrono::duration<double> elapsed_seconds;
    socket.setBlocking(false);
    
    do{
        Packet packet_recv;
        IpAddress sender;
        unsigned short port;
        
        if (socket.receive(packet_recv, sender, port) == Socket::Done)
        {
            if(sender == servers[pos].address){
                string _datatype;
                
                packet_recv >> _datatype;
                
                if(!_datatype.compare(SERVER_CONN_RESPONSE_ERROR)){
                    servers.erase(servers.begin() + pos);
                    return false;
                }
                if(!_datatype.compare(SERVER_CONN_RESPONSE_SUCCESS)){
                    server_address = servers[pos].address;
                    cout << "Connected to the Server " << server_address << endl;
                    return true;
                }
            }
        }
        elapsed_seconds = chrono::system_clock::now() - start;
    } while(elapsed_seconds.count() <= MAX_CONNECTION_TIME);

    return false;
}

bool Client::disconnect_server(){
    UdpSocket socket;

    if (socket.bind(CLIENT_PORT) != sf::Socket::Done)
    {
        cout << "Client: Connection error" << endl;
        return false;
    }
    
    Packet packet_send;
    packet_send << SERVER_DISCONNECTION;
    
    if (socket.send(packet_send, server_address, SERVER_PORT) != sf::Socket::Done)
    {
        cout << "Client: Send error" << endl;
        return false;
    }

    server_address = IpAddress::None;
    cout << "Disconnected to the Server " << server_address << endl;
    return true;
}