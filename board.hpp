#include <iostream>
#include <stdio.h>
#include "figures.hpp"

using namespace std;

class Board {
    private:
        unsigned **map;
        unsigned x_dimension;
        unsigned y_dimension;
        Figure* figure; 

        friend ostream& operator <<(ostream& stream, const Board& board);
    public:
        Board();

        void initialize();
        
        unsigned get_x_dim() const;
        unsigned get_y_dim() const;

        bool is_empty(const Point& point) const;
        void change_point(const Point& point, const unsigned& new_value = 0);

        bool step_down();

        ~Board();        
};