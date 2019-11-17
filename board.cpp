#include "board.hpp"

void Point::set_x(int x_new) {
    x = x_new;
};

void Point::set_y(int y_new) {
    y = y_new;
};

void Point::increment_x(const unsigned& value = 1) {
    x += value;
};

void Point::increment_y(const int& value = 1) {
    y += value;
};

int Point::get_x() const {
    return x;
};

int Point::get_y() const {
    return y;
};

Point::~Point() {};

Board::Board() {
    x_dimension = 20;
    y_dimension = 10;

    map = new unsigned *[x_dimension];
    for (int i = 0; i < x_dimension; i++)
        map[i] = new unsigned [y_dimension];

    for (int i = 0; i < x_dimension; i++)
        for (int j = 0; j < y_dimension; j++)
            map[i][j] = 0;
};

unsigned Board::get_x_dim() const {
    return x_dimension;
};

unsigned Board::get_y_dim() const {
    return y_dimension;
};

bool Board::is_empty(const Point& point) const {
    if (map[point.get_x()][point.get_y()] == 0)
        return true;
    else 
        return false
};

void Board::change_point(const Point& point, const unsigned& new_value = 0) {
    map[point.get_x()][point.get_y()] = new_value;
};

Board::~Board() {};

ostream& operator <<(ostream& stream, const Board& board) {
    for (int i = 0; i < board.x_dimension; i++) {
        stream << "| ";
        for (int j = 0; j < board.y_dimension; j++) {
            stream << board.map[i][j]; 
        };
        stream << " |\n";
    };

    return stream;
};