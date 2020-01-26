#include "server.hpp"

void Server::search_clients() {
    UdpSocket socket;
    socket.setBlocking(false);
    
    if (socket.bind(SERVER_PORT) != Socket::Done)
    {
        cout << "Server: Connection error" << endl;
        return;
    }

    Packet packet_recv;;
    IpAddress sender;
    unsigned short port;

    while(true){
        if (socket.receive(packet_recv, sender, port) == Socket::Done)
        {
            string _datatype;
            Packet packet_send;

            packet_recv >> _datatype;
            
            if(!_datatype.compare(SERVER_INFO_REQUEST)){
                packet_send << SERVER_INFO_RESPONSE;
                packet_send << (Uint32)clients_address.size();
                
                if (socket.send(packet_send, sender, CLIENT_PORT) != sf::Socket::Done)
                    cout << "Server: Send error" << endl;

                cout << "Client " << _datatype << endl;
            }
            else{
                if(!_datatype.compare(SERVER_CONN_REQUEST)){
                    if(clients_address.size() == MAX_CLIENTS){
                        packet_send << SERVER_CONN_RESPONSE_ERROR;
                    }
                    else{
                        if(find(clients_address.begin(), clients_address.end(), sender) == clients_address.end())
                            clients_address.emplace_back(sender);
                            
                        packet_send << SERVER_CONN_RESPONSE_SUCCESS;
                    }

                    if (socket.send(packet_send, sender, CLIENT_PORT) != sf::Socket::Done)
                        cout << "Server: Send error" << endl;
                    
                    cout << "New client " << sender << endl;
                }
                else{
                    if(!_datatype.compare(SERVER_DISCONNECTION)){
                        vector<IpAddress>::iterator it = find(clients_address.begin(), clients_address.end(), sender);
                        if(it != clients_address.end()){
                            clients_address.erase(it);
                            cout << "Delete client " << sender << endl;
                        }
                    }
                }
            }
            std::cout << "Received bytes from " << sender << " on port " << port << std::endl;
        }
    }
}