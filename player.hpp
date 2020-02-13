#ifndef playerHPP
#define playerHPP

#include <iostream>
#include <thread>
#include <chrono>
#include <cstring>
#include <string>
#include "sockets_constants.hpp"

using namespace std;

/**
 * @brief General class that generalize both servers and clients.
 * 
 */
class Player {
    protected:
        // Player's status before the start of the game
        // (ready - true / not ready - false):
        bool player_status;
        // Player's nickname:
        string player_nickname;
        // Player's udp socket:
        UdpSocket socket;
    public:
        Player(): player_status(false), player_nickname("Player") {};
        Player(const string& new_player_nickname): player_status(false), player_nickname(new_player_nickname) {};
        Player(const bool& new_player_status, const string& new_player_nickname): player_status(new_player_status), player_nickname(new_player_nickname) {};

        bool get_player_status() const;
        string get_player_nickname() const;

        void set_player_status(const bool& new_player_status);
        void set_player_nickname(const string& new_player_nickname);

        /**
         * @brief Function to initialize player's udp socket (port number and IP address).
         * 
         */
        virtual void connect_udp_socket() = 0;
        /**
         * @brief Function to unbind player's udp socket (port number and IP address).
         * 
         */
        virtual void disconnect_udp_socket() = 0;
        /**
         * @brief Function to inform other players about changes of current player's readiness.
         * 
         */
        virtual void ready(bool) = 0;
        /**
         * @brief Function that receives other players' game info.
         * 
         * @param game_over 
         * @param status 
         */
        virtual void listen_game(Board& game_over, request_status& status) = 0;
        /**
         * @brief Informs other players about multiplayer pause re / initialization.
         * 
         */
        virtual void pause(bool) = 0;
        /**
         * @brief Informs other players about reach of game over condition.
         * 
         */
        virtual void game_over() = 0;

        ~Player() {};
};

#endif
