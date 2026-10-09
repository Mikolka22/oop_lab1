#pragma once
#include "System.h"

class Symbol {
    int x, y, color;
    char letter;

public:
    Symbol(int column, int row, char ch, int textColor);
    void show(System& console);
};
