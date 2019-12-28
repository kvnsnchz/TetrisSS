#include "board.hpp"
#include <random>
#include <algorithm>

Vector2f boardCellSize(40.0f, 40.0f);
// Next figure graphic variable,
// she is declared globally because she needs to be called
// from different methods (needs to be updated):
RectangleShape** figure_to_draw;

Board::Board(RenderWindow& window) {
    initialize(window);
};

// Initialize game board 20x10,
// two more y coordinates to create figures outside (on top of) game board:
void Board::initialize(RenderWindow& window){
    x_dimension = 10;
    y_dimension = 22;

    // initialize map and board:
    map = new unsigned *[x_dimension];
    grid = new RectangleShape *[x_dimension];
    for (unsigned i = 0; i < x_dimension; i++) {
        map[i] = new unsigned [y_dimension];
        grid[i] = new RectangleShape [y_dimension];
    }

    for (unsigned i = 0; i < x_dimension; i++)
        for (unsigned j = 0; j < y_dimension; j++)
            map[i][j] = 0;

    current_figure = create_figure();
    next_figure = create_figure();
    add_figure();

    // draw the initial grid:
    for(unsigned i = 0; i < x_dimension; i++) {
        for(unsigned j = 2; j < y_dimension; j++) {
            grid[i][j].setSize(boardCellSize);
            grid[i][j].setPosition(i * boardCellSize.x + (i + 1) * 5.0f, (j - 2) * boardCellSize.y + (j - 1) * 5.0f);

            window.draw(grid[i][j]);
        }
    }

    score = 0;
}

Figure* Board::create_figure() {
    // figure that we are going to create:
    Figure* new_figure;

    // Randomly choosing a figure
    // of certain type according to figure_number value:
    // 1 - Figure_O; 2 - Figure_I; 3 - Figure_T;
    // 4 - Figure_L; 5 - Figure_J; 6 - Figure_Z; 7 - Figure_S:
    random_device generator;
    uniform_int_distribution<unsigned> distribution(1,7);
    unsigned figure_number = distribution(generator);

    // Using switch to construct chosen figure:
    switch (figure_number) {
    case 1:
        new_figure = new Figure_O(x_dimension);
        break;
    case 2:
        new_figure = new Figure_I(x_dimension);
        break;
    case 3:
        new_figure = new Figure_T(x_dimension);
        break;
    case 4:
        new_figure = new Figure_L(x_dimension);
        break;
    case 5:
        new_figure = new Figure_J(x_dimension);
        break;
    case 6:
        new_figure = new Figure_Z(x_dimension);
        break;
    case 7:
        new_figure = new Figure_S(x_dimension);
        break;
    default:
        break;
    }

    return new_figure;
}

