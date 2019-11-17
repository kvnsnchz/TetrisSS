#include <iostream>
#include <stdio.h>
#include <vector>
#include "board.hpp"

using namespace std;

// General figure definition:
class Figure {
    protected:
        vector<Point *> points;
    public:
        Figure(const unsigned& y_dim);

        virtual void initialize_figure(const unsigned& y_dim) = 0;

        void step_down(const Board& game_board);
};

// Square definition:
class Figure_O: public Figure {
    public:
        Figure_O(const unsigned& y_dim): Figure(y_dim) {};

        virtual void initialize_figure(const unsigned& y_dim);
};