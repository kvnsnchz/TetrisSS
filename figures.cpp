#include "figures.hpp"

// Vector2f cell_size(40.0f, 40.0f);

void Point::set_x(unsigned& x_new) {
    x = x_new;
};

void Point::set_y(unsigned& y_new) {
    y = y_new;
};

void Point::increment_x(const int& value) {
    x += value;
};

void Point::increment_y(const int& value) {
    y += value;
};

void Point::rotate(const bool& right, const Point* reference){
    if(reference == NULL){
        return;
    }

    int old_x;

    if(right){
        old_x = x;
        x = (y - reference->get_y()) * (-1) + reference->get_x();
        y = (old_x - reference->get_x()) + reference->get_y();
        return;
    }

    old_x = x;
    x = (y - reference->get_y()) + reference->get_x();
    y = (old_x - reference->get_x()) * (-1) + reference->get_y();
}

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

Point* Figure::get_point_reference() const{
    return point_reference;
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
void Figure_O::initialize(const unsigned& x_dim, const Vector2f& cell_size) {
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
            grid[i][j].setSize(cell_size);
            grid[i][j].setPosition(x_dim * (cell_size.x + 1) + (i * cell_size.x) + i + x_dim + 1 + 5.0f, j * cell_size.y + j + 1 + 85.0f);
            grid[i][j].setFillColor(Color(255, 255, 255, 0));
        };

    // set the figure`s own color according to the color code for certain cells:
    grid[0][0].setFillColor(Color(255, 216, 0, 255));
    grid[0][1].setFillColor(Color(255, 216, 0, 255));
    grid[1][0].setFillColor(Color(255, 216, 0, 255));
    grid[1][1].setFillColor(Color(255, 216, 0, 255));  
}

// Stick figure initialization:
void Figure_I::initialize(const unsigned& x_dim, const Vector2f& cell_size) {
    // fill the vector with the points of specific current figure`s coordinates:
    point_reference = new Point(x_dim / 2, 1);
    points.push_back(new Point(x_dim / 2 - 2, 1));
    points.push_back(new Point(x_dim / 2 - 1, 1));  
    points.push_back(new Point(x_dim / 2 + 1, 1));
    points.push_back(point_reference);

    // initialize the figure`s grid:
    // set each cell`s size, position 
    // and initial color (same as background):
    for (unsigned i = 0; i < 4; i++)
        for (unsigned j = 0; j < 2; j++) {
            grid[i][j].setSize(cell_size);
            grid[i][j].setPosition(x_dim * (cell_size.x + 1) + (i * cell_size.x) + i + x_dim + 1 + 5.0f, j * cell_size.y + j + 1 + 85.0f);
            grid[i][j].setFillColor(Color(255, 255, 255, 0));
        };

    // set the figure`s own color according to the color code for certain cells:
    grid[0][0].setFillColor(Color(28, 230, 199, 255));
    grid[1][0].setFillColor(Color(28, 230, 199, 255));
    grid[2][0].setFillColor(Color(28, 230, 199, 255));
    grid[3][0].setFillColor(Color(28, 230, 199, 255)); 
}

// T figure initialization:
void Figure_T::initialize(const unsigned& x_dim, const Vector2f& cell_size) {
    // fill the vector with the points of specific current figure`s coordinates:
    point_reference = new Point(x_dim / 2 - 1, 1);
    points.push_back(new Point(x_dim / 2 - 1, 0));
    points.push_back(new Point(x_dim / 2 - 2, 1));
    points.push_back(new Point(x_dim / 2, 1));
    points.push_back(point_reference);

    // initialize the figure`s grid:
    // set each cell`s size, position 
    // and initial color (same as background):
    for (unsigned i = 0; i < 4; i++)
        for (unsigned j = 0; j < 2; j++) {
            grid[i][j].setSize(cell_size);
            grid[i][j].setPosition(x_dim * (cell_size.x + 1) + (i * cell_size.x) + i + x_dim + 1 + 5.0f, j * cell_size.y + j + 1 + 85.0f);
            grid[i][j].setFillColor(Color(255, 255, 255, 0));
        };

    // set the figure`s own color according to the color code for certain cells:
    grid[1][0].setFillColor(Color(248, 131, 6, 255));
    grid[0][1].setFillColor(Color(248, 131, 6, 255));
    grid[1][1].setFillColor(Color(248, 131, 6, 255));
    grid[2][1].setFillColor(Color(248, 131, 6, 255)); 
}

