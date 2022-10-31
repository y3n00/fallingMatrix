#pragma once
#include <iostream>
#include <sstream>
#include <string>
#include <thread>

#include "Config.hpp"
#include "Random.hpp"
#include "Screen.hpp"
#include "Timer.hpp"
#include "cliColors.hpp"
using namespace cliColors;

class Matrix {
   private:
    static Config config;
    static const Config::cfg_t* cfg;

    static std::string generateStr() {
        std::stringstream sstr;
        static const uint16_t size = Screen::getSize().ws_col;
        for (size_t w = 0; w < size; ++w) {
            if (bool isSpace = Random::getRand(0, 3); !isSpace) {
                sstr << ' ';
                continue;
            }
            const auto symbol = static_cast<char>(Random::getRand('!', '}'));
            auto color = cfg->color;
            if (cfg->color == _default)
                color = static_cast<Colors>(Random::getRand(red, white));
            sstr << ColorTxt::GetColor(color) << symbol;
        }
        return sstr.str();
    }

   public:
    void run() {
        Screen::init(Matrix::generateStr);

        while (true) {
            if (system("clear") != 0)
                return;

            Screen::update();
            Screen::draw();
            std::this_thread::sleep_for(Timer::ms(cfg->deltaTime));
        }
    }
};

Config Matrix::config;
const Config::cfg_t* Matrix::cfg = Matrix::config.get();