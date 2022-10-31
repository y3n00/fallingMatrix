#pragma once
#include <sys/ioctl.h>
#include <unistd.h>

#include <iostream>
#include <ranges>
#include <string>
#include <vector>

class Screen {
   private:
    static winsize ws;
    static std::string (*generationFunc)();
    static std::vector<std::string> _screen;

   public:
    static void init(std::string (*strGen)()) {
        ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws);
        generationFunc = strGen;
        _screen.resize(ws.ws_row);
        for (auto& str : _screen)
            str = generationFunc();
    }

    static const auto getSize() { return ws; }
    static void update() {
        _screen.erase(_screen.begin());
        _screen.emplace_back(generationFunc());
    }
    static void draw() {
        for (const auto& line : _screen | std::views::reverse)
            std::cerr << line << '\n';
    }
};

winsize Screen::ws;
std::string (*Screen::generationFunc)();
std::vector<std::string> Screen::_screen;