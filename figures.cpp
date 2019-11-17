#include "figures.hpp"

Figure::Figure(const unsigned& y_dim) {
    initialize_figure(y_dim);
};

void Figure::step_down(const Board& game_board) {
    // int *y;
    // for (unsigned i = 0; i < 4; i++) {
    //     if (game_board.is_empty[points[i]->][])
    // }

};

void Figure_O::initialize_figure(const unsigned& y_dim) {
    points.push_back(new Point(0, y_dim / 2));
    points.push_back(new Point(0, y_dim / 2 + 1));
    points.push_back(new Point(1, y_dim / 2));
    points.push_back(new Point(1, y_dim / 2 + 1));
};