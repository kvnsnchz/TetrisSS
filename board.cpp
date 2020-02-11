#define boardCPP

#include "board.hpp"
#include <random>
#include <algorithm>

// Next figure graphic variable,
// she is declared globally because she needs to be called
// from different methods (needs to be updated):
RectangleShape** figure_to_draw;

Board::Board(RenderWindow& window, const unsigned& initial_complexity, const Vector2f& initial_cell_size) {
    initialize(window, initial_complexity, initial_cell_size);
};

// Initialize game board 20x10,
// two more y coordinates to create figures outside (on top of) the game board:
void Board::initialize(RenderWindow& window, const unsigned& initial_complexity, const Vector2f& initial_cell_size){
    x_dimension = BOARD_GRID_WIDTH;
    y_dimension = BOARD_GRID_HEIGHT + FIGURE_GRID_HEIGHT;

    // initialize game complexity:
    complexity = initial_complexity;

    // initialize cell size:
    cell_size.x = initial_cell_size.x;
    cell_size.y = initial_cell_size.y;

    // initialize map and board:
    map = new unsigned *[x_dimension];
    grid = new RectangleShape *[x_dimension];
    for (int i = 0; i < x_dimension; i++) {
        map[i] = new unsigned [y_dimension];
        grid[i] = new RectangleShape [y_dimension];
    }

    for (int i = 0; i < x_dimension; i++)
        for (int j = 0; j < y_dimension; j++)
            map[i][j] = 0;

    current_figure = create_figure();
    next_figure = create_figure();
    add_figure();

    // draw the initial grid:
    for(int i = 0; i < x_dimension; i++) {
        for(int j = FIGURE_GRID_HEIGHT; j < y_dimension; j++) {
            grid[i][j].setSize(cell_size);
            grid[i][j].setPosition(i * cell_size.x + i + 5.0f, (j - FIGURE_GRID_HEIGHT) * cell_size.y + j - FIGURE_GRID_HEIGHT + 5.0f);

            window.draw(grid[i][j]);
        }
    }

    score = 0;
}

Figure* Board::create_figure() {
    shadow_points.clear();
    // figure that we are going to create:
    Figure* new_figure;

    // Randomly choosing a figure
    // of certain type according to figure_number value:
    // 1 - Figure_O; 2 - Figure_I; 3 - Figure_T;
    // 4 - Figure_L; 5 - Figure_J; 6 - Figure_Z; 7 - Figure_S:
    random_device generator;
    uniform_int_distribution<unsigned> distribution(1, 7);
    unsigned figure_number = distribution(generator);

    // Using switch to construct chosen figure:
    switch (figure_number) {
    case 1:
        new_figure = new Figure_O(x_dimension, cell_size);
        break;
    case 2:
        new_figure = new Figure_I(x_dimension, cell_size);
        break;
    case 3:
        new_figure = new Figure_T(x_dimension, cell_size);
        break;
    case 4:
        new_figure = new Figure_L(x_dimension, cell_size);
        break;
    case 5:
        new_figure = new Figure_J(x_dimension, cell_size);
        break;
    case 6:
        new_figure = new Figure_Z(x_dimension, cell_size);
        break;
    case 7:
        new_figure = new Figure_S(x_dimension, cell_size);
        break;
    default:
        break;
    }

    return new_figure;
}

void Board::add_figure() {
    for (unsigned i = 0; i < current_figure->get_points().size(); i++) 
        change_point(*current_figure->get_points()[i], current_figure->get_color_code());
};

unsigned** Board::get_map() const {
    return map;
}

unsigned Board::get_complexity() {
    return complexity;
};

int Board::get_x_dim() const {
    return x_dimension;
};

int Board::get_y_dim() const {
    return y_dimension;
};

long Board::get_score() const {
    return score;
};

Figure* Board::get_next_figure() const {
    return next_figure;
};

Thread* Board::get_descend_thread() const {
    return descend_thread;
};

void Board::set_map(const unsigned new_map[BOARD_GRID_WIDTH][BOARD_GRID_HEIGHT + FIGURE_GRID_HEIGHT]) {
    for (unsigned i = 0; i < BOARD_GRID_WIDTH; i++)
        for (unsigned j = 0; j < BOARD_GRID_HEIGHT + FIGURE_GRID_HEIGHT; j++)
            map[i][j] = new_map[i][j];
};

void Board::set_complexity(const unsigned& new_complexity) {
    complexity = new_complexity;
};

