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
    // fill the vector with the points of specific current figure`s coordinates:
    points.push_back(new Point(x_dim / 2 - 1, 0));
    points.push_back(new Point(x_dim / 2, 0));
    points.push_back(new Point(x_dim / 2 - 1, 1));
    points.push_back(new Point(x_dim / 2, 1));

    // initialize the figure`s grid:
    // set each cell`s size, position 
    // and initial color (same as background):
    for (unsigned i = 0; i < 4; i++)
        for (unsigned j = 0; j < 2; j++) {
            grid[i][j].setSize(figureCellSize);
            grid[i][j].setPosition(x_dim * (figureCellSize.x + 1) + (i * figureCellSize.x) + (x_dim + 1) * 5.0f + (i + 1) * 5.0f, ((j + 2) * figureCellSize.y) + (j + 2) * 5.0f);
            grid[i][j].setFillColor(Color(255, 255, 255, 0));
        };

    // set the figure`s own color according to the color code for certain cells:
    grid[0][0].setFillColor(Color(255, 216, 0, 255));
    grid[0][1].setFillColor(Color(255, 216, 0, 255));
    grid[1][0].setFillColor(Color(255, 216, 0, 255));
    grid[1][1].setFillColor(Color(255, 216, 0, 255));  
}

// Stick figure initialization:
void Figure_I::initialize(unsigned x_dim) {
    // fill the vector with the points of specific current figure`s coordinates:
    points.push_back(new Point(x_dim / 2 - 2, 1));
    points.push_back(new Point(x_dim / 2 - 1, 1));  
    points.push_back(new Point(x_dim / 2, 1));
    points.push_back(new Point(x_dim / 2 + 1, 1));

    // initialize the figure`s grid:
    // set each cell`s size, position 
    // and initial color (same as background):
    for (unsigned i = 0; i < 4; i++)
        for (unsigned j = 0; j < 2; j++) {
            grid[i][j].setSize(figureCellSize);
            grid[i][j].setPosition(x_dim * (figureCellSize.x + 1) + (i * figureCellSize.x) + (x_dim + 1) * 5.0f + (i + 1) * 5.0f, ((j + 2) * figureCellSize.y) + (j + 2) * 5.0f);
            grid[i][j].setFillColor(Color(255, 255, 255, 0));
        };

    // set the figure`s own color according to the color code for certain cells:
    grid[0][0].setFillColor(Color(28, 230, 199, 255));
    grid[1][0].setFillColor(Color(28, 230, 199, 255));
    grid[2][0].setFillColor(Color(28, 230, 199, 255));
    grid[3][0].setFillColor(Color(28, 230, 199, 255)); 
}

// T figure initialization:
void Figure_T::initialize(unsigned x_dim) {
    // fill the vector with the points of specific current figure`s coordinates:
    points.push_back(new Point(x_dim / 2 - 1, 0));
    points.push_back(new Point(x_dim / 2 - 2, 1));
    points.push_back(new Point(x_dim / 2 - 1, 1));
    points.push_back(new Point(x_dim / 2, 1));

    // initialize the figure`s grid:
    // set each cell`s size, position 
    // and initial color (same as background):
    for (unsigned i = 0; i < 4; i++)
        for (unsigned j = 0; j < 2; j++) {
            grid[i][j].setSize(figureCellSize);
            grid[i][j].setPosition(x_dim * (figureCellSize.x + 1) + (i * figureCellSize.x) + (x_dim + 1) * 5.0f + (i + 1) * 5.0f, ((j + 2) * figureCellSize.y) + (j + 2) * 5.0f);
            grid[i][j].setFillColor(Color(255, 255, 255, 0));
        };

    // set the figure`s own color according to the color code for certain cells:
    grid[1][0].setFillColor(Color(248, 131, 6, 255));
    grid[0][1].setFillColor(Color(248, 131, 6, 255));
    grid[1][1].setFillColor(Color(248, 131, 6, 255));
    grid[2][1].setFillColor(Color(248, 131, 6, 255)); 
}

// L figure initialization:
void Figure_L::initialize(unsigned x_dim) {
    // fill the vector with the points of specific current figure`s coordinates:
    points.push_back(new Point(x_dim / 2, 0));
    points.push_back(new Point(x_dim / 2 - 2, 1));
    points.push_back(new Point(x_dim / 2 - 1, 1));
    points.push_back(new Point(x_dim / 2, 1));

    // initialize the figure`s grid:
    // set each cell`s size, position 
    // and initial color (same as background):
    for (unsigned i = 0; i < 4; i++)
        for (unsigned j = 0; j < 2; j++) {
            grid[i][j].setSize(figureCellSize);
            grid[i][j].setPosition(x_dim * (figureCellSize.x + 1) + (i * figureCellSize.x) + (x_dim + 1) * 5.0f + (i + 1) * 5.0f, ((j + 2) * figureCellSize.y) + (j + 2) * 5.0f);
            grid[i][j].setFillColor(Color(255, 255, 255, 0));
        };

    // set the figure`s own color according to the color code for certain cells:
    grid[2][0].setFillColor(Color(248, 6, 248, 255));
    grid[0][1].setFillColor(Color(248, 6, 248, 255));
    grid[1][1].setFillColor(Color(248, 6, 248, 255));
    grid[2][1].setFillColor(Color(248, 6, 248, 255)); 
}

