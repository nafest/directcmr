#include "document.h"

#include "skia_backend.h"

#include <chrono>
#include <iostream>

using bench_clock = std::chrono::high_resolution_clock;

int main(int argc, const char *argv[]) {
    if (argc < 2) {
        std::cout << "no file given." << std::endl;
        return 1;
    }

    bool be_verbose = false;
    std::vector<std::string> file_names;
    // parse the arguments
    for (int i = 1; i < argc; i++) {
        std::string arg_string(argv[i]);
        if (arg_string[0] == '-') {
            if (arg_string == "-v")
                be_verbose = true;
        } else { // everything with a dash is a filename
            file_names.push_back(arg_string);
        }
    }

    for (auto file : file_names) {
        skia_backend backnd;

        auto d = dcmr::document::from_file(file, be_verbose);
        d.set_backend(&backnd);
        auto start_time = bench_clock::now();
        auto layout_height = d.layout(600);
        auto layout_time = bench_clock::now() - start_time;

        start_time = bench_clock::now();
        d.render(dcmr::vec2(0, 0), 800);
        auto render_time = bench_clock::now() - start_time;

        std::cout << "Processed " << file << std::endl;
        std::cout << "Layout height: " << layout_height << std::endl;
        std::cout << "Layout time: "
                  << std::chrono::duration_cast<std::chrono::microseconds>(
                         layout_time)
                         .count()
                  << " microseconds." << std::endl;
        std::cout << "Render time: "
                  << std::chrono::duration_cast<std::chrono::microseconds>(
                         render_time)
                         .count()
                  << " microseconds." << std::endl;

        auto dot_pos = file.find_last_of('.');
        std::string out_name = file.substr(0, dot_pos) + ".png";

        backnd.dump_canvas(out_name);
    }
    return 0;
}