void Board::set_cell_size(const Vector2f& new_cell_size) {
    cell_size.x = new_cell_size.x;
    cell_size.y = new_cell_size.y;

    // Update next figures' cell size:
    next_figure->set_grid(x_dimension, new_cell_size);
};

void Board::set_score(const long& new_score) {
    score = new_score;
};

void Board::set_current_figure(Figure* figure) {
    current_figure = figure;
};

void Board::set_next_figure(Figure* figure) {
    next_figure = figure;
};

void Board::set_descend_thread(Thread* new_thread) {
    descend_thread = new_thread;
};

// print game board:
void Board::print_board(RenderWindow& window, const Font& font, const double& font_size, const string& nickname, const bool& game_is_over, const unsigned& board_index) {
    for(int i = 0; i < x_dimension; i++) {
        for(int j = FIGURE_GRID_HEIGHT; j < y_dimension; j++) {
            // Update cell size and position:
            grid[i][j].setSize(cell_size);
            if (board_index > 0)
                grid[i][j].setPosition((x_dimension * (2 * cell_size.x + 1.0f) - 1.0f) + (board_index - 1) * ((x_dimension * (cell_size.x + 1.0f) - 1.0f)) + i * (cell_size.x + 1) + (board_index + 3) * 10.0f + (float) font_size * MAX_NICKNAME_LENGTH / 2.15,
                    (j - FIGURE_GRID_HEIGHT) * cell_size.y + j - FIGURE_GRID_HEIGHT + 5.0f);
            else
                grid[i][j].setPosition(i * (cell_size.x + 1) + 5.0f,
                    (j - FIGURE_GRID_HEIGHT) * cell_size.y + j - FIGURE_GRID_HEIGHT + 5.0f);

            // Update cell color:
            switch(map[i][j]) {
                case 0:
                    grid[i][j].setFillColor(COLOR_TRANSPARENT_WHITE);
                    break;
                case SHADOW_COLOR_CODE:
                    grid[i][j].setFillColor(COLOR_TRANSPARENT_GREY);
                    break;
                case FIGURE_O_COLOR_CODE:
                case FIGURE_O_COLOR_CODE * STOP_FIGURE_FACTOR:
                    grid[i][j].setFillColor(COLOR_YELLOW);
                    break;
                case FIGURE_I_COLOR_CODE:
                case FIGURE_I_COLOR_CODE * STOP_FIGURE_FACTOR:
                    grid[i][j].setFillColor(COLOR_CYAN);
                    break;
                case FIGURE_T_COLOR_CODE:
                case FIGURE_T_COLOR_CODE * STOP_FIGURE_FACTOR:
                    grid[i][j].setFillColor(COLOR_ORANGE);
                    break;
                case FIGURE_L_COLOR_CODE:
                case FIGURE_L_COLOR_CODE * STOP_FIGURE_FACTOR:
                    grid[i][j].setFillColor(COLOR_PINK);
                    break;
                case FIGURE_J_COLOR_CODE:
                case FIGURE_J_COLOR_CODE * STOP_FIGURE_FACTOR:
                    grid[i][j].setFillColor(COLOR_DARK_VIOLET);
                    break;
                case FIGURE_Z_COLOR_CODE:
                case FIGURE_Z_COLOR_CODE * STOP_FIGURE_FACTOR:
                    grid[i][j].setFillColor(COLOR_RED);
                    break;
                case FIGURE_S_COLOR_CODE:
                case FIGURE_S_COLOR_CODE * STOP_FIGURE_FACTOR:
                    grid[i][j].setFillColor(COLOR_GREEN_SALAD);
                    break;
                default:
                    break;
            }

            window.draw(grid[i][j]);
        }
    }   

    // if the function was called from sinpleplayer:
    Text next_figure_title;
    if (board_index == 0) {
        // update next figure:
        next_figure_title.setFont(font);
        next_figure_title.setString("Next Figure:");
        next_figure_title.setPosition(x_dimension * (cell_size.x + 1) + x_dimension + 4.0f, 10.0f);
        next_figure_title.setCharacterSize(font_size);
        next_figure_title.setStyle(Text::Bold);
        next_figure_title.setFillColor(COLOR_LIGHT_GREEN);
        window.draw(next_figure_title);

        // next figure itself (its grid):
        figure_to_draw = next_figure->get_grid();
        for (unsigned i = 0; i < FIGURE_GRID_WIDTH; i++)
            for (unsigned j = 0; j < FIGURE_GRID_HEIGHT; j++)
                window.draw(figure_to_draw[i][j]);
    }

    // draw a score:
    Text score_title;
    score_title.setFont(font);
    score_title.setCharacterSize(font_size);
    score_title.setStyle(Text::Bold);
    score_title.setFillColor(COLOR_LIGHT_GREEN);
    if (board_index == 0) {
        if (nickname == "" && !game_is_over)
            score_title.setString("My Score:\n" + to_string(score));
        else if (nickname == "" && game_is_over)
            score_title.setString("Final Score:\n" + to_string(score));
        else if (nickname != "" && !game_is_over)
            score_title.setString(nickname + "\nScore:\n" + to_string(score));
        else if (nickname != "" && game_is_over)
            score_title.setString(nickname + "\nFinal Score:\n" + to_string(score));
        score_title.setPosition(x_dimension * (cell_size.x + 1) + x_dimension + 5.0f, 
            next_figure_title.getGlobalBounds().height + next_figure_title.getGlobalBounds().top + FIGURE_GRID_HEIGHT * cell_size.y + 30.0f);
    } else {
        if (!game_is_over)
            score_title.setString(nickname + "\nScore:\n" + to_string(score));
        else if (game_is_over)
            score_title.setString(nickname + "\nFinal Score:\n" + to_string(score));
        score_title.setPosition(grid[0][y_dimension - 1].getPosition().x + (grid[x_dimension - 1][y_dimension - 1].getPosition().x + 20.0f - grid[0][y_dimension - 1].getPosition().x - score_title.getGlobalBounds().width) / 2,
            (y_dimension - FIGURE_GRID_HEIGHT) * (cell_size.y + 1) + 4.0f);
    }
    window.draw(score_title);
};

