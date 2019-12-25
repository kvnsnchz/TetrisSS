#include "figures.hpp"

Vector2f figureCellSize(40.0f, 40.0f);

void Point::set_x(unsigned x_new) {
    x = x_new;
};

void Point::set_y(unsigned y_new) {
    y = y_new;
};

void Point::increment_x(const unsigned& value) {
    x += value;
};

void Point::increment_y(const unsigned& value) {
    y += value;
};

unsigned Point::get_x() const {
    return x;
};

unsigned Point::get_y() const {
    return y;
};

Point::~Point() {};

vector<Point *> Figure::get_points() const{
    return points;
}

unsigned Figure::get_color_code() const{
    return color_code;
}

RectangleShape** Figure::get_grid() const {
    RectangleShape** result = new RectangleShape *[4];
    for (unsigned i = 0; i < 4; i++) {
        result[i] = new RectangleShape [2];
    }

    for (unsigned i = 0; i < 4; i++) 
        for (unsigned j = 0; j < 2; j++)
            result[i][j] = grid[i][j];

    return result;
}

// Square figure initialization:
void Figure_O::initialize(unsigned x_dim) {
    points.push_back(new Point(x_dim / 2 - 1, 0));
    points.push_back(new Point(x_dim / 2, 0));
    points.push_back(new Point(x_dim / 2 - 1, 1));
    points.push_back(new Point(x_dim / 2, 1));

    for (unsigned i = 0; i < 4; i++)
        for (unsigned j = 0; j < 2; j++) {
            grid[i][j].setSize(figureCellSize);
            grid[i][j].setOutlineColor(sf::Color::Black);
            grid[i][j].setOutlineThickness(1.0f);
            grid[i][j].setPosition(x_dim * (figureCellSize.x + 1) + (i * figureCellSize.x) + 5.0f, ((j + 2) * figureCellSize.y) + 5.0f);
            grid[i][j].setFillColor(Color::Black);
        };

    grid[0][0].setFillColor(Color::Yellow);
    grid[0][1].setFillColor(Color::Yellow);
    grid[1][0].setFillColor(Color::Yellow);
    grid[1][1].setFillColor(Color::Yellow);  
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

    for (unsigned i = 0; i < 4; i++)
        for (unsigned j = 0; j < 2; j++) {
            grid[i][j].setSize(figureCellSize);
            grid[i][j].setOutlineColor(sf::Color::Black);
            grid[i][j].setOutlineThickness(1.0f);
            grid[i][j].setPosition(x_dim * (figureCellSize.x + 1) + (i * figureCellSize.x) + 5.0f , ((j + 2) * figureCellSize.y) + 5.0f);
            grid[i][j].setFillColor(Color::Black);
        };

    grid[0][0].setFillColor(Color::Cyan);
    grid[1][0].setFillColor(Color::Cyan);
    grid[2][0].setFillColor(Color::Cyan);
    grid[3][0].setFillColor(Color::Cyan); 
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

    for (unsigned i = 0; i < 4; i++)
        for (unsigned j = 0; j < 2; j++) {
            grid[i][j].setSize(figureCellSize);
            grid[i][j].setOutlineColor(sf::Color::Black);
            grid[i][j].setOutlineThickness(1.0f);
            grid[i][j].setPosition(x_dim * (figureCellSize.x + 1) + (i * figureCellSize.x) + 5.0f , ((j + 2) * figureCellSize.y) + 5.0f);
            grid[i][j].setFillColor(Color::Black);
        };

    grid[1][0].setFillColor(Color::Magenta);
    grid[0][1].setFillColor(Color::Magenta);
    grid[1][1].setFillColor(Color::Magenta);
    grid[2][1].setFillColor(Color::Magenta); 
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

    for (unsigned i = 0; i < 4; i++)
        for (unsigned j = 0; j < 2; j++) {
            grid[i][j].setSize(figureCellSize);
            grid[i][j].setOutlineColor(sf::Color::Black);
            grid[i][j].setOutlineThickness(1.0f);
            grid[i][j].setPosition(x_dim * (figureCellSize.x + 1) + (i * figureCellSize.x) + 5.0f , ((j + 2) * figureCellSize.y) + 5.0f);
            grid[i][j].setFillColor(Color::Black);
        };

    grid[2][0].setFillColor(Color::White);
    grid[0][1].setFillColor(Color::White);
    grid[1][1].setFillColor(Color::White);
    grid[2][1].setFillColor(Color::White); 
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

    for (unsigned i = 0; i < 4; i++)
        for (unsigned j = 0; j < 2; j++) {
            grid[i][j].setSize(figureCellSize);
            grid[i][j].setOutlineColor(sf::Color::Black);
            grid[i][j].setOutlineThickness(1.0f);
            grid[i][j].setPosition(x_dim * (figureCellSize.x + 1) + (i * figureCellSize.x) + 5.0f , ((j + 2) * figureCellSize.y) + 5.0f);
            grid[i][j].setFillColor(Color::Black);
        };

    grid[0][0].setFillColor(Color::Blue);
    grid[0][1].setFillColor(Color::Blue);
    grid[1][1].setFillColor(Color::Blue);
    grid[2][1].setFillColor(Color::Blue); 
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

    for (unsigned i = 0; i < 4; i++)
        for (unsigned j = 0; j < 2; j++) {
            grid[i][j].setSize(figureCellSize);
            grid[i][j].setOutlineColor(sf::Color::Black);
            grid[i][j].setOutlineThickness(1.0f);
            grid[i][j].setPosition(x_dim * (figureCellSize.x + 1) + (i * figureCellSize.x) + 5.0f , ((j + 2) * figureCellSize.y) + 5.0f);
            grid[i][j].setFillColor(Color::Black);
        };

    grid[0][0].setFillColor(Color::Green);
    grid[1][0].setFillColor(Color::Green);
    grid[1][1].setFillColor(Color::Green);
    grid[2][1].setFillColor(Color::Green); 
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

    for (unsigned i = 0; i < 4; i++)
        for (unsigned j = 0; j < 2; j++) {
            grid[i][j].setSize(figureCellSize);
            grid[i][j].setOutlineColor(sf::Color::Black);
            grid[i][j].setOutlineThickness(1.0f);
            grid[i][j].setPosition(x_dim * (figureCellSize.x + 1) + (i * figureCellSize.x) + 5.0f , ((j + 2) * figureCellSize.y) + 5.0f);
            grid[i][j].setFillColor(Color::Black);
        };

    grid[1][0].setFillColor(Color::Red);
    grid[2][0].setFillColor(Color::Red);
    grid[0][1].setFillColor(Color::Red);
    grid[1][1].setFillColor(Color::Red);     
}

void Figure_S::print() {
    cout << "   " << color_code << " " << color_code << " \n";
    cout << " " << color_code << " " << color_code << "   " << endl;
};
