#define figureCPP

#include "figures.hpp"

void Point::set_x(int& x_new) {
    x = x_new;
};

void Point::set_y(int& y_new) {
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

Point* Figure::get_point_reference() const{
    return point_reference;
}

unsigned Figure::get_color_code() const{
    return color_code;
}

RectangleShape** Figure::get_grid() const {
    RectangleShape** result = new RectangleShape *[FIGURE_GRID_WIDTH];
    for (unsigned i = 0; i < FIGURE_GRID_WIDTH; i++) {
        result[i] = new RectangleShape [FIGURE_GRID_HEIGHT];
    }

    for (unsigned i = 0; i < FIGURE_GRID_WIDTH; i++) 
        for (unsigned j = 0; j < FIGURE_GRID_HEIGHT; j++)
            result[i][j] = grid[i][j];

    return result;
}

void Figure::set_grid(const unsigned& x_dim, const Vector2f& new_cell_size) {
    for (unsigned i = 0; i < FIGURE_GRID_WIDTH; i++)
        for (unsigned j = 0; j < FIGURE_GRID_HEIGHT; j++) {
            grid[i][j].setSize(new_cell_size);
            grid[i][j].setPosition((x_dim + i) * (new_cell_size.x + 1) + x_dim + 1 + 5.0f, (j + 2) * (new_cell_size.y + 1) + 5.0f);
        }
};

bool Figure_Simple_Rotation::get_next_rotation(){
    already_rotated = !already_rotated;
    return already_rotated;
}

// Square figure initialization:
void Figure_O::initialize(const unsigned& x_dim, const Vector2f& cell_size) {
    // fill the vector with the points of specific current figure`s coordinates:
    point_reference = NULL;
    points.push_back(new Point(x_dim / 2 - 1, 0));
    points.push_back(new Point(x_dim / 2, 0));
    points.push_back(new Point(x_dim / 2 - 1, 1));
    points.push_back(new Point(x_dim / 2, 1));
    
    // initialize the figure`s grid:
    // set each cell`s size, position 
    // and initial color (same as background):
    for (unsigned i = 0; i < FIGURE_GRID_WIDTH; i++)
        for (unsigned j = 0; j < FIGURE_GRID_HEIGHT; j++) {
            grid[i][j].setSize(cell_size);
            grid[i][j].setPosition((x_dim + i) * (cell_size.x + 1) + x_dim + 1 + 5.0f, (j + 2) * (cell_size.y + 1) + 5.0f);
            grid[i][j].setFillColor(Color(255, 255, 255, 0));
        };

    // set the figure`s own color according to the color code for certain cells:
    grid[0][0].setFillColor(COLOR_YELLOW);
    grid[0][1].setFillColor(COLOR_YELLOW);
    grid[1][0].setFillColor(COLOR_YELLOW);
    grid[1][1].setFillColor(COLOR_YELLOW);  
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
    for (unsigned i = 0; i < FIGURE_GRID_WIDTH; i++)
        for (unsigned j = 0; j < FIGURE_GRID_HEIGHT; j++) {
            grid[i][j].setSize(cell_size);
            grid[i][j].setPosition((x_dim + i) * (cell_size.x + 1) + x_dim + 1 + 5.0f, (j + 2) * (cell_size.y + 1) + 5.0f);
            grid[i][j].setFillColor(Color(255, 255, 255, 0));
        };

    // set the figure`s own color according to the color code for certain cells:
    grid[0][0].setFillColor(COLOR_CYAN);
    grid[1][0].setFillColor(COLOR_CYAN);
    grid[2][0].setFillColor(COLOR_CYAN);
    grid[3][0].setFillColor(COLOR_CYAN); 
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
    for (unsigned i = 0; i < FIGURE_GRID_WIDTH; i++)
        for (unsigned j = 0; j < FIGURE_GRID_HEIGHT; j++) {
            grid[i][j].setSize(cell_size);
            grid[i][j].setPosition((x_dim + i) * (cell_size.x + 1) + x_dim + 1 + 5.0f, (j + 2) * (cell_size.y + 1) + 5.0f);
            grid[i][j].setFillColor(Color(255, 255, 255, 0));
        };

    // set the figure`s own color according to the color code for certain cells:
    grid[1][0].setFillColor(COLOR_ORANGE);
    grid[0][1].setFillColor(COLOR_ORANGE);
    grid[1][1].setFillColor(COLOR_ORANGE);
    grid[2][1].setFillColor(COLOR_ORANGE); 
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
    for (unsigned i = 0; i < FIGURE_GRID_WIDTH; i++)
        for (unsigned j = 0; j < FIGURE_GRID_HEIGHT; j++) {
            grid[i][j].setSize(cell_size);
            grid[i][j].setPosition((x_dim + i) * (cell_size.x + 1) + x_dim + 1 + 5.0f, (j + 2) * (cell_size.y + 1) + 5.0f);
            grid[i][j].setFillColor(Color(255, 255, 255, 0));
        };

    // set the figure`s own color according to the color code for certain cells:
    grid[2][0].setFillColor(COLOR_PINK);
    grid[0][1].setFillColor(COLOR_PINK);
    grid[1][1].setFillColor(COLOR_PINK);
    grid[2][1].setFillColor(COLOR_PINK); 
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
    for (unsigned i = 0; i < FIGURE_GRID_WIDTH; i++)
        for (unsigned j = 0; j < FIGURE_GRID_HEIGHT; j++) {
            grid[i][j].setSize(cell_size);
            grid[i][j].setPosition((x_dim + i) * (cell_size.x + 1) + x_dim + 1 + 5.0f, (j + 2) * (cell_size.y + 1) + 5.0f);
            grid[i][j].setFillColor(Color(255, 255, 255, 0));
        };

    // set the figure`s own color according to the color code for certain cells:
    grid[0][0].setFillColor(COLOR_DARK_VIOLET);
    grid[0][1].setFillColor(COLOR_DARK_VIOLET);
    grid[1][1].setFillColor(COLOR_DARK_VIOLET);
    grid[2][1].setFillColor(COLOR_DARK_VIOLET); 
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
    for (unsigned i = 0; i < FIGURE_GRID_WIDTH; i++)
        for (unsigned j = 0; j < FIGURE_GRID_HEIGHT; j++) {
            grid[i][j].setSize(cell_size);
            grid[i][j].setPosition((x_dim + i) * (cell_size.x + 1) + x_dim + 1 + 5.0f, (j + 2) * (cell_size.y + 1) + 5.0f);
            grid[i][j].setFillColor(Color(255, 255, 255, 0));
        };

    // set the figure`s own color according to the color code for certain cells:
    grid[0][0].setFillColor(COLOR_RED);
    grid[1][0].setFillColor(COLOR_RED);
    grid[1][1].setFillColor(COLOR_RED);
    grid[2][1].setFillColor(COLOR_RED); 
}

// S figure initialization:
void Figure_S::initialize(const unsigned& x_dim, const Vector2f& cell_size) {
    // fill the vector with the points of specific current figure`s coordinates:
    point_reference = new Point(x_dim / 2 - 1, 0);
    points.push_back(new Point(x_dim / 2, 0));
    points.push_back(new Point(x_dim / 2 - 2, 1));
    points.push_back(new Point(x_dim / 2 - 1, 1));
    points.push_back(point_reference);

    // initialize the figure`s grid:
    // set each cell`s size, position 
    // and initial color (same as background):
    for (unsigned i = 0; i < FIGURE_GRID_WIDTH; i++)
        for (unsigned j = 0; j < FIGURE_GRID_HEIGHT; j++) {
            grid[i][j].setSize(cell_size);
            grid[i][j].setPosition((x_dim + i) * (cell_size.x + 1) + x_dim + 1 + 5.0f, (j + 2) * (cell_size.y + 1) + 5.0f);
            grid[i][j].setFillColor(Color(255, 255, 255, 0));
        };

    // set the figure`s own color according to the color code for certain cells:
    grid[1][0].setFillColor(COLOR_GREEN_SALAD);
    grid[2][0].setFillColor(COLOR_GREEN_SALAD);
    grid[0][1].setFillColor(COLOR_GREEN_SALAD);
    grid[1][1].setFillColor(COLOR_GREEN_SALAD);     
}