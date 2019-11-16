#include <iostream>
#include <stdio.h>

using namespace std;

class Board {
    private:
        bool **map;
        int dimension[2];

        friend ostream& operator <<(ostream& stream, const Board& board);
    public:
        Board();

        ~Board();        
};