#ifndef COLOR_H
#define COLOR_H

#pragma once

#include <iostream>
using std::cerr;
using std::cout;
using std::endl;

#include <vector>
using std::vector;

class Color {
    public:
        Color();
        Color(int color);
        Color(unsigned char red, unsigned char green, unsigned char blue);

        /**
         * @brief Mixes two colors ignoring the alpha channel.
         *
         * @param color1 The first color.
         * @param color2 The second color.
         * @param ratio The ratio of the first color.
         *
         * @return The mixed color.
        */
        static Color mix(Color color1, Color color2, float ratio = 0.5f);
        /**
         * @brief Switches between colors in an array.
         *
         * @param colors The array of colors.
         * @param index The index of the color to switch to.
         *
         * @return The selected color.
        */
        static Color switchColor(vector<Color> colors, size_t index);
        /**
         * @brief Gets the color as a hexadecimal value.
         *
         * @return The color as a hexadecimal value.
        */
        static int toHex(Color color);
        int toHex();

        static const Color BLACK;
        static const Color WHITE;

        static const Color BRIGHT_YELLOW;
        static const Color BRIGHT_CYAN;
        static const Color BRIGHT_MAGENTA;

        static const Color BRIGHT_RED;
        static const Color BRIGHT_GREEN;
        static const Color BRIGHT_BLUE;

        unsigned char red;
        unsigned char green;
        unsigned char blue;
};

#endif