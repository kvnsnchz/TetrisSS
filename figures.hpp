#include <iostream>
#include <stdio.h>
#include <vector>
#include "board.hpp"

using namespace std;

// General figure definition:
class Figure {
    protected:
        vector<Point *> points;
        unsigned color_code;
    public:
        Figure(const unsigned& y_dim, const unsigned& code);

        virtual void initialize_figure(const unsigned& y_dim) = 0;

        void step_down(Board& game_board);
};

// Square definition:
class Figure_O: public Figure {
    public:
        Figure_O(const unsigned& y_dim): Figure(y_dim, 1) {};

        virtual void initialize_figure(const unsigned& y_dim);
};