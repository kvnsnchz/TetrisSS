#include "figures.hpp"

void Point::set_x(int x_new) {
    x = x_new;
};

void Point::set_y(int y_new) {
    y = y_new;
};

void Point::increment_x(const unsigned& value) {
    x += value;
};

void Point::increment_y(const int& value) {
    y += value;
};

int Point::get_x() const {
    return x;
};

int Point::get_y() const {
    return y;
};

Point::~Point() {};

vector<Point *> Figure::get_points() const{
    return points;
}

unsigned Figure::get_color_code() const{
    return color_code;
}

void Figure_O::initialize(unsigned y_dim){
    points.push_back(new Point(0, y_dim / 2));
    points.push_back(new Point(0, y_dim / 2 + 1));
    points.push_back(new Point(1, y_dim / 2));
    points.push_back(new Point(1, y_dim / 2 + 1));
}