void Board::add_figure() {
    for (unsigned i = 0; i < current_figure->get_points().size(); i++) 
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

// print game board:
void Board::print_board(RenderWindow& window) {
    for(unsigned i = 0; i < x_dimension; i++) {
        for(unsigned j = 2; j < y_dimension; j++) {
            switch(map[i][j]) {
                case 0:
                    grid[i][j].setFillColor(Color(121, 163, 249, 180));
                    break;
                case 1:
                case 10:
                    grid[i][j].setFillColor(Color(255, 216, 0, 255));
                    break;
                case 2:
                case 20:
                    grid[i][j].setFillColor(Color(28, 230, 199, 255));
                    break;
                case 3:
                case 30:
                    grid[i][j].setFillColor(Color(248, 131, 6, 255));
                    break;
                case 4:
                case 40:
                    grid[i][j].setFillColor(Color(248, 6, 248, 255));
                    break;
                case 5:
                case 50:
                    grid[i][j].setFillColor(Color(134, 33, 255, 255));
                    break;
                case 6:
                case 60:
                    grid[i][j].setFillColor(Color(220, 0, 20, 255));
                    break;
                case 7:
                case 70:
                    grid[i][j].setFillColor(Color(46, 228, 25, 255));
                    break;
                default:
                    break;
            }

            window.draw(grid[i][j]);
        }
    }   

    // update next figure:
    Font font;
    try {
        if (!font.loadFromFile("amatic/Amatic-Bold.ttf")) 
            throw 0;
    } catch (int e) {
        if (e == 0)
            cout << "Sorry, font file not found!" << endl;
    } 
    
    Text next_figure_title;
    next_figure_title.setFont(font);
    next_figure_title.setString("Next Figure:");
    next_figure_title.setPosition(x_dimension * (boardCellSize.x + 1) + (x_dimension + 1) * 5.0f, 25.0f);
    next_figure_title.setCharacterSize(50);
    next_figure_title.setFillColor(Color(242, 0, 0, 255));
    next_figure_title.setStyle(Text::Bold);
    window.draw(next_figure_title);

    // next figure itself (its grid):
    figure_to_draw = next_figure->get_grid();
    for (unsigned i = 0; i < 4; i++)
        for (unsigned j = 0; j < 2; j++)
            window.draw(figure_to_draw[i][j]);

    // draw a score:
    Text score_title;
    score_title.setFont(font);
    score_title.setString("Score: \n" + to_string(score));
    score_title.setPosition(x_dimension * (boardCellSize.x + 1) + (x_dimension + 1) * 5.0f, 250.0f);
    score_title.setCharacterSize(50);
    score_title.setFillColor(Color(242, 0, 0, 255));
    score_title.setStyle(Text::Bold);
    window.draw(score_title);
};

bool Board::is_empty(const Point& point) const {
    if (map[point.get_x()][point.get_y()] == 0 || map[point.get_x()][point.get_y()] == current_figure->get_color_code())
        return true;
    else 
        return false;
};

void Board::change_point(const Point& point, const int& new_value) {
    map[point.get_x()][point.get_y()] = new_value;
};

bool Board::step_down() {
    bool is_free = true;
    
    for (unsigned i = 0; i < current_figure->get_points().size(); i++) {
        if(current_figure->get_points()[i]->get_y() + 1 >= y_dimension){
            is_free = false;
            break;
        }

        if (!is_empty(Point(current_figure->get_points()[i]->get_x(), current_figure->get_points()[i]->get_y() + 1))) {
            is_free = false;
            break;
        };
    };

    unsigned factor = 10;
    if(is_free) {
        factor = 1;
        for (unsigned i = 0; i < current_figure->get_points().size(); i++) {
            change_point(*current_figure->get_points()[i]);
            current_figure->get_points()[i]->increment_y();
        }
    }

    for (unsigned i = 0; i < current_figure->get_points().size(); i++)
        change_point(*current_figure->get_points()[i], current_figure->get_color_code() * factor);
        
    return is_free;
};

void Board::step_left() {
    bool is_free = true;
    
    for (unsigned i = 0; i < current_figure->get_points().size(); i++) {
        if(current_figure->get_points()[i]->get_x() == 0){
            is_free = false;
            break;
        }

        if (!is_empty(Point(current_figure->get_points()[i]->get_x() - 1, current_figure->get_points()[i]->get_y()))) {
            is_free = false;
            break;
        };
    };

    if(is_free) {
        for (unsigned i = 0; i < current_figure->get_points().size(); i++) {
            change_point(*current_figure->get_points()[i]);
            current_figure->get_points()[i]->increment_x(-1);
        }
    }

    for (unsigned i = 0; i < current_figure->get_points().size(); i++)
        change_point(*current_figure->get_points()[i], current_figure->get_color_code());
    
};

void Board::step_right() {
    bool is_free = true;
    
    for (unsigned i = 0; i < current_figure->get_points().size(); i++) {
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
        for (unsigned i = 0; i < current_figure->get_points().size(); i++) {
            change_point(*current_figure->get_points()[i]);
            current_figure->get_points()[i]->increment_x();
        }
    }

    for (unsigned i = 0; i < current_figure->get_points().size(); i++)
        change_point(*current_figure->get_points()[i], current_figure->get_color_code());
};

// Check for the full lines and erase them if they are exist:
void Board::erase_lines() {
    // full lines counter:
    unsigned full_lines = 0;
    // second full lines counter in case of a gap
    // between full lines:
    unsigned previous_full_lines = 0;
    // the number of the lowest full line :
    unsigned lowest_line = 0;
    // full line trigger:
    bool is_full = true;

    // starting check from the first visible line:
    for (unsigned i = 2; i < y_dimension; i++) {
        for (unsigned j = 0; j < x_dimension; j++) {
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
            if (lowest_line == 0 || lowest_line + 1 == i) {
                // increasing counter value:
                full_lines += 1;
                // change lowest full line value:
                lowest_line = i;
            // if we have a gap between some full lines:
            } else {
                // starting from our previous full line,
                // we are moving the part of a map above that line
                // by the number of steps which is equal to full lines number:
                for (unsigned i = lowest_line; i > 0; i--)
                    for(unsigned j = 0; j < x_dimension; j++)
                        // lowering existent lines:  
                        if (i >= full_lines)
                            map[j][i] = map[j][i - full_lines];
                        // set the value of the cell out of our dimension as 0:
                        else
                            map[j][i] = 0;

                // remember the number of full lines in the previous sequence
                // to calculate a score correctly in the end:
                previous_full_lines = full_lines;
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
        for (unsigned i = lowest_line; i > 0; i--)
            for(unsigned j = 0; j < x_dimension; j++)
                // lowering existent lines:  
                if (i >= full_lines)
                    map[j][i] = map[j][i - full_lines];
                // set the value of the cell out of our dimension as 0:
                else
                    map[j][i] = 0;
    }

    // change score value according to the number of erased lignes:
    switch (full_lines + previous_full_lines) {
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
    for (unsigned j = 0; j < x_dimension; j++)
        if (map[j][2] >= 10)
             return true;
    
    return false;
};

Board::~Board() {};

ostream& operator <<(ostream& stream, const Board& board) {
    // display a game board starting from the 2nd x coordinate,
    // indices 0 and 1 are reserver for new figure creation zone:
    for (unsigned i = 2; i < board.y_dimension; i++) {
        stream << "| ";
        for (unsigned j = 0; j < board.x_dimension; j++) {
            if(board.map[j][i] < 10)
                stream << " " << board.map[j][i] << " "; 
            else
                stream << board.map[j][i] << " "; 
        };
        stream << " |\n";
    };

    return stream;
};