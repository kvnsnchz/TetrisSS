#include "figures.hpp"

enum overflow {
    NONE,
    OVERFLOW_RIGHT,
    OVERFLOW_LEFT,
    OVERFLOW_DOWN,
    OVERFLOW_UP
};

class Board {
    private:
        // game board map - two-dimensional matrix
        // our future figures are going to move through:
        unsigned **map;
        // graphic game board:
        RectangleShape **grid;
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

        friend ostream& operator <<(ostream& stream, const Board& board);

        overflow is_empty(const Point& point) const;
        void change_points_rotated(const overflow&);
        bool has_floor(const Point& point) const;
        void print();
    public:
        Board(RenderWindow& window, const Vector2f& initial_cell_size);

        // game board initialization:
        void initialize(RenderWindow& window, const Vector2f& initial_cell_size);
        // new figure creation function:
        Figure* create_figure();
        // adding figure on top of the board:
        void add_figure();
        
        void set_cell_size(const Vector2f& new_cell_size);

        int get_x_dim() const;
        int get_y_dim() const;

        long get_score() const;

        Figure* get_next_figure() const;
        void set_current_figure(Figure* figure);
        void set_next_figure(Figure* figure);

        // draw game board:
        void print_board(RenderWindow& window, const Font& font, const double& font_size);

        void change_point(const Point& point, const int& new_value = 0, const bool& from_rotation = false);
        bool step_down();
        bool step_left(const bool& with_floor = false);
        bool step_right(const bool& with_floor = false);
        void rotate(bool right);
        // Check for the full lines and erase them if they are:
        void erase_lines(const unsigned& complexity);
        void insert_figure_current();
        // adding game over condition function:
        bool game_over();

        ~Board();        
};