#include <iostream>
#include <stdio.h>

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

        int get_x() const;
        int get_y() const;

        ~Point();
};

class Board {
    private:
        bool **map;
        unsigned x_dimension;
        unsigned y_dimension;

        friend ostream& operator <<(ostream& stream, const Board& board);
    public:
        Board();

        unsigned get_x_dim() const;
        unsigned get_y_dim() const;

        bool is_empty(const Point& point) const;
        void change_point(const Point& point);

        ~Board();        
};