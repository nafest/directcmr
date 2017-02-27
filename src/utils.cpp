#include "utils.h"

std::vector<std::string> split_string(const std::string &in,
                                      const char split_char) {
    std::vector<std::string> result;
    auto pos = in.find(split_char);
    std::string::size_type start_pos = 0;
    while (pos != std::string::npos) {
        auto word = in.substr(start_pos, pos - start_pos);
        if (word.length())
            result.push_back(word);
        start_pos = pos + 1;
        pos = in.find(split_char, start_pos);
    }
    if (start_pos < in.length())
        result.push_back(in.substr(start_pos));

    return result;
}
