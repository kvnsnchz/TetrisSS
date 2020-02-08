#ifndef playerHPP
#define playerHPP

class Player {
    protected:
        // Player's status before the start of the game
        // (ready - true / not ready - false):
        bool player_status;
    public:
        Player(): player_status(false) {};
        Player(const bool& new_player_status): player_status(new_player_status) {};

        bool get_player_status() const;

        void set_player_status(const bool& new_player_status);

        ~Player() {};
};

#endif