overflow Board::is_empty(const Point& point) const {
    if (point.get_x() < 0)
        return OVERFLOW_LEFT;
    if (point.get_x() >= x_dimension)
        return OVERFLOW_RIGHT;
    if (point.get_y() < 0)
        return OVERFLOW_UP;
    if (point.get_y() >= y_dimension)
        return OVERFLOW_DOWN;

    if (map[point.get_x()][point.get_y()] == 0 
            || map[point.get_x()][point.get_y()] == current_figure->get_color_code()
            || map[point.get_x()][point.get_y()] == SHADOW_COLOR_CODE
        )
        return NONE;
    else 
        return OVERFLOW_DOWN;
};

void Board::change_point(const Point& point, const int& new_value, const bool& from_rotation) {
    if (point.get_x() < 0 || point.get_x() >= x_dimension || point.get_y() < 0 || point.get_y() >= y_dimension) {
        return;
    }

    map[point.get_x()][point.get_y()] = new_value;
};

bool Board::step_down() {
    bool is_free = true;
    
    for (unsigned i = 0; i < current_figure->get_points().size(); i++) {
        if(current_figure->get_points()[i]->get_y() + 1 >= y_dimension) {
            is_free = false;
            break;
        }

        if (is_empty(Point(current_figure->get_points()[i]->get_x(), current_figure->get_points()[i]->get_y() + 1)) != NONE) {
            is_free = false;
            break;
        }
    }

    if (is_free) {
        for (unsigned i = 0; i < current_figure->get_points().size(); i++) {
            change_point(*current_figure->get_points()[i]);
            current_figure->get_points()[i]->increment_y();
        }

        for (unsigned i = 0; i < current_figure->get_points().size(); i++)
            change_point(*current_figure->get_points()[i], current_figure->get_color_code());

        shadow();
    }
    else{
        shadow_points.clear();
    }
        
    return is_free;
};

void Board::hard_drop() {
    bool is_free = true;
    vector<Point> current_points;

    for (unsigned i = 0; i < current_figure->get_points().size(); i++){
        current_points.emplace_back(*current_figure->get_points()[i]);
    }

    while(is_free){
        for (unsigned i = 0; i < current_figure->get_points().size(); i++) {
            if(current_figure->get_points()[i]->get_y() + 1 >= y_dimension) {
                is_free = false;
                break;
            }

            if (is_empty(Point(current_figure->get_points()[i]->get_x(), current_figure->get_points()[i]->get_y() + 1)) != NONE) {
                is_free = false;
                break;
            }
        }

        if(is_free)
            for (unsigned i = 0; i < current_figure->get_points().size(); i++) 
                current_figure->get_points()[i]->increment_y();
    }
    
    for (unsigned i = 0; i < current_points.size(); i++) 
        change_point(current_points[i]);

    for (unsigned i = 0; i < current_figure->get_points().size(); i++)
        change_point(*current_figure->get_points()[i], current_figure->get_color_code());
}

