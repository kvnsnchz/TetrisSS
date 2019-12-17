#include <iostream>
#include "figures.hpp"

using namespace std;

class Board {
    private:
        unsigned **map;
        unsigned x_dimension;
        unsigned y_dimension;
        Figure* current_figure;
        Figure* next_figure;

        friend ostream& operator <<(ostream& stream, const Board& board);
    public:
        Board();

        // game board initialization:
        void initialize();
        // new figure creation function:
        Figure* create_figure();
        // adding figure on top of the board:
        void add_figure();
        
        unsigned get_x_dim() const;
        unsigned get_y_dim() const;

        Figure* get_next_figure() const;
        void set_current_figure(Figure* figure);
        void set_next_figure(Figure* figure);

        bool is_empty(const Point& point) const;
        void change_point(const Point& point, const unsigned& new_value = 0);

        bool step_down();
        void step_left();
        void step_right();

        // adding game over condition function:
        bool game_over();

        ~Board();        
};