// L figure initialization:
void Figure_L::initialize(const unsigned& x_dim, const Vector2f& cell_size) {
    // fill the vector with the points of specific current figure`s coordinates:
    point_reference = new Point(x_dim / 2 - 1, 1);
    points.push_back(new Point(x_dim / 2, 0));
    points.push_back(new Point(x_dim / 2, 1));
    points.push_back(point_reference);
    points.push_back(new Point(x_dim / 2 - 2, 1));

    // initialize the figure`s grid:
    // set each cell`s size, position 
    // and initial color (same as background):
    for (unsigned i = 0; i < 4; i++)
        for (unsigned j = 0; j < 2; j++) {
            grid[i][j].setSize(cell_size);
            grid[i][j].setPosition(x_dim * (cell_size.x + 1) + (i * cell_size.x) + i + x_dim + 1 + 5.0f, j * cell_size.y + j + 1 + 85.0f);
            grid[i][j].setFillColor(Color(255, 255, 255, 0));
        };

    // set the figure`s own color according to the color code for certain cells:
    grid[2][0].setFillColor(Color(248, 6, 248, 255));
    grid[0][1].setFillColor(Color(248, 6, 248, 255));
    grid[1][1].setFillColor(Color(248, 6, 248, 255));
    grid[2][1].setFillColor(Color(248, 6, 248, 255)); 
}

// J figure initialization:
void Figure_J::initialize(const unsigned& x_dim, const Vector2f& cell_size) {
    // fill the vector with the points of specific current figure`s coordinates:
    point_reference = new Point(x_dim / 2 - 1, 1);
    points.push_back(new Point(x_dim / 2 - 2, 0));
    points.push_back(new Point(x_dim / 2 - 2, 1));
    points.push_back(new Point(x_dim / 2, 1));
    points.push_back(point_reference);

    // initialize the figure`s grid:
    // set each cell`s size, position 
    // and initial color (same as background):
    for (unsigned i = 0; i < 4; i++)
        for (unsigned j = 0; j < 2; j++) {
            grid[i][j].setSize(cell_size);
            grid[i][j].setPosition(x_dim * (cell_size.x + 1) + (i * cell_size.x) + i + x_dim + 1 + 5.0f, j * cell_size.y + j + 1 + 85.0f);
            grid[i][j].setFillColor(Color(255, 255, 255, 0));
        };

    // set the figure`s own color according to the color code for certain cells:
    grid[0][0].setFillColor(Color(134, 33, 255, 255));
    grid[0][1].setFillColor(Color(134, 33, 255, 255));
    grid[1][1].setFillColor(Color(134, 33, 255, 255));
    grid[2][1].setFillColor(Color(134, 33, 255, 255)); 
}

// Z figure initialization:
void Figure_Z::initialize(const unsigned& x_dim, const Vector2f& cell_size) {
    // fill the vector with the points of specific current figure`s coordinates:
    point_reference = new Point(x_dim / 2 - 1, 1);
    points.push_back(new Point(x_dim / 2 - 2, 0));
    points.push_back(new Point(x_dim / 2 - 1, 0));
    points.push_back(new Point(x_dim / 2, 1));
    points.push_back(point_reference);

    // initialize the figure`s grid:
    // set each cell`s size, position 
    // and initial color (same as background):
    for (unsigned i = 0; i < 4; i++)
        for (unsigned j = 0; j < 2; j++) {
            grid[i][j].setSize(cell_size);
            grid[i][j].setPosition(x_dim * (cell_size.x + 1) + (i * cell_size.x) + i + x_dim + 1 + 5.0f, j * cell_size.y + j + 1 + 85.0f);
            grid[i][j].setFillColor(Color(255, 255, 255, 0));
        };

    // set the figure`s own color according to the color code for certain cells:
    grid[0][0].setFillColor(Color(220, 0, 20, 255));
    grid[1][0].setFillColor(Color(220, 0, 20, 255));
    grid[1][1].setFillColor(Color(220, 0, 20, 255));
    grid[2][1].setFillColor(Color(220, 0, 20, 255)); 
}

// S figure initialization:
void Figure_S::initialize(const unsigned& x_dim, const Vector2f& cell_size) {
    // fill the vector with the points of specific current figure`s coordinates:
    point_reference = new Point(x_dim / 2 - 1, 1);
    points.push_back(new Point(x_dim / 2 - 1, 0));
    points.push_back(new Point(x_dim / 2 - 2, 1));
    points.push_back(new Point(x_dim / 2, 1));
    points.push_back(point_reference);

    // initialize the figure`s grid:
    // set each cell`s size, position 
    // and initial color (same as background):
    for (unsigned i = 0; i < 4; i++)
        for (unsigned j = 0; j < 2; j++) {
            grid[i][j].setSize(cell_size);
            grid[i][j].setPosition(x_dim * (cell_size.x + 1) + (i * cell_size.x) + i + x_dim + 1 + 5.0f, j * cell_size.y + j + 1 + 85.0f);
            grid[i][j].setFillColor(Color(255, 255, 255, 0));
        };

    // set the figure`s own color according to the color code for certain cells:
    grid[1][0].setFillColor(Color(46, 228, 25, 255));
    grid[2][0].setFillColor(Color(46, 228, 25, 255));
    grid[0][1].setFillColor(Color(46, 228, 25, 255));
    grid[1][1].setFillColor(Color(46, 228, 25, 255));     
}