void Board::shadow(){
    bool is_free = true;

    if(shadow_points.size() > 0){
        for (unsigned i = 0; i < shadow_points.size(); i++) 
            if(map[shadow_points[i].get_x()][shadow_points[i].get_y()] != current_figure->get_color_code())
                change_point(shadow_points[i]);
        shadow_points.clear();
    }

    for (unsigned i = 0; i < current_figure->get_points().size(); i++){
        shadow_points.emplace_back(*current_figure->get_points()[i]);
    }

    while(is_free){
        for (unsigned i = 0; i < shadow_points.size(); i++) {
            if(shadow_points[i].get_y() + 1 >= y_dimension) {
                is_free = false;
                break;
            }

            if (is_empty(Point(shadow_points[i].get_x(), shadow_points[i].get_y() + 1)) != NONE) {
                is_free = false;
                break;
            }
        }

        if(is_free)
            for (unsigned i = 0; i < shadow_points.size(); i++) 
                shadow_points[i].increment_y();
    }

    for (unsigned i = 0; i < shadow_points.size(); i++)
        if(map[shadow_points[i].get_x()][shadow_points[i].get_y()] != current_figure->get_color_code())
            change_point(shadow_points[i], SHADOW_COLOR_CODE);
}

bool Board::step_left(const bool& with_floor) {
    bool is_free = true;
    unsigned floor = 0;

    for (unsigned i = 0; i < current_figure->get_points().size(); i++) {
        if(current_figure->get_points()[i]->get_x() == 0){
            is_free = false;
            break;
        }

        if (is_empty(Point(current_figure->get_points()[i]->get_x() - 1, current_figure->get_points()[i]->get_y())) != NONE) {
            is_free = false;
            break;
        };

        if(with_floor)
            if(has_floor(Point(current_figure->get_points()[i]->get_x() - 1, current_figure->get_points()[i]->get_y())))
                floor++;
    };

    if(with_floor && floor == 0)
        is_free = false;

    if(is_free) {
        for (unsigned i = 0; i < current_figure->get_points().size(); i++) {
            change_point(*current_figure->get_points()[i]);
            current_figure->get_points()[i]->increment_x(-1);
        }
        for (unsigned i = 0; i < current_figure->get_points().size(); i++)
            change_point(*current_figure->get_points()[i], current_figure->get_color_code());
        
        shadow();
    }

    return is_free;
    
};

bool Board::step_right(const bool& with_floor) {
    bool is_free = true;
    unsigned floor = 0;

    for (unsigned i = 0; i < current_figure->get_points().size(); i++) {
        if(current_figure->get_points()[i]->get_x() + 1 >= x_dimension){
            is_free = false;
            break;
        }

        if (is_empty(Point(current_figure->get_points()[i]->get_x() + 1, current_figure->get_points()[i]->get_y())) != NONE) {
            is_free = false;
            break;
        };

        if(with_floor)
            if(has_floor(Point(current_figure->get_points()[i]->get_x() + 1, current_figure->get_points()[i]->get_y())))
                floor++;
                
    };
    
    if(with_floor && floor == 0)
        is_free = false;

    if(is_free) {
        for (unsigned i = 0; i < current_figure->get_points().size(); i++) {
            change_point(*current_figure->get_points()[i]);
            current_figure->get_points()[i]->increment_x();
        }
        for (unsigned i = 0; i < current_figure->get_points().size(); i++)
            change_point(*current_figure->get_points()[i], current_figure->get_color_code());

        shadow();
    }

    return is_free;
};

void Board::change_points_rotated(const overflow& overf){
    overflow _overf;

    if(overf != NONE)
        for (unsigned i = 0; i < current_figure->get_points().size(); i++) {
            if(overf == OVERFLOW_DOWN)
                current_figure->get_points()[i]->increment_y(-1);
                
            if(overf == OVERFLOW_LEFT)
                current_figure->get_points()[i]->increment_x(1);
            if(overf == OVERFLOW_RIGHT)
                current_figure->get_points()[i]->increment_x(-1);
        }

    for (unsigned i = 0; i < current_figure->get_points().size(); i++){
        _overf = is_empty(*current_figure->get_points()[i]);
        if(_overf != NONE){
            break;
        }
    }
    
    if(_overf != NONE && _overf != OVERFLOW_UP)
        change_points_rotated(_overf);
    else
        for (unsigned i = 0; i < current_figure->get_points().size(); i++)
            change_point(*current_figure->get_points()[i], current_figure->get_color_code()); 

    return;
}

