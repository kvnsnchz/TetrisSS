#define playerCPP

#include "player.hpp"

bool Player::get_player_status() const {
    return player_status;
};

string Player::get_player_nickname() const {
    return player_nickname;
}

void Player::set_player_status(const bool& new_player_status) {
    player_status = new_player_status;
};

void Player::set_player_nickname(const string& new_player_nickname) {
    player_nickname = new_player_nickname;
};