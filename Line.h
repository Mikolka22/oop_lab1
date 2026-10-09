#pragma once
#include "System.h"

class Line {
    int x, length, color, number;

public:
    Line(int column, int size, int textColor);
    bool step(System& console);
};
