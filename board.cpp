#include "board.hpp"
#include <iostream>
using namespace std;
Board::Board() {
    initialize();
};

void Board::initialize(){
    x_dimension = 20;
    y_dimension = 10;

    map = new unsigned *[x_dimension];
    for (int i = 0; i < x_dimension; i++)
        map[i] = new unsigned [y_dimension];

    for (int i = 0; i < x_dimension; i++)
        for (int j = 0; j < y_dimension; j++)
            map[i][j] = 0;

    figure = new Figure_O(y_dimension);

    for (int i = 0; i < figure->get_points().size(); i++) 
        change_point(*figure->get_points()[i], figure->get_color_code());
}

unsigned Board::get_x_dim() const {
    return x_dimension;
};

unsigned Board::get_y_dim() const {
    return y_dimension;
};

bool Board::is_empty(const Point& point) const {
    if (map[point.get_x()][point.get_y()] == 0 || map[point.get_x()][point.get_y()] == figure->get_color_code())
        return true;
    else 
        return false;
};

void Board::change_point(const Point& point, const unsigned& new_value) {
    map[point.get_x()][point.get_y()] = new_value;
};

bool Board::step_down() {
    bool is_free = true;
    
    for (int i = 0; i < figure->get_points().size(); i++) {
        if(figure->get_points()[i]->get_x() + 1 >= x_dimension){
            is_free = false;
            break;
        }

        if (!is_empty(Point(figure->get_points()[i]->get_x() + 1, figure->get_points()[i]->get_y()))) {
            is_free = false;
            break;
        };
    };

    int factor = 10;
    if(is_free) {
        factor = 1;
        for (int i = 0; i < figure->get_points().size(); i++) {
            change_point(*figure->get_points()[i]);
            figure->get_points()[i]->increment_x();
        }
    }

    for (int i = 0; i < figure->get_points().size(); i++)
        change_point(*figure->get_points()[i], figure->get_color_code() * factor);
        
    return is_free;
};

Board::~Board() {};

ostream& operator <<(ostream& stream, const Board& board) {
    for (int i = 0; i < board.x_dimension; i++) {
        stream << "| ";
        for (int j = 0; j < board.y_dimension; j++) {
            if(board.map[i][j] < 10)
                stream << " " << board.map[i][j] << " "; 
            else
                stream << board.map[i][j] << " "; 
            
        };
        stream << " |\n";
    };

    return stream;
};