#include <string>
#include <fstream>
#include <utility>
#include <iostream>
#include <algorithm>
#include <unordered_map>
#include <unistd.h>
#include <boost/functional/hash.hpp>

typedef std::pair<int,int> coords;
typedef std::unordered_map<coords, int, boost::hash<coords>> map_lines;

void update_straight(int x1, int x2, int y1, int y2, map_lines& lines){
    auto xs = std::minmax(x1, x2);
    auto ys = std::minmax(y1, y2);
    for (int i = xs.first; i<= xs.second; i++){
        for (int j = ys.first; j<= ys.second; j++){
            lines[std::make_pair(i, j)]++;
        }
    }
}

void update_diagonal(int x1, int x2, int y1, int y2, map_lines& lines){
    int inc_x = 1, inc_y = 1;
    if (x1 > x2) inc_x = -1; 
    if (y1 > y2) inc_y = -1; 
    while (x1 != x2 and y1 != y2){
        lines[std::make_pair(x1, y1)]++;
        x1+=inc_x;
        y1+=inc_y;
    }
    lines[std::make_pair(x2, y2)]++;
}

int main(int argc, char** argv){
    std::string file_name, line;
    getopt(argc, argv, "");
    file_name = std::string(argv[optind]);
    std::ifstream input_file(file_name);
    map_lines lines;
    while (std::getline(input_file, line)){
        auto arrow = line.find(" -> ");
        std::string begin = line.substr(0, arrow);
        std::string end = line.substr(arrow+4);
        auto comma = begin.find(",");
        int x1 = std::stoi(begin.substr(0, comma));
        int y1 = std::stoi(begin.substr(comma+1));
        comma = end.find(",");
        int x2 = std::stoi(end.substr(0, comma));
        int y2 = std::stoi(end.substr(comma+1));
        if (x1 != x2 and y1 != y2 ) {
            update_diagonal(x1, x2, y1, y2, lines);
        }
        else{
            update_straight(x1, x2, y1, y2, lines);
        }
    }
    int total = 0;
    for (auto&& point: lines){
        if (point.second > 1) total++;
    }
    std::cout << total << std::endl;
}
