#include <iostream>
#include <vector>

using namespace std;

class Point {
    private:
        int x;
        int y;
    public:
        Point(): x(0), y(0) {};
        Point(int x_0, int y_0): x(x_0), y(y_0) {};
        Point(const Point& start_point): x(start_point.x), y(start_point.y) {};

        void set_x(int x_new);
        void set_y(int y_new);

        void increment_x(const unsigned& value = 1);
        void increment_y(const int& value = 1);

        int get_x() const;
        int get_y() const;

        ~Point();
};

// General figure definition:
class Figure {
    protected:
        vector<Point *> points;
        unsigned color_code;
    public:
        Figure(){};
        Figure(const unsigned& code) : color_code(code) {}

        vector<Point *> get_points() const;
        unsigned get_color_code() const;
        virtual void initialize(unsigned) = 0;
};

// Square definition:
class Figure_O: public Figure {
    public:
        Figure_O(const unsigned& y_dim):Figure(1) {
            initialize(y_dim);
        };
        
        virtual void initialize(unsigned);

};