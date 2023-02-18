#pragma once
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <sstream>
#include <string>
#include <thread>

#define RANDOM_TEMPLATES
#include "Random.hpp"
#include "Screen.hpp"
#include "cliColors.hpp"
using namespace cliColors;
using namespace nlohmann;
#define saveVal(val) mainJson[#val] = val;
#define loadVal(val) mainJson[#val].get_to(val);

class Matrix {
   private:
    static struct Config {
        static const std::string filename;
        static size_t deltaTime;
        static json mainJson;
        static cliColors::Colors color;
        static void init() {
            std::filesystem::exists(filename) ? load() : save();
        }
        static void save() {
            saveVal(deltaTime);
            saveVal(color);
            std::ofstream(filename) << mainJson;
        }
        static void load() {
            std::ifstream(filename) >> mainJson;
            loadVal(deltaTime);
            loadVal(color);
        }
    } cfg;

   public:
    static void run() {
        Config::init();
        Screen::init([] -> std::string {
            std::stringstream sstr;
            static const uint16_t size = Screen::getSize().ws_col;
            for (size_t w = 0; w < size; ++w) {
                const bool isSpace = !Random<uint8_t>::getRand(0, 2);  // if rand returns 0, isSpace becomes true
                if (isSpace) {
                    sstr << ' ';
                    continue;
                }
                const char symbol = Random<char>::getRand('!', '}');
                auto color = cfg.color;
                if (color == _default)
                    color = static_cast<Colors>(Random<uint8_t>::getRand(red, white));
                sstr << ColorTxt::GetColor(color) << symbol;
            }
            return sstr.str();
        });

        while (true) {
            if (system("clear") != 0)
                return;
            Screen::update();
            Screen::draw();
            std::this_thread::sleep_for(std::chrono::milliseconds(cfg.deltaTime));
        }
    }
};

const std::string Matrix::Config::filename = "matrixcfg.json";
json Matrix::Config::mainJson{};
size_t Matrix::Config::deltaTime{200};
cliColors::Colors Matrix::Config::color{_default};