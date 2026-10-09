#include "System.h"
#include <windows.h>
#include <conio.h>
#include <cstdlib>

System::System() {
    left = top = width = height = 0;
    SetConsoleOutputCP(CP_UTF8);
    std::srand(GetTickCount());
}

bool System::start() {
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO info;
    if (!GetConsoleScreenBufferInfo(console, &info)) return false;

    left = info.srWindow.Left;
    top = info.srWindow.Top;
    width = info.srWindow.Right - left + 1;
    height = info.srWindow.Bottom - top + 1;
    if (width < 3) return false;

    DWORD written;
    DWORD size = info.dwSize.X * info.dwSize.Y;
    COORD pos = {0, 0};
    FillConsoleOutputCharacterA(console, ' ', size, pos, &written);
    FillConsoleOutputAttribute(console, info.wAttributes, size, pos, &written);
    return true;
}

void System::draw(int x, int y, char letter, int color) {
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos;
    pos.X = left + x;
    pos.Y = top + y;
    WORD attribute = color;
    DWORD written;
    WriteConsoleOutputCharacterA(console, &letter, 1, pos, &written);
    WriteConsoleOutputAttribute(console, &attribute, 1, pos, &written);
}

void System::wait(int speed) {
    Sleep(1000 / speed);
}

bool System::stop() {
    if (_kbhit()) {
        if (_getch() == 27) return true;
    }
    return false;
}

int System::random(int from, int to) {
    return from + std::rand() % (to - from + 1);
}
