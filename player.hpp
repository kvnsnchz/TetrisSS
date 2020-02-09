#ifndef playerHPP
#define playerHPP

#include <iostream>
#include <thread>
#include <chrono>
#include <cstring>
#include <boost/algorithm/string.hpp>
#include <string>
#include "sockets_constants.hpp"

using namespace std;

class Player {
    protected:
        // Player's status before the start of the game
        // (ready - true / not ready - false):
        bool player_status;
        //Player's nickname:
        string player_nickname;
    public:
        Player(): player_status(false), player_nickname("Player") {};
        Player(const string& new_player_nickname): player_status(false), player_nickname(new_player_nickname) {};
        Player(const bool& new_player_status, const string& new_player_nickname): player_status(new_player_status), player_nickname(new_player_nickname) {};

        bool get_player_status() const;
        string get_player_nickname() const;

        void set_player_status(const bool& new_player_status);
        void set_player_nickname(const string& new_player_nickname);

        virtual void connect_udp_socket() = 0;
        virtual void disconnect_udp_socket() = 0;
        virtual void ready(bool) = 0;
        virtual void listen_game(request_status& status) = 0;
        virtual void pause(bool) = 0;

        ~Player() {};
};

#endif
