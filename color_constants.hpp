#include <SFML/Graphics.hpp>

using namespace sf; 

#ifndef COLOR_CONSTANTS
#define COLOR_CONSTANTS

namespace colors {
    // Colors of buttons:
    const Color COLOR_DARK_VIOLET = Color(144, 12, 63, 255); // also J figure's color
    const Color COLOR_LIGHT_GREEN = Color(218, 247, 166, 255);
    const Color COLOR_YELLOW = Color(255, 195, 0, 255); // also O figure's color
    const Color COLOR_DARK_BLUE = Color(8, 0, 93, 255); 

    // Colors of figures:
    const Color COLOR_CYAN = Color(28, 230, 199, 255);
    const Color COLOR_ORANGE = Color(248, 131, 6, 255);
    const Color COLOR_PINK = Color(248, 6, 248, 255);
    const Color COLOR_RED = Color(220, 0, 20, 255);
    const Color COLOR_GREEN_SALAD = Color(46, 228, 25, 255);
}

#endif