#include "utils.h"
#include "renderer.h"

color string_to_color(const std::string &str) {
    color c(0, 0, 0, 255);
    if (str.length() < 7 || str[0] != '#')
        return c;

    try {
        c.r = static_cast<unsigned char>(std::strtoul(str.substr(1, 2).c_str(), nullptr, 16));
        c.g = static_cast<unsigned char>(std::strtoul(str.substr(3, 2).c_str(), nullptr, 16));
        c.b = static_cast<unsigned char>(std::strtoul(str.substr(5, 2).c_str(), nullptr, 16));
    } catch (...) {
        return color(0, 0, 0, 255);
    }

    try {
      c.a = static_cast<unsigned char>(std::strtoul(str.substr(7, 2).c_str(), nullptr, 16));
    } catch (...) {
        c.a = 255;
    }

    return c;
}

int num_lines(const std::string &in) {
    int num = 0;
    for (auto &line : line_splitter(in))
        num++;

    return num;
}
