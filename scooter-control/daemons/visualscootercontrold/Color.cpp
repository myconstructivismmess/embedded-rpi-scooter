#include "Color.hpp"

Color::Color() {
    red = 0;
    green = 0;
    blue = 0;
}
Color::Color(int color) {
    red = (color >> 16) & 0xFF;
    green = (color >> 8) & 0xFF;
    blue = color & 0xFF;
}
Color::Color(unsigned char redValue, unsigned char greenValue, unsigned char blueValue) {
    red = redValue;
    green = greenValue;
    blue = blueValue;
}

Color Color::mix(Color color1, Color color2, float ratio) {
    if (ratio < 0) {
        cerr << "Color::mix: Ratio cannot be less than 0, setting it to 0" << endl;
    } else if (ratio > 1) {
        cerr << "Color::mix: Ratio cannot be greater than 1, setting it to 1" << endl;
    }

    ratio = std::max(0.0f, std::min(1.0f, ratio));

    unsigned char newRed = (color1.red * (1 - ratio)) + (color2.red * ratio);
    unsigned char newGreen = (color1.green * (1 - ratio)) + (color2.green * ratio);
    unsigned char newBlue = (color1.blue * (1 - ratio)) + (color2.blue * ratio);

    return Color(newRed, newGreen, newBlue);
}

Color Color::switchColor(vector<Color> colors, size_t index) {
    if (index >= colors.size()) {
        cerr << "Color::switchColor: Index is greater than the size of the array, setting it to 0" << endl;
        index = 0;
    }

    return colors[index];
}

int Color::toHex(Color color) {
    return (color.red << 16) | (color.green << 8) | color.blue;
}
int Color::toHex() {
    return (red << 16) | (green << 8) | blue;
}

const Color Color::BLACK = Color(0x000000);
const Color Color::WHITE = Color(0xFFFFFF);

const Color Color::BRIGHT_YELLOW = Color(0xFFFF00);
const Color Color::BRIGHT_CYAN = Color(0x00FFFF);
const Color Color::BRIGHT_MAGENTA = Color(0xFF00FF);

const Color Color::BRIGHT_RED = Color(0xFF0000);
const Color Color::BRIGHT_GREEN = Color(0x00FF00);
const Color Color::BRIGHT_BLUE = Color(0x0000FF);
