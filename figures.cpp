#include "figures.hpp"

void Point::set_x(int x_new) {
    x = x_new;
};

void Point::set_y(int y_new) {
    y = y_new;
};

void Point::increment_x(const int& value) {
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

// Square figure initialization:
void Figure_O::initialize(unsigned x_dim) {
    points.push_back(new Point(x_dim / 2 - 1, 0));
    points.push_back(new Point(x_dim / 2, 0));
    points.push_back(new Point(x_dim / 2 - 1, 1));
    points.push_back(new Point(x_dim / 2, 1));
}

void Figure_O::print() {
    cout << " " << color_code << " " << color_code << " \n";
    cout << " " << color_code << " " << color_code << " " << endl;
};

// Stick figure initialization:
void Figure_I::initialize(unsigned x_dim) {
    points.push_back(new Point(x_dim / 2 - 2, 1));
    points.push_back(new Point(x_dim / 2 - 1, 1));
    points.push_back(new Point(x_dim / 2, 1));
    points.push_back(new Point(x_dim / 2 + 1, 1));
}

void Figure_I::print() {
    cout << " " << color_code << " " << color_code << " " << color_code << " " << color_code << " " << endl;
};

// T figure initialization:
void Figure_T::initialize(unsigned x_dim) {
    points.push_back(new Point(x_dim / 2 - 1, 0));
    points.push_back(new Point(x_dim / 2 - 2, 1));
    points.push_back(new Point(x_dim / 2 - 1, 1));
    points.push_back(new Point(x_dim / 2, 1));
}

void Figure_T::print() {
    cout << "   " << color_code << "   \n";
    cout << " " << color_code << " " << color_code << " " << color_code << " " << endl;
};

// L figure initialization:
void Figure_L::initialize(unsigned x_dim) {
    points.push_back(new Point(x_dim / 2, 0));
    points.push_back(new Point(x_dim / 2 - 2, 1));
    points.push_back(new Point(x_dim / 2 - 1, 1));
    points.push_back(new Point(x_dim / 2, 1));
}

void Figure_L::print() {
    cout << "     " << color_code << " \n";
    cout << " " << color_code << " " << color_code << " " << color_code << " " << endl;
};

// J figure initialization:
void Figure_J::initialize(unsigned x_dim) {
    points.push_back(new Point(x_dim / 2 - 2, 0));
    points.push_back(new Point(x_dim / 2 - 2, 1));
    points.push_back(new Point(x_dim / 2 - 1, 1));
    points.push_back(new Point(x_dim / 2, 1));
}

void Figure_J::print() {
    cout << " " << color_code << "      \n";
    cout << " " << color_code << " " << color_code << " " << color_code << " " << endl;
};

// Z figure initialization:
void Figure_Z::initialize(unsigned x_dim) {
    points.push_back(new Point(x_dim / 2 - 2, 0));
    points.push_back(new Point(x_dim / 2 - 1, 0));
    points.push_back(new Point(x_dim / 2 - 1, 1));
    points.push_back(new Point(x_dim / 2, 1));
}

void Figure_Z::print() {
    cout << " " << color_code << " " << color_code << "   \n";
    cout << "   " << color_code << " " << color_code << " " << endl;
};

// S figure initialization:
void Figure_S::initialize(unsigned x_dim) {
    points.push_back(new Point(x_dim / 2 - 1, 0));
    points.push_back(new Point(x_dim / 2, 0));
    points.push_back(new Point(x_dim / 2 - 2, 1));
    points.push_back(new Point(x_dim / 2 - 1, 1));
}

void Figure_S::print() {
    cout << "   " << color_code << " " << color_code << " \n";
    cout << " " << color_code << " " << color_code << "   " << endl;
};
