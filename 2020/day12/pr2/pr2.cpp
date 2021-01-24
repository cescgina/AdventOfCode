#include <map>
#include <set>
#include <queue>
#include <vector>
#include <string>
#include <numeric>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <unistd.h>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

std::map<int, std::pair<int, int>> rotations = {
    std::make_pair(90, std::make_pair(0, 1)), std::make_pair(180, std::make_pair(-1, 0)),
    std::make_pair(-90, std::make_pair(0, -1)), std::make_pair(-180, std::make_pair(-1, 0)),
    std::make_pair(270, std::make_pair(0, -1)), std::make_pair(-270, std::make_pair(0, 1))
};
bool DEBUG = false;

void advance(int& x, int& y, int waypoint_x, int waypoint_y, int magnitude){
    x += magnitude*waypoint_x;
    y += magnitude*waypoint_y;
}

void turn_right(int& x, int& y, int magnitude){
    int cp_x = x, cp_y = y;
    auto variation = rotations[magnitude];
    x = variation.first*cp_x+variation.second*cp_y;
    y = -variation.second*cp_x+variation.first*cp_y;
}

void turn_left(int& x, int &y, int magnitude){
    turn_right(x, y, -magnitude);
}

int main(int argc, char** argv){
    std::string file_name;
    getopt(argc, argv, "");
    file_name = std::string(argv[optind]);
    std::ifstream input_file(file_name);
    int x = 0, y = 0;
    int x_way = 10, y_way = 1;
    for (std::string line; std::getline(input_file, line);){
        char action = line[0];
        int magnitude = std::stoi(line.substr(1));
        if (DEBUG) std::cout << line << " before " << x << "," << y << " waypoint " << x_way << "," << y_way;
        switch (action){
            case 'E':
                x_way+=magnitude;
                break;
            case 'W':
                x_way-=magnitude;
                break;
            case 'S':
                y_way-=magnitude;
                break;
            case 'N':
                y_way+=magnitude;
                break;
            case 'R':
                turn_right(x_way, y_way, magnitude);
                break;
            case 'L':
                turn_left(x_way, y_way, magnitude);
                break;
            case 'F':
                advance(x, y, x_way, y_way, magnitude);
        }
        if (DEBUG) std::cout << " after " << x << "," << y << " waypoint " << x_way << "," << y_way << std::endl;
    }
    input_file.close();
    std::cout << std::abs(x)+std::abs(y) << std::endl;
}
