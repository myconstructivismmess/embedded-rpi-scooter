#include "Color.hpp"

Color::Color() {
    _red = 0;
    _green = 0;
    _blue = 0;
}
Color::Color(int color) {
    _red = (color >> 16) & 0xFF;
    _green = (color >> 8) & 0xFF;
    _blue = color & 0xFF;
    cout << "Color: " << (int)_red << ", " << (int)_green << ", " << (int)_blue << endl;
}
Color::Color(unsigned char red, unsigned char green, unsigned char blue) {
    _red = red;
    _green = green;
    _blue = blue;
}

Color::~Color() {

}

const Color Color::BRIGHT_BLACK = Color(0x000000);
const Color Color::BRIGHT_WHITE = Color(0xFFFFFF);

const Color Color::BRIGHT_YELLOW = Color(0xFFFF00);
const Color Color::BRIGHT_CYAN = Color(0x00FFFF);
const Color Color::BRIGHT_MAGENTA = Color(0xFF00FF);

const Color Color::BRIGHT_RED = Color(0xFF0000);
const Color Color::BRIGHT_GREEN = Color(0x00FF00);
const Color Color::BRIGHT_BLUE = Color(0x0000FF);
