#pragma once
#include <conio.h>
#include <cstdlib>
#include "Line.h"

// Менеджер выбирает место и цвет линии, управляет консолью и скоростью.
class AppManager {
    int speed, length;
    bool colorful;

public:
    AppManager(int speed, int length, bool colorful)
        : speed(speed), length(length), colorful(colorful) {}

    bool run() const {
        HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_SCREEN_BUFFER_INFO info{};
        if (!GetConsoleScreenBufferInfo(console, &info)) return false;
        WORD normal = info.wAttributes;
        DWORD written;
        DWORD cells = static_cast<DWORD>(info.dwSize.X) * info.dwSize.Y;
        FillConsoleOutputCharacterW(console, L' ', cells, COORD{0, 0}, &written);
        FillConsoleOutputAttribute(console, normal, cells, COORD{0, 0}, &written);

        std::srand(GetTickCount());
        auto stop = [] {
            return (GetAsyncKeyState(VK_ESCAPE) & 0x8000) ||
                   (_kbhit() && _getch() == 27);
        };

        while (!stop()) {
            if (!GetConsoleScreenBufferInfo(console, &info) ||
                info.srWindow.Right - info.srWindow.Left < 3) return false;

            int x = info.srWindow.Left + 1 +
                    std::rand() % (info.srWindow.Right - info.srWindow.Left - 2);
            WORD color = colorful ? static_cast<WORD>(1 + std::rand() % 15)
                                 : WORD(FOREGROUND_GREEN | FOREGROUND_INTENSITY);
            Line line(x, info.srWindow.Top, info.srWindow.Bottom, length, color);

            while (!line.finished()) {
                if (stop()) return true;
                line.step(console, normal);
                Sleep(1000 / speed);
            }
        }
        return true;
    }
};
