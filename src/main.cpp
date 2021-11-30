// MIT License
//
// Copyright (c) 2021 Xi Chen
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include <optional>
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <io2d.h>
#include "route_model.h"
#include "render.h"
#include "route_planner.h"

using namespace std::experimental;

static std::optional <std::vector<std::byte>> ReadFile(const std::string &path) {
    std::ifstream is{path, std::ios::binary | std::ios::ate};
    if (!is)
        return std::nullopt;

    auto size = is.tellg();
    std::vector <std::byte> contents(size);

    is.seekg(0);
    is.read((char *) contents.data(), size);

    if (contents.empty())
        return std::nullopt;
    return std::move(contents);
}

float GetInput() {
    float input = 0.0f;
    std::cin >> input;
    while (std::cin.fail()) {
        std::cout << "Invalid input. Try again." << std::endl;
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin >> input;
    }
    return input;
}

bool CheckRange(float user_input_x, float user_input_y) {
    return 0.0f <= user_input_x && user_input_x <= 100.0f &&
           0.0f <= user_input_y && user_input_y <= 100.0f;
}


int main(int argc, const char **argv) {
    std::string osm_data_file = "";
    if (argc > 1) {
        for (int i = 1; i < argc; ++i)
            if (std::string_view{argv[i]} == "-f" && ++i < argc)
                osm_data_file = argv[i];
    } else {
        std::cout << "To specify a map file use the following format: " << std::endl;
        std::cout << "Usage: [executable] [-f filename.osm]" << std::endl;
        osm_data_file = "../map.osm";
    }

    std::vector <std::byte> osm_data;

    if (osm_data.empty() && !osm_data_file.empty()) {
        std::cout << "Reading OpenStreetMap data from the following file: " << osm_data_file << std::endl;
        auto data = ReadFile(osm_data_file);
        if (!data)
            std::cout << "Failed to read." << std::endl;
        else
            osm_data = std::move(*data);
    }
    float start_x = 0.0f;
    float start_y = 0.0f;
    float end_x = 0.0f;
    float end_y = 0.0f;
    while (true) {
        std::cout << "Please input the x coordinate of the start point (value must between 0 and 100)" << std::endl;
        start_x = GetInput();
        std::cout << "Please input the y coordinate of the start point (value must between 0 and 100)" << std::endl;
        start_y = GetInput();
        std::cout << "Please input the x coordinate of the end point (value must between 0 and 100)" << std::endl;
        end_x = GetInput();
        std::cout << "Please input the y coordinate of the end point (value must between 0 and 100)" << std::endl;
        end_y = GetInput();
        if (CheckRange(start_x, start_y) && CheckRange(end_x, end_y)) break;
        std::cout << "Every coordinate must be between 0 and 100. Please try again." << std::endl;
    }

    // Build Model.
    RouteModel model{osm_data};

    // Create RoutePlanner object and perform A* search.
    RoutePlanner route_planner{model, start_x, start_y, end_x, end_y};
    route_planner.AStarSearch();

    std::cout << "Distance: " << route_planner.GetDistance() << " meters. \n";

    // Render results of search.
    Render render{model};

    auto display = io2d::output_surface{400, 400, io2d::format::argb32, io2d::scaling::none, io2d::refresh_style::fixed,
                                        30};
    display.size_change_callback([](io2d::output_surface &surface) {
        surface.dimensions(surface.display_dimensions());
    });
    display.draw_callback([&](io2d::output_surface &surface) {
        render.Display(surface);
    });
    display.begin_show();
}
