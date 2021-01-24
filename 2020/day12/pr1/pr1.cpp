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

bool DEBUG = false;

void advance(int& x, int& y, int direction, int magnitude){
    switch (direction){
        case 0:
            y-=magnitude;
            break;
        case 1:
            x+=magnitude;
            break;
        case 2:
            y+=magnitude;
            break;
        case 3:
            x-=magnitude;
            break;
    }
}

void turn_right(int& direction, int magnitude){
    magnitude /= 90;
    direction = (direction+magnitude)%4;
}

void turn_left(int& direction, int magnitude){
    magnitude /= 90;
    turn_right(direction, 90*(4-(magnitude%4)));
}

int main(int argc, char** argv){
    std::string file_name;
    getopt(argc, argv, "");
    file_name = std::string(argv[optind]);
    std::ifstream input_file(file_name);
    int x = 0, y = 0;
    //start facing east;
    int direction = 1;
    for (std::string line; std::getline(input_file, line);){
        char action = line[0];
        int magnitude = std::stoi(line.substr(1));
        if (DEBUG) std::cout << line << " before " << x << "," << y << " " << direction;
        switch (action){
            case 'E':
                x+=magnitude;
                break;
            case 'W':
                x-=magnitude;
                break;
            case 'S':
                y+=magnitude;
                break;
            case 'N':
                y-=magnitude;
                break;
            case 'R':
                turn_right(direction, magnitude);
                break;
            case 'L':
                turn_left(direction, magnitude);
                break;
            case 'F':
                advance(x, y, direction, magnitude);
        }
        if (DEBUG) std::cout << " after " << x << "," << y << " " << direction << std::endl;
    }
    input_file.close();
    std::cout << std::abs(x)+std::abs(y) << std::endl;
}
