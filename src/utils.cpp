#include "utils.h"

int num_lines(const std::string &in) {
    int num = 0;
    for (auto& line : line_splitter(in))
        num++;

    return num;
}
