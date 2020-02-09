#ifndef figureHPP
#define figureHPP

#include <iostream>
#include <vector>
#include "color_constants.hpp"

using namespace sf;
using namespace std;
using namespace colors;

enum figure_state {
    DESCEND_FIGURE,
    STOP_FIGURE,
    CHANGE_FIGURE
};


// figure grid size (for drawing next figures):
#define FIGURE_GRID_WIDTH 4
#define FIGURE_GRID_HEIGHT 2

#define FIGURE_O_COLOR_CODE 1
#define FIGURE_I_COLOR_CODE 2
#define FIGURE_T_COLOR_CODE 3
#define FIGURE_L_COLOR_CODE 4
#define FIGURE_J_COLOR_CODE 5
#define FIGURE_Z_COLOR_CODE 6
#define FIGURE_S_COLOR_CODE 7

class Point {
    private:
        int x;
        int y;
    public:
        Point(): x(0), y(0) {};
        Point(int x_0, int y_0): x(x_0), y(y_0) {};
        Point(const Point& start_point): x(start_point.x), y(start_point.y) {};

        void set_x(int&);
        void set_y(int&);

        void increment_x(const int& value = 1);
        void increment_y(const int& value = 1);

        void rotate(const bool&, const Point*);

        int get_x() const;
        int get_y() const;

        ~Point();
};

// General figure definition:
class Figure {
    protected:
        vector<Point *> points;
        Point* point_reference;
        unsigned color_code;
        // figure's graphical grid:
        RectangleShape grid[FIGURE_GRID_WIDTH][FIGURE_GRID_HEIGHT];
    public:
        Figure(){};
        Figure(const unsigned& code) : 
            color_code(code) {}

        vector<Point *> get_points() const;
        Point* get_point_reference() const;
        unsigned get_color_code() const;
        RectangleShape** get_grid() const;
        void set_grid(const unsigned& x_dim, const Vector2f& new_cell_size);
        
        virtual void initialize(const unsigned&, const Vector2f&) = 0;
};

class Figure_Simple_Rotation: public Figure {
    protected:
        bool already_rotated;
    public:
        Figure_Simple_Rotation(){};
        Figure_Simple_Rotation(const unsigned& code): Figure(code), already_rotated(false) {};

        bool get_next_rotation();
};

// Square figure definition:
class Figure_O: public Figure {
    public:
        Figure_O(const unsigned& x_dim, const Vector2f& cell_size): Figure(FIGURE_O_COLOR_CODE) {
            initialize(x_dim, cell_size);
        };
        
        virtual void initialize(const unsigned&, const Vector2f&);
};

// Stick figure definition:
class Figure_I: public Figure_Simple_Rotation {
    public:
        Figure_I(const unsigned& x_dim, const Vector2f& cell_size):Figure_Simple_Rotation(FIGURE_I_COLOR_CODE) {
            initialize(x_dim, cell_size);
        };
        
        virtual void initialize(const unsigned&, const Vector2f&);
};

// T figure definition:
class Figure_T: public Figure {
    public:
        Figure_T(const unsigned& x_dim, const Vector2f& cell_size):Figure(FIGURE_T_COLOR_CODE) {
            initialize(x_dim, cell_size);
        };
        
        virtual void initialize(const unsigned&, const Vector2f&);
};

// L figure definition:
class Figure_L: public Figure {
    public:
        Figure_L(const unsigned& x_dim, const Vector2f& cell_size):Figure(FIGURE_L_COLOR_CODE) {
            initialize(x_dim, cell_size);
        };
        
        virtual void initialize(const unsigned&, const Vector2f&);
};

// J figure definition:
class Figure_J: public Figure {
    public:
        Figure_J(const unsigned& x_dim, const Vector2f& cell_size):Figure(FIGURE_J_COLOR_CODE) {
            initialize(x_dim, cell_size);
        };
        
        virtual void initialize(const unsigned&, const Vector2f&);
};

// Z figure definition:
class Figure_Z: public Figure_Simple_Rotation {
    public:
        Figure_Z(const unsigned& x_dim, const Vector2f& cell_size):Figure_Simple_Rotation(FIGURE_Z_COLOR_CODE) {
            initialize(x_dim, cell_size);
        };
        
        virtual void initialize(const unsigned&, const Vector2f&);
};

// S figure definition:
class Figure_S: public Figure_Simple_Rotation {
    public:
        Figure_S(const unsigned& x_dim, const Vector2f& cell_size):Figure_Simple_Rotation(FIGURE_S_COLOR_CODE) {
            initialize(x_dim, cell_size);
        };
        
        virtual void initialize(const unsigned&, const Vector2f&);
};

#endif