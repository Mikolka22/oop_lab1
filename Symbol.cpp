#include "Symbol.h"

Symbol::Symbol(int column, int row, char ch, int textColor) {
    x = column;
    y = row;
    letter = ch;
    color = textColor;
}

void Symbol::show(System& console) {
    console.draw(x, y, letter, color);
}
