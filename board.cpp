#include "board.hpp"
#include <random>

using namespace std;

Board::Board() {
    initialize();
};

// Initialize game board 20x10,
// two more y coordinates to create figures outside (on top of) game board:
void Board::initialize(){
    x_dimension = 10;
    y_dimension = 22;

    map = new unsigned *[x_dimension];
    for (int i = 0; i < x_dimension; i++)
        map[i] = new unsigned [y_dimension];

    for (int i = 0; i < x_dimension; i++)
        for (int j = 0; j < y_dimension; j++)
            map[i][j] = 0;

    current_figure = create_figure();
    next_figure = create_figure();
    add_figure();

    score = 0;
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
        new_figure = new Figure_O(x_dimension);
        break;
    case 1:
        new_figure = new Figure_I(x_dimension);
        break;
    case 2:
        new_figure = new Figure_T(x_dimension);
        break;
    case 3:
        new_figure = new Figure_L(x_dimension);
        break;
    case 4:
        new_figure = new Figure_J(x_dimension);
        break;
    case 5:
        new_figure = new Figure_Z(x_dimension);
        break;
    case 6:
        new_figure = new Figure_S(x_dimension);
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

long Board::get_score() const {
    return score;
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
        if(current_figure->get_points()[i]->get_y() + 1 >= y_dimension){
            is_free = false;
            break;
        }

        if (!is_empty(Point(current_figure->get_points()[i]->get_x(), current_figure->get_points()[i]->get_y() + 1))) {
            is_free = false;
            break;
        };
    };

    int factor = 10;
    if(is_free) {
        factor = 1;
        for (int i = 0; i < current_figure->get_points().size(); i++) {
            change_point(*current_figure->get_points()[i]);
            current_figure->get_points()[i]->increment_y();
        }
    }

    for (int i = 0; i < current_figure->get_points().size(); i++)
        change_point(*current_figure->get_points()[i], current_figure->get_color_code() * factor);
        
    return is_free;
};

void Board::step_left() {
    bool is_free = true;
    
    for (int i = 0; i < current_figure->get_points().size(); i++) {
        if(current_figure->get_points()[i]->get_x() - 1 < 0){
            is_free = false;
            break;
        }

        if (!is_empty(Point(current_figure->get_points()[i]->get_x() - 1, current_figure->get_points()[i]->get_y()))) {
            is_free = false;
            break;
        };
    };

    if(is_free) {
        for (int i = 0; i < current_figure->get_points().size(); i++) {
            change_point(*current_figure->get_points()[i]);
            current_figure->get_points()[i]->increment_x(-1);
        }
    }

    for (int i = 0; i < current_figure->get_points().size(); i++)
        change_point(*current_figure->get_points()[i], current_figure->get_color_code());
    
};

void Board::step_right() {
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
    
    if(is_free) {
        for (int i = 0; i < current_figure->get_points().size(); i++) {
            change_point(*current_figure->get_points()[i]);
            current_figure->get_points()[i]->increment_x();
        }
    }

    for (int i = 0; i < current_figure->get_points().size(); i++)
        change_point(*current_figure->get_points()[i], current_figure->get_color_code());
};

// Check for the full lines and erase them if they are exist:
void Board::erase_lines() {
    // full lines counter:
    unsigned full_lines = 0;
    // the number of the lowest full line :
    unsigned lowest_line = 0;
    // full line trigger:
    bool is_full = true;

    // starting check from the first visible line:
    for (int i = 2; i < y_dimension; i++) {
        for (int j = 0; j < x_dimension; j++) {
            // if a current cell is occupied - do not touch a trigger:
            if (map[j][i] >= 10)
                is_full = true;
            else {
                // otherwise - set trigger as false and move to the next line:
                is_full = false;
                break;
            }
        }

        // if current line is full:
        if (is_full) {
            // if this is the first full line 
            // or if our full lines sequence is continuous: 
            if (lowest_line == 0 || lowest_line == i - 1) {
                // increasing counter value:
                full_lines += 1;
                // change lowest full line value:
                lowest_line = i;
            // if we have a gap between some full lines:
            } else {
                // starting from our previous full line,
                // we are moving the part of a map above that line
                // by the number of steps which is equal to full lines number:
                for (int i = lowest_line; i >= 0; i--)
                    for(int j = 0; j < x_dimension; j++)
                        // lowering existent lines:  
                        if (i >= full_lines)
                            map[j][i] = map[j][i - full_lines];
                        // set the value of the cell out of our dimension as 0:
                        else
                            map[j][i] = 0;

                // change score value according to the number of erased lignes:
                switch (full_lines) {
                    case 1:
                        score += 40;
                        break;
                    case 2:
                        score += 100;
                        break;
                    default:
                        break;
                }

                // reset a number of the full lines (new full lines sequence):
                full_lines = 1;
                // change lowest full line value:
                lowest_line = i;   
            }
        }
    }

    // if we found some full lines:
    if (full_lines > 0) {
        // starting from the lowest full line,
        // we are moving the part of a map above this line
        // by the number of steps which is equal to full lines number:
        for (int i = lowest_line; i >= 0; i--)
            for(int j = 0; j < x_dimension; j++)
                // lowering existent lines:  
                if (i >= full_lines)
                    map[j][i] = map[j][i - full_lines];
                // set the value of the cell out of our dimension as 0:
                else
                    map[j][i] = 0;
    }

    // change score value according to the number of erased lignes:
    switch (full_lines) {
        // if no line has been erased - adding 4 points 
        // (like the figure descent reward),
        // which is equal to the figure size:
        case 0:
            score += 4;
            break;
        case 1:
            score += 40;
            break;
        case 2:
            score += 100;
            break;
        case 3:
            score += 300;
            break;
        case 4:
            score += 1200;
            break;
        default:
            break;
    }
};

// end of game condition:
bool Board::game_over() {
    for (int j = 0; j < x_dimension; j++)
        if (map[j][2] > 7)
             return true;
    
    return false;
};

Board::~Board() {};

ostream& operator <<(ostream& stream, const Board& board) {
    // display a game board starting from the 2nd x coordinate,
    // indices 0 and 1 are reserver for new figure creation zone:
    for (int i = 2; i < board.y_dimension; i++) {
        stream << "| ";
        for (int j = 0; j < board.x_dimension; j++) {
            if(board.map[j][i] < 10)
                stream << " " << board.map[j][i] << " "; 
            else
                stream << board.map[j][i] << " "; 
        };
        stream << " |\n";
    };

    return stream;
};