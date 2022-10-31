#include <fmt/format.h>

#include <filesystem>
#include <fstream>
#include <initializer_list>
#include <nlohmann/json.hpp>
#include <string>
#include <vector>

#include "cliColors.hpp"
using namespace nlohmann;
#define saveVal(val) mainJs[#val] = cfg.val;
#define loadVal(val, type) cfg.val = mainJs[#val].get<type>();

class Config {
    static const std::string filename;
    json mainJs;
    struct {
        size_t deltaTime = 200;
        cliColors::Colors color{cliColors::_default};
    } cfg;

   public:
    Config() { std::filesystem::exists(filename) ? load() : init(); }

    Config(Config&&) = delete;
    Config(const Config&) = delete;
    Config& operator=(const Config&) = delete;

    typedef decltype(cfg) cfg_t;
    void init() {
        saveVal(deltaTime);
        saveVal(color);
        std::ofstream(filename) << mainJs;
    }

    void load() {
        mainJs.clear();
        std::ifstream(filename) >> mainJs;
        loadVal(deltaTime, size_t);
        loadVal(color, cliColors::Colors);
    }
    const auto* get() const { return &cfg; }
};
const std::string Config::filename = "matrixCfg.json";