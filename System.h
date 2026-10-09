#pragma once

class System {
    int left, top;

public:
    int width, height;

    System();
    bool start();
    void draw(int x, int y, char letter, int color);
    void wait(int speed);
    bool stop();
    int random(int from, int to);
};
