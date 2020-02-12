#ifndef boardHPP
#define boardHPP

#include "figures.hpp"

// board grid size:
#define BOARD_GRID_WIDTH 10
#define BOARD_GRID_HEIGHT 20

#define MAX_NICKNAME_LENGTH 12

#define STOP_FIGURE_FACTOR 10
#define SHADOW_COLOR_CODE 101

/**
 * @brief possible out-of-border states.
 * 
 */
enum overflow {
    NONE,
    OVERFLOW_RIGHT,
    OVERFLOW_LEFT,
    OVERFLOW_DOWN,
    OVERFLOW_UP
};

/**
 * @brief Class that implements player's game board,
 * its current and next figures, score and level.
 * 
 */
class Board {
    private:
        // game board map - two-dimensional matrix
        // our future figures are going to move through:
        unsigned **map;
        // graphic game board:
        RectangleShape **grid;
        // game board complexity:
        unsigned complexity;
        // 2D size of each cell:
        Vector2f cell_size;
        // x (horizontal) board dimension:
        int x_dimension;
        // y (vertical) board dimension:
        int y_dimension;
        // Score of the match:
        long score;
        // The figure which is present on the map and moving alongside the game board right now:
        Figure* current_figure;
        // The figure which will be added on the game board after the fall of the current feafure:
        Figure* next_figure;
        // Using a thread to fall down:
        Thread* descend_thread;
        // Shadow points
        vector<Point> shadow_points;

        /**
         * @brief Overwritted operator to output the game board map on the console.
         * 
         * @param stream 
         * @param board 
         * @return ostream& 
         */
        friend ostream& operator <<(ostream& stream, const Board& board);

        /**
         * @brief Current function is dedicated to check the availability
         * to move current figure to game board map cell
         * according to its coordinates and value. 
         * 
         * @param point 
         * @return overflow 
         */
        overflow is_empty(const Point& point) const;
        /**
         * @brief Function to move current figure's points during its rotation.
         * 
         */
        void change_points_rotated(const overflow&);
        /**
         * @brief Function to determine the threshold of the cell with current coordinates.
         * 
         * @param point 
         * @return true 
         * @return false 
         */
        bool has_floor(const Point& point) const;
        void print();
    public:
        Board(RenderWindow& window, const unsigned& initial_complexity, const Vector2f& initial_cell_size);

        /**
         * @brief Game board initialization function which is called from the constructor.
         * 
         * @param window 
         * @param initial_complexity 
         * @param initial_cell_size 
         */
        void initialize(RenderWindow& window, const unsigned& initial_complexity, const Vector2f& initial_cell_size);
        /**
         * @brief Create a new figure object that belongs to current board.
         * 
         * @return Figure* 
         */
        Figure* create_figure();
        /**
         * @brief Function to add current figure on top of the board.
         * 
         */
        void add_figure();
        
        unsigned** get_map() const;
        unsigned get_complexity();
        int get_x_dim() const;
        int get_y_dim() const;
        long get_score() const;
        Figure* get_next_figure() const;
        Thread* get_descend_thread() const;
        
        void set_map(const unsigned new_map[BOARD_GRID_WIDTH][BOARD_GRID_HEIGHT + FIGURE_GRID_HEIGHT]);
        void set_complexity(const unsigned& new_complexity);
        void set_cell_size(const Vector2f& new_cell_size);
        void set_score(const long& new_score);
        void set_current_figure(Figure* figure);
        void set_next_figure(Figure* figure);
        void set_descend_thread(Thread*);

        /**
         * @brief Function to increase complexity of the game (level up).
         * 
         * @param increment_value 
         */
        void increment_complexity(const int& increment_value = 1);

        
        /**
         * @brief Draws game board, next figure, current score and level on a current window. 
         * 
         * @param window 
         * @param font 
         * @param font_size 
         * @param nickname 
         * @param game_is_over 
         * @param board_index 
         */
        void print_board(RenderWindow& window, const Font& font, const double& font_size, const string& nickname = "", const bool& game_is_over = false, const unsigned& board_index = 0);

        /**
         * @brief Function to change some value of the game board map.
         * 
         * @param point 
         * @param new_value 
         * @param from_rotation 
         */
        void change_point(const Point& point, const int& new_value = 0, const bool& from_rotation = false);
        /**
         * @brief Moves current figure one step downwards through the board if it is possible.
         * 
         * @return true 
         * @return false 
         */
        bool step_down();
        /**
         * @brief Moves current figure downwards through the board untill the reach stop condition.
         * 
         */
        void hard_drop();
        /**
         * @brief Moves current figure one step to the left through the board if it is possible.
         * 
         * @param with_floor 
         * @return true 
         * @return false 
         */
        bool step_left(const bool& with_floor = false);
        /**
         * @brief Moves current figure one step to the right through the board if it is possible.
         * 
         * @param with_floor 
         * @return true 
         * @return false 
         */
        bool step_right(const bool& with_floor = false);
        /**
         * @brief Rotates current figure 90 degrees in the chosen orientation.
         * 
         * @param right 
         */
        void rotate(bool right);
        /**
         * @brief Displays the shadow of the current figure - its possible position on the "bottom" of a board.
         * 
         */
        void shadow();
        // Check for the full lines and erase them if they are:
        /**
         * @brief Function to check the board for the full lines, 
         * erase them if they are and increase player's score 
         * in accordance with the number of erased lines.
         * 
         * @return unsigned 
         */
        unsigned erase_lines();
        /**
         * @brief Fixes current figure's position on the board if the stop condition was reached.
         * 
         */
        void fix_current_figure();
        /**
         * @brief Function to check game over condition - presence of at least one occupied fixed cell in the highest row. 
         * 
         * @return true 
         * @return false 
         */
        bool game_over();

        ~Board();        
};

#endif