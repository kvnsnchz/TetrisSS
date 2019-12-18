#include "board.hpp"
#include <random>

using namespace std;

Board::Board() {
    initialize();
};

// Initialize game board 20x10,
// two more x coordinates to create figures on top of the game board
void Board::initialize(){
    x_dimension = 22;
    y_dimension = 10;

    map = new unsigned *[x_dimension];
    for (int i = 0; i < x_dimension; i++)
        map[i] = new unsigned [y_dimension];

    for (int i = 0; i < x_dimension; i++)
        for (int j = 0; j < y_dimension; j++)
            map[i][j] = 0;

    current_figure = create_figure();
    next_figure = create_figure();
    add_figure();
}

Figure* Board::create_figure() {
    // figure that we are going to create:
    Figure* new_figure;

    // Randomly choosing a figure of certain type according to figure_number value:
    // 0 - Figure_0; 1 - Figure_I; 2 - Figure_T;
    // 3 - Figure_L; 4 - Figure_J; 5 - Figure_Z; 6 - Figure_S.
    unsigned figure_number = rand() % 7;
    
    // Using switch to construct chosen figure:
    switch (figure_number) {
    case 0:
        new_figure = new Figure_O(y_dimension);
        break;
    case 1:
        new_figure = new Figure_I(y_dimension);
        break;
    case 2:
        new_figure = new Figure_T(y_dimension);
        break;
    case 3:
        new_figure = new Figure_L(y_dimension);
        break;
    case 4:
        new_figure = new Figure_J(y_dimension);
        break;
    case 5:
        new_figure = new Figure_Z(y_dimension);
        break;
    case 6:
        new_figure = new Figure_S(y_dimension);
        break;
    default:
        break;
    }

    return new_figure;
}

void Board::add_figure() {
    for (int i = 0; i < current_figure->get_points().size(); i++) 
        change_point(*current_figure->get_points()[i], current_figure->get_color_code());
}

unsigned Board::get_x_dim() const {
    return x_dimension;
};

unsigned Board::get_y_dim() const {
    return y_dimension;
};

Figure* Board::get_next_figure() const {
    return next_figure;
};
        
void Board::set_current_figure(Figure* figure) {
    current_figure = figure;
};

void Board::set_next_figure(Figure* figure) {
    next_figure = figure;
};


bool Board::is_empty(const Point& point) const {
    if (map[point.get_x()][point.get_y()] == 0 || map[point.get_x()][point.get_y()] == current_figure->get_color_code())
        return true;
    else 
        return false;
};

void Board::change_point(const Point& point, const unsigned& new_value) {
    map[point.get_x()][point.get_y()] = new_value;
};

bool Board::step_down() {
    bool is_free = true;
    
    for (int i = 0; i < current_figure->get_points().size(); i++) {
        if(current_figure->get_points()[i]->get_x() + 1 >= x_dimension){
            is_free = false;
            break;
        }

        if (!is_empty(Point(current_figure->get_points()[i]->get_x() + 1, current_figure->get_points()[i]->get_y()))) {
            is_free = false;
            break;
        };
    };

    int factor = 10;
    if(is_free) {
        factor = 1;
        for (int i = 0; i < current_figure->get_points().size(); i++) {
            change_point(*current_figure->get_points()[i]);
            current_figure->get_points()[i]->increment_x();
        }
    }

    for (int i = 0; i < current_figure->get_points().size(); i++)
        change_point(*current_figure->get_points()[i], current_figure->get_color_code() * factor);
        
    return is_free;
};

void Board::step_left() {
    bool is_free = true;
    
    for (int i = 0; i < current_figure->get_points().size(); i++) {
        if(current_figure->get_points()[i]->get_y() - 1 < 0){
            is_free = false;
            break;
        }

        if (!is_empty(Point(current_figure->get_points()[i]->get_x(), current_figure->get_points()[i]->get_y() - 1))) {
            is_free = false;
            break;
        };
    };

    if(is_free) {
        for (int i = 0; i < current_figure->get_points().size(); i++) {
            change_point(*current_figure->get_points()[i]);
            current_figure->get_points()[i]->increment_y(-1);
        }
    }

    for (int i = 0; i < current_figure->get_points().size(); i++)
        change_point(*current_figure->get_points()[i], current_figure->get_color_code());
    
};

void Board::step_right() {
    bool is_free = true;
    
    for (int i = 0; i < current_figure->get_points().size(); i++) {
        if(current_figure->get_points()[i]->get_y() + 1 >= y_dimension){
            is_free = false;
            break;
        }

        if (!is_empty(Point(current_figure->get_points()[i]->get_x(), current_figure->get_points()[i]->get_y() + 1))) {
            is_free = false;
            break;
        };
    };
    
    if(is_free) {
        for (int i = 0; i < current_figure->get_points().size(); i++) {
            change_point(*current_figure->get_points()[i]);
            current_figure->get_points()[i]->increment_y();
        }
    }

    for (int i = 0; i < current_figure->get_points().size(); i++)
        change_point(*current_figure->get_points()[i], current_figure->get_color_code());
};

// end of game condition:
bool Board::game_over() {
    for (int j = 0; j < y_dimension; j++)
        if (map[2][j] > 7)
             return true;
    
    return false;
};

Board::~Board() {};

ostream& operator <<(ostream& stream, const Board& board) {
    // display a game board starting from the 2nd x coordinate,
    // indices 0 and 1 are reserver for new figure creation zone:
    for (int i = 2; i < board.x_dimension; i++) {
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