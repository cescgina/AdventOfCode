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
    int min_dist = 10000;
    std::vector<int> distances;
    for (auto it=nodes.begin(); it != nodes.end(); it++){
        distances.push_back(std::abs(x_coor-it->first)+std::abs(y_coor-it->second));
    }
    int min_val = std::distance(distances.begin(), std::min_element(distances.begin(), distances.end()));
    min_dist = distances[min_val];
    if (std::count(distances.begin(), distances.end(), min_dist) > 1) return -1;
    return min_val+1;
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
    int max_x = 10, max_y = 10;
    for (std::string line; std::getline(input_file, line);){
        auto pos = line.find(", ");
        int y = std::stoi(line.substr(0, pos));
        int x = std::stoi(line.substr(pos+2));
        max_x = std::max(max_x, x);
        max_y = std::max(max_y, y);
        centers.insert(std::make_pair(x, y));
    }
    std::vector<std::vector<int>> grid(max_x, std::vector<int>(max_y, ' '));
    std::unordered_map<int, int> areas;
    for (int i=0; i<max_x; i++){
        for (int j=0; j<max_y; j++){
            grid[i][j] = find_val(i, j, centers);
            areas[grid[i][j]]++;
        }
    }
    std::set<int> infinites;
    for (int j=0; j<max_y; j++){
        infinites.insert(grid[0][j]);
        infinites.insert(grid[max_x-1][j]);
    }
    for (int i=0; i<max_x; i++){
        infinites.insert(grid[i][0]);
        infinites.insert(grid[i][max_y-1]);
    }
    int max_area = 0;
    for (auto it=areas.begin(); it != areas.end(); it++){
        if (infinites.find(it->first) == infinites.end()){
            max_area = std::max(max_area, it->second);
        }
    }
    std::cout << max_area << std::endl;
}
