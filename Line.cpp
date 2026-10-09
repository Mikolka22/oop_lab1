#include "Line.h"
#include "Symbol.h"

Line::Line(int column, int size, int textColor) {
    x = column;
    length = size;
    color = textColor;
    number = 0;
}

bool Line::step(System& console) {
    // Семь букв ромба: A / B C / D / E F / G.
    int dx[7] = {0, -1, 1, 0, -1, 1, 0};
    int dy[7] = {0, 1, 1, 2, 3, 3, 4};

    if (number >= length) {
        int old = number - length;
        int y = old / 7 * 5 + dy[old % 7];
        if (y >= console.height) return false;
        Symbol tail(x + dx[old % 7], y, ' ', color);
        tail.show(console);
    }

    int y = number / 7 * 5 + dy[number % 7];
    if (y < console.height) {
        char letter = 'A' + number % 26;
        Symbol head(x + dx[number % 7], y, letter, color);
        head.show(console);
    }
    ++number;
    return true;
}
