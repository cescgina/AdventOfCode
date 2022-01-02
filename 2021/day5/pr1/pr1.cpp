#include <string>
#include <fstream>
#include <utility>
#include <iostream>
#include <algorithm>
#include <unordered_map>
#include <unistd.h>
#include <boost/functional/hash.hpp>

typedef std::pair<int,int> coords;
int main(int argc, char** argv){
    std::string file_name, line;
    getopt(argc, argv, "");
    file_name = std::string(argv[optind]);
    std::ifstream input_file(file_name);
    std::unordered_map<coords, int, boost::hash<coords>> lines;
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
        if (x1 != x2 and y1 != y2 ) continue;
        auto xs = std::minmax(x1, x2);
        auto ys = std::minmax(y1, y2);
        for (int i = xs.first; i<= xs.second; i++){
            for (int j = ys.first; j<= ys.second; j++){
                lines[std::make_pair(i, j)]++;
            }
        }
    }
    int total = 0;
    for (auto&& point: lines){
        if (point.second > 1) total++;
    }
    std::cout << total << std::endl;
}
