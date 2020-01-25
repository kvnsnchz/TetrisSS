#include "client.hpp"

void Client::search_servers(){
    thread thread_socket([&] () {
        UdpSocket socket;

        if (socket.bind(CLIENT_PORT) != sf::Socket::Done)
        {
            cout << "Client: Connection error" << endl;
        }

        vector<string> data;
        data.emplace_back(REQUEST_CONNECTION);

        if (socket.send(data.data(), data.size(), IpAddress::Broadcast, SERVER_PORT) != sf::Socket::Done)
        {
            cout << "Client: Send error" << endl;
        }

    /* vector<string>(2).swap(data);
        IpAddress sender;
        size_t received;

        if (socket.receive(data.data(), 2, received, sender, port) != sf::Socket::Done)
        {
            cout << "Client: Receive error" << endl;
        }
        std::cout << "Received " << received << " bytes from " << sender << " on port " << port << std::endl;
        */
    });
    thread_socket.join();
}