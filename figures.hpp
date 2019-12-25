#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>

using namespace sf;
using namespace std;

class Point {
    private:
        unsigned x;
        unsigned y;
    public:
        Point(): x(0), y(0) {};
        Point(unsigned x_0, unsigned y_0): x(x_0), y(y_0) {};
        Point(const Point& start_point): x(start_point.x), y(start_point.y) {};

        void set_x(unsigned x_new);
        void set_y(unsigned y_new);

        void increment_x(const unsigned& value = 1);
        void increment_y(const unsigned& value = 1);

        unsigned get_x() const;
        unsigned get_y() const;

        ~Point();
};

// General figure definition:
class Figure {
    protected:
        vector<Point *> points;
        unsigned color_code;
        // figure's graphical grid:
        RectangleShape grid[4][2];
    public:
        Figure(){};
        Figure(const unsigned& code) : color_code(code) {}

        vector<Point *> get_points() const;
        unsigned get_color_code() const;
        RectangleShape** get_grid() const;
        
        virtual void initialize(unsigned) = 0;

        // print our figure:
        virtual void print() = 0;
        
};

// Square figure definition:
class Figure_O: public Figure {
    public:
        Figure_O(const unsigned& x_dim): Figure(1) {
            initialize(x_dim);
        };
        
        virtual void initialize(unsigned);

        virtual void print();
};

// Stick figure definition:
class Figure_I: public Figure {
    public:
        Figure_I(const unsigned& x_dim):Figure(2) {
            initialize(x_dim);
        };
        
        virtual void initialize(unsigned);

        virtual void print();
};

// T figure definition:
class Figure_T: public Figure {
    public:
        Figure_T(const unsigned& x_dim):Figure(3) {
            initialize(x_dim);
        };
        
        virtual void initialize(unsigned);

        virtual void print();
};

// L figure definition:
class Figure_L: public Figure {
    public:
        Figure_L(const unsigned& x_dim):Figure(4) {
            initialize(x_dim);
        };
        
        virtual void initialize(unsigned);

        virtual void print();
};

// J figure definition:
class Figure_J: public Figure {
    public:
        Figure_J(const unsigned& x_dim):Figure(5) {
            initialize(x_dim);
        };
        
        virtual void initialize(unsigned);

        virtual void print();
};

// Z figure definition:
class Figure_Z: public Figure {
    public:
        Figure_Z(const unsigned& x_dim):Figure(6) {
            initialize(x_dim);
        };
        
        virtual void initialize(unsigned);

        virtual void print();
};

// S figure definition:
class Figure_S: public Figure {
    public:
        Figure_S(const unsigned& x_dim):Figure(7) {
            initialize(x_dim);
        };
        
        virtual void initialize(unsigned);

        virtual void print();
};