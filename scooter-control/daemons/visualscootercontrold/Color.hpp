#ifndef COLOR_H
#define COLOR_H

#pragma once

#include <iostream>
using std::cerr;
using std::cout;
using std::endl;

class Color {
    public:
        Color();
        Color(int color);
        Color(unsigned char red, unsigned char green, unsigned char blue);
        
        ~Color();

        static const Color BRIGHT_BLACK;
        static const Color BRIGHT_WHITE;

        static const Color BRIGHT_YELLOW;
        static const Color BRIGHT_CYAN;
        static const Color BRIGHT_MAGENTA;
        
        static const Color BRIGHT_RED;
        static const Color BRIGHT_GREEN;
        static const Color BRIGHT_BLUE;
        
    private:
        unsigned char _red;
        unsigned char _green;
        unsigned char _blue;
};

#endif