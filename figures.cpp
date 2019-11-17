#include "figures.hpp"

Figure::Figure(const unsigned& y_dim, const unsigned& code) : color_code(code) {
    initialize_figure(y_dim);
};

void Figure::step_down(Board& game_board) {
    bool is_free = true;

    for (int i = 0; i < points.size(); i++) {
        if (!game_board.is_empty(Point(points[i]->get_x() + 1, points[i]->get_y()))) {
            is_free = false;
            break;
        };
    };

    if(is_free) {
        for (int i = 0; i < points.size(); i++) {
            game_board.change_point(*points[i]);
            points[i]->increment_x();
            game_board.change_point(*points[i], color_code);
        }
    }

};

void Figure_O::initialize_figure(const unsigned& y_dim) {
    points.push_back(new Point(0, y_dim / 2));
    points.push_back(new Point(0, y_dim / 2 + 1));
    points.push_back(new Point(1, y_dim / 2));
    points.push_back(new Point(1, y_dim / 2 + 1));
};