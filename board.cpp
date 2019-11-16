#include "board.hpp"

Board::Board() {
    dimension[0] = 20;
    dimension[1] = 10;

    map = new bool *[dimension[0]];
    for (int i = 0; i < dimension[0]; i++)
        map[i] = new bool [dimension[1]];

    for (int i = 0; i < dimension[0]; i++)
        for (int j = 0; j < dimension[1]; j++)
            map[i][j] = 0;
};

Board::~Board() {};

ostream& operator <<(ostream& stream, const Board& board) {
    for (int i = 0; i < board.dimension[0]; i++) {
        stream << "| ";
        for (int j = 0; j < board.dimension[1]; j++) {
            stream << board.map[i][j]; 
        };
        stream << " |\n";
    };

    return stream;
};