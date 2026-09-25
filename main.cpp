#define NOMINMAX
#include <windows.h>
#include <conio.h>
#include <cstdlib>
#include <iostream>
#include <queue>
#include <string>
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

int main(int argc, char* argv[]) {
    SetConsoleOutputCP(CP_UTF8);

    if (argc == 2 && (std::string(argv[1]) == "--help" ||
                      std::string(argv[1]) == "/?")) {
        std::cout << "matrix.exe [скорость длина Y|N]\n"
                  << "Скорость и длина: 1..30. Без аргументов — диалоговый ввод.\n"
                  << "Y: случайный цвет всей линии; N: зелёный. Выход: Esc или Ctrl+C.\n";
        return 0;
    }

    // Вернёт 0, если текст не является числом от 1 до 30.
    auto number = [](const std::string& text) {
        int value = 0;
        if (text.empty()) return 0;
        for (char ch : text) {
            if (ch < '0' || ch > '9') return 0;
            value = value * 10 + ch - '0';
            if (value > 30) return 0;
        }
        return value;
    };

    std::string input[3];
    if (argc == 1) {
        const char* prompt[3] = {"Скорость (1..30): ", "Длина (1..30): ", "Режим (Y/N): "};
        for (int i = 0; i < 3; ++i) {
            while (true) {
                std::cout << prompt[i];
                if (!std::getline(std::cin, input[i])) return 1;
                bool ok = i < 2 ? number(input[i]) != 0 :
                    input[i] == "Y" || input[i] == "y" ||
                    input[i] == "N" || input[i] == "n";
                if (ok) break;
                std::cout << "Неверное значение. Попробуйте снова.\n";
            }
        }
    } else if (argc == 4) {
        for (int i = 0; i < 3; ++i) input[i] = argv[i + 1];
    } else {
        std::cerr << "Нужны три параметра. Справка: matrix.exe --help\n";
        return 1;
    }

    int speed = number(input[0]);
    int length = number(input[1]);
    std::string mode = input[2];
    if (!speed || !length ||
        (mode != "Y" && mode != "y" && mode != "N" && mode != "n")) {
        std::cerr << "Неверные параметры. Справка: matrix.exe --help\n";
        return 1;
    }

    AppManager app(speed, length, mode == "Y" || mode == "y");
    if (!app.run()) {
        std::cerr << "Нужна консоль Windows шириной не меньше 4 символов.\n";
        return 1;
    }
    return 0;
}
