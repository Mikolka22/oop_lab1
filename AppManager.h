#pragma once
#include <string>

class AppManager {
    int speed, length;
    bool colorful;

    int number(std::string text);
    int readNumber(const char* prompt);

public:
    int run(int argc, char* argv[]);
};