// J figure initialization:
void Figure_J::initialize(unsigned x_dim) {
    // fill the vector with the points of specific current figure`s coordinates:
    points.push_back(new Point(x_dim / 2 - 2, 0));
    points.push_back(new Point(x_dim / 2 - 2, 1));
    points.push_back(new Point(x_dim / 2 - 1, 1));
    points.push_back(new Point(x_dim / 2, 1));

    // initialize the figure`s grid:
    // set each cell`s size, position 
    // and initial color (same as background):
    for (unsigned i = 0; i < 4; i++)
        for (unsigned j = 0; j < 2; j++) {
            grid[i][j].setSize(figureCellSize);
            grid[i][j].setPosition(x_dim * (figureCellSize.x + 1) + (i * figureCellSize.x) + (x_dim + 1) * 5.0f + (i + 1) * 5.0f, ((j + 2) * figureCellSize.y) + (j + 2) * 5.0f);
            grid[i][j].setFillColor(Color(255, 255, 255, 0));
        };

    // set the figure`s own color according to the color code for certain cells:
    grid[0][0].setFillColor(Color(134, 33, 255, 255));
    grid[0][1].setFillColor(Color(134, 33, 255, 255));
    grid[1][1].setFillColor(Color(134, 33, 255, 255));
    grid[2][1].setFillColor(Color(134, 33, 255, 255)); 
}

// Z figure initialization:
void Figure_Z::initialize(unsigned x_dim) {
    // fill the vector with the points of specific current figure`s coordinates:
    points.push_back(new Point(x_dim / 2 - 2, 0));
    points.push_back(new Point(x_dim / 2 - 1, 0));
    points.push_back(new Point(x_dim / 2 - 1, 1));
    points.push_back(new Point(x_dim / 2, 1));

    // initialize the figure`s grid:
    // set each cell`s size, position 
    // and initial color (same as background):
    for (unsigned i = 0; i < 4; i++)
        for (unsigned j = 0; j < 2; j++) {
            grid[i][j].setSize(figureCellSize);
            grid[i][j].setPosition(x_dim * (figureCellSize.x + 1) + (i * figureCellSize.x) + (x_dim + 1) * 5.0f + (i + 1) * 5.0f, ((j + 2) * figureCellSize.y) + (j + 2) * 5.0f);
            grid[i][j].setFillColor(Color(255, 255, 255, 0));
        };

    // set the figure`s own color according to the color code for certain cells:
    grid[0][0].setFillColor(Color(220, 0, 20, 255));
    grid[1][0].setFillColor(Color(220, 0, 20, 255));
    grid[1][1].setFillColor(Color(220, 0, 20, 255));
    grid[2][1].setFillColor(Color(220, 0, 20, 255)); 
}

// S figure initialization:
void Figure_S::initialize(unsigned x_dim) {
    // fill the vector with the points of specific current figure`s coordinates:
    points.push_back(new Point(x_dim / 2 - 1, 0));
    points.push_back(new Point(x_dim / 2, 0));
    points.push_back(new Point(x_dim / 2 - 2, 1));
    points.push_back(new Point(x_dim / 2 - 1, 1));

    // initialize the figure`s grid:
    // set each cell`s size, position 
    // and initial color (same as background):
    for (unsigned i = 0; i < 4; i++)
        for (unsigned j = 0; j < 2; j++) {
            grid[i][j].setSize(figureCellSize);
            grid[i][j].setPosition(x_dim * (figureCellSize.x + 1) + (i * figureCellSize.x) + (x_dim + 1) * 5.0f + (i + 1) * 5.0f, ((j + 2) * figureCellSize.y) + (j + 2) * 5.0f);
            grid[i][j].setFillColor(Color(255, 255, 255, 0));
        };

    // set the figure`s own color according to the color code for certain cells:
    grid[1][0].setFillColor(Color(46, 228, 25, 255));
    grid[2][0].setFillColor(Color(46, 228, 25, 255));
    grid[0][1].setFillColor(Color(46, 228, 25, 255));
    grid[1][1].setFillColor(Color(46, 228, 25, 255));     
}