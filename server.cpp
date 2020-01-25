#include "server.hpp"

void Server::search_clients() {
    thread thread_socket([&] () {
        UdpSocket socket;
        socket.setBlocking(false);
        
        if (socket.bind(SERVER_PORT) != Socket::Done)
        {
            cout << "Server: Connection error" << endl;
        }

        vector<string> data(1);
        IpAddress sender;
        size_t received;

        while(true){
            if (socket.receive(data.data(), 1, received, sender, port) == Socket::Done)
            {
            std::cout << "Received " << received << " bytes from " << sender << " on port " << port << std::endl;
            }
        }
    });
    thread_socket.join();
}