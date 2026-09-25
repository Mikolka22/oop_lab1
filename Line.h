#pragma once
#include <queue>
#include "Symbol.h"

// Линия добавляет новую букву и стирает старую.
class Line {
    int x, top, bottom, length, stepNumber = 0;
    WORD color;
    bool below = false;
    std::queue<Symbol> letters;

public:
    Line(int x, int top, int bottom, int length, WORD color)
        : x(x), top(top), bottom(bottom), length(length), color(color) {}

    void step(HANDLE console, WORD normal) {
        // Ромб: A / B C / D / E F / G.
        const int dx[7] = {0, -1, 1, 0, -1, 1, 0};
        const int dy[7] = {0, 1, 1, 2, 3, 3, 4};
        int part = stepNumber % 7;
        int y = top + stepNumber / 7 * 5 + dy[part];

        if (y <= bottom) {
            COORD pos{static_cast<SHORT>(x + dx[part]), static_cast<SHORT>(y)};
            letters.emplace(pos, L'A' + stepNumber % 26, color);
            letters.back().show(console, normal);
        } else {
            below = true;
        }

        if (!letters.empty() && (letters.size() > static_cast<size_t>(length) || below)) {
            letters.front().show(console, normal, true);
            letters.pop();
        }
        ++stepNumber;
    }

    bool finished() const { return below && letters.empty(); }
};
