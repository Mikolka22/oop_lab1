#pragma once
#define NOMINMAX
#include <windows.h>

// Один символ: его место, буква и цвет.
class Symbol {
public:
    COORD pos;
    wchar_t letter;
    WORD color;

    Symbol(COORD p, wchar_t ch, WORD c) : pos(p), letter(ch), color(c) {}

    void show(HANDLE console, WORD normal, bool erase = false) const {
        wchar_t ch = erase ? L' ' : letter;
        SetConsoleTextAttribute(console, erase ? normal : color);
        SetConsoleCursorPosition(console, pos);
        DWORD written;
        WriteConsoleW(console, &ch, 1, &written, nullptr);
        SetConsoleTextAttribute(console, normal);
    }
};
