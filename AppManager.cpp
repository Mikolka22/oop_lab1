#include "AppManager.h"
#include "System.h"
#include "Line.h"
#include <iostream>

int AppManager::number(std::string text) {
    int value = 0;
    if (text.empty()) return 0;
    for (unsigned int i = 0; i < text.size(); ++i) {
        if (text[i] < '0' || text[i] > '9') return 0;
        value = value * 10 + text[i] - '0';
        if (value > 30) return 0;
    }
    return value;
}

int AppManager::readNumber(const char* prompt) {
    std::string text;
    int value;
    do {
        std::cout << prompt;
        if (!std::getline(std::cin, text)) return 0;
        value = number(text);
        if (value == 0) std::cout << "Введите целое число от 1 до 30.\n";
    } while (value == 0);
    return value;
}

int AppManager::run(int argc, char* argv[]) {
    System console;
    std::string mode;
    if (argc == 2) {
        std::string help = argv[1];
        if (help == "--help" || help == "/?") {
            std::cout << "matrix.exe [скорость длина Y|N]\n"
                      << "Скорость и длина: 1..30. Без параметров — диалог.\n"
                      << "Y: цвет всей линии случайный, N: зелёный. Выход: Esc или Ctrl+C.\n";
            return 0;
        }
    }

    if (argc == 1) {
        speed = readNumber("Скорость (1..30): ");
        if (speed == 0) return 1;
        length = readNumber("Длина (1..30): ");
        if (length == 0) return 1;
        do {
            std::cout << "Режим (Y/N): ";
            if (!std::getline(std::cin, mode)) return 1;
            if (mode != "Y" && mode != "y" && mode != "N" && mode != "n") {
                std::cout << "Введите Y или N.\n";
            }
        } while (mode != "Y" && mode != "y" && mode != "N" && mode != "n");
    } else if (argc == 4) {
        speed = number(argv[1]);
        length = number(argv[2]);
        mode = argv[3];
    } else {
        std::cout << "Нужны три параметра. Справка: matrix.exe --help\n";
        return 1;
    }

    if (speed == 0 || length == 0 ||
        (mode != "Y" && mode != "y" && mode != "N" && mode != "n")) {
        std::cout << "Неверные параметры. Справка: matrix.exe --help\n";
        return 1;
    }
    colorful = (mode == "Y" || mode == "y");
    if (!console.start()) {
        std::cout << "Нужна консоль Windows шириной хотя бы 3 символа.\n";
        return 1;
    }

    while (true) {
        int x = console.random(1, console.width - 2);
        int color = 10;
        if (colorful) color = console.random(1, 15);
        Line line(x, length, color);
        while (true) {
            if (console.stop()) return 0;
            if (!line.step(console)) break;
            console.wait(speed);
        }
    }
}
