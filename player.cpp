#include "player.hpp"

bool Player::get_player_status() const {
    return player_status;
};

void Player::set_player_status(const bool& new_player_status) {
    player_status = new_player_status;
};