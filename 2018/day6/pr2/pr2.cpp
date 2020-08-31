#include <set>
#include <string>
#include <vector>
#include <fstream>
#include <utility>
#include <iostream>
#include <numeric>
#include <algorithm>
#include <unordered_map>
#include <unistd.h>


int find_val(int x_coor, int y_coor, const std::set<std::pair<int, int>> nodes){
    int dist = 0;
    for (auto it=nodes.begin(); it != nodes.end(); it++){
        dist += std::abs(x_coor-it->first)+std::abs(y_coor-it->second);
    }
    return dist;
}

void print_grid(const std::vector<std::vector<int>>& state){
    for (int i = 0; i < 10; i++){
        for (int j = 0; j < 10; j++){
            std::cout << state[i][j];
        }
        std::cout << std::endl;
    }
}

int main(int argc, char** argv){
    std::string file_name;
    getopt(argc, argv, "");
    file_name = std::string(argv[optind]);
    std::ifstream input_file;
    input_file.open(file_name);
    std::set<std::pair<int, int>> centers;
    for (std::string line; std::getline(input_file, line);){
        auto pos = line.find(", ");
        int y = std::stoi(line.substr(0, pos));
        int x = std::stoi(line.substr(pos+2));
        centers.insert(std::make_pair(x, y));
    }
    std::unordered_map<int, int> areas;
    int region_limit = 10000;
    if (file_name == "data/test.txt") region_limit = 32;
    int max_x = 10*region_limit/centers.size();
    int region = 0, result = 0;
    for (int i=-max_x; i<max_x; i++){
        for (int j=-max_x; j<max_x; j++){
            result = find_val(i, j, centers);
            if (result < region_limit) region++;
        }
    }
    std::cout << region << std::endl;
}
