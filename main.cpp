#include <iostream>
#include <string>
#include "AppManager.h"

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