bool Board::has_floor(const Point& point) const{
    if(is_empty(Point(point.get_x(), point.get_y() + 1)) == NONE)
        return false;
    return true;
}

void Board::rotate(bool right){
    if(dynamic_cast<Figure_Simple_Rotation*>(current_figure)){
       right = ((Figure_Simple_Rotation*)current_figure)->get_next_rotation();
    }

    for (unsigned i = 0; i < current_figure->get_points().size(); i++) {
        change_point(*current_figure->get_points()[i]);
        current_figure->get_points()[i]->rotate(right, current_figure->get_point_reference());
    }

    change_points_rotated(NONE);
    shadow();
}

// Check for the full lines and erase them if they exist:
void Board::erase_lines(const unsigned& complexity) {
    // initialize the indexes of the highest and lowest possible full lines
    // (according to the y coordinates of the current figure):
    int begin = y_dimension + FIGURE_GRID_HEIGHT;
    int end = FIGURE_GRID_HEIGHT;

    // calculate these indices:
    for (unsigned i = 0; i < current_figure->get_points().size(); i++) {
        if (begin > current_figure->get_points()[i]->get_y())
            begin = current_figure->get_points()[i]->get_y();
        if (end < current_figure->get_points()[i]->get_y())
            end = current_figure->get_points()[i]->get_y();
    }

    // full lines counter:
    int full_lines = 0;
    // second full lines counter in case of a gap
    // between full lines:
    unsigned previous_full_lines = 0;
    // the index of the lowest full line :
    int lowest_line = 0;
    // full line trigger:
    bool is_full = true;

    // starting check from the first visible line:
    for (int i = begin; i <= end; i++) {
        for (int j = 0; j < x_dimension; j++) {
            // if a current cell is occupied - do not touch a trigger:
            if (map[j][i] >= STOP_FIGURE_FACTOR)
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
                for (int i = lowest_line; i > 0; i--)
                    for(int j = 0; j < x_dimension; j++)
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
        for (int i = lowest_line; i > 0; i--)
            for(int j = 0; j < x_dimension; j++)
                // lowering existent lines:  
                if (i >= full_lines)
                    map[j][i] = map[j][i - full_lines];
                // set the value of the cell out of our dimension as 0:
                else
                    map[j][i] = 0;
    }

    // change score value according to the number of erased lignes and also complexity:
    switch (full_lines + previous_full_lines) {
        // if no line has been erased - adding 4 points 
        // (like the figure descent reward),
        // which is equal to the figure size:
        case 0:
            score += 4 + 2 * (complexity - 1);
            break;
        case 1:
            score += 40 * complexity;
            break;
        case 2:
            score += 100 * complexity;
            break;
        case 3:
            score += 300 * complexity;
            break;
        case 4:
            score += 1200 * complexity;
            break;
        default:
            break;
    }
};

// Fix current figure's position on the board if we can't move further:
void Board::fix_current_figure() {
    for (unsigned i = 0; i < current_figure->get_points().size(); i++)
        change_point(*current_figure->get_points()[i], current_figure->get_color_code() * STOP_FIGURE_FACTOR);     
}

// end of game condition:
bool Board::game_over() {
    for (int j = 0; j < x_dimension; j++)
        if (map[j][FIGURE_GRID_HEIGHT] >= STOP_FIGURE_FACTOR && map[j][FIGURE_GRID_HEIGHT] != SHADOW_COLOR_CODE)
             return true;
    
    return false;
};

Board::~Board() {};

void Board::print(){
    for (int i = FIGURE_GRID_HEIGHT; i < y_dimension; i++) {
        cout << "| ";
        for (int j = 0; j < x_dimension; j++) {
            if(map[j][i] < (unsigned) x_dimension)
                cout << " " << map[j][i] << " "; 
            else
                cout << map[j][i] << " "; 
        };
        cout << " |\n";
    };
}
ostream& operator <<(ostream& stream, const Board& board) {
    // display a game board starting from the 2nd x coordinate,
    // indices 0 and 1 are reserver for new figure creation zone:
    for (int i = FIGURE_GRID_HEIGHT; i < board.y_dimension; i++) {
        stream << "| ";
        for (int j = 0; j < board.x_dimension; j++) {
            if(board.map[j][i] < (unsigned) board.get_x_dim())
                stream << " " << board.map[j][i] << " "; 
            else
                stream << board.map[j][i] << " "; 
        };
        stream << " |\n";
    };

    return stream;
};
