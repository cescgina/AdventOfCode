#include <queue>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <unordered_set>
#include <unordered_map>
#include <unistd.h>
#include <boost/functional/hash.hpp>

typedef std::unordered_map<std::pair<int, int>, long long, boost::hash<std::pair<int, int>>> grid;
                  //time, x, y, tool
typedef std::tuple<int, int, int, int> info_path;
typedef std::unordered_set<std::tuple<int, int, int>, boost::hash<std::tuple<int, int, int>>> visited_points;

void print_risk(grid& cave, int target_y, int target_x){
    for (int i=0; i<target_y; i++){
        for (int j=0; j<target_x; j++){
            auto pos = std::make_pair(i, j);
            if (cave[pos] == 0){
                std::cout << '.';
            }
            else if (cave[pos] == 1){
                std::cout << '=';
            }
            else if (cave[pos] == 2){
                std::cout << '|';
            }
            else{
                std::cout << '~';
            }
        }
        std::cout << std::endl;
    }
}

void calculate(int y, int x, grid& erosion, grid& geological_index, grid& type_rock, int depth){
    auto position = std::make_pair(y, x);
    if (y == 0){
        geological_index[position] = x*16807;
        erosion[position] = (geological_index[position]+depth) % 20183;
        type_rock[position] = erosion[position] % 3;
    }
    else if (x == 0){
        geological_index[position] = y*48271;
        erosion[position] = (geological_index[position]+depth) % 20183;
        type_rock[position] = erosion[position] % 3;
    }
    else{
        auto neighbor1 = std::make_pair(y-1, x);
        auto neighbor2 = std::make_pair(y, x-1);
        if (erosion.find(neighbor1) == erosion.end()){
            calculate(y-1, x, erosion, geological_index, type_rock, depth);
        }
        if (erosion.find(neighbor2) == erosion.end()){
            calculate(y, x-1, erosion, geological_index, type_rock, depth);
        }
        geological_index[position] = erosion[neighbor1]*erosion[neighbor2];
        erosion[position] = (geological_index[position]+depth) % 20183;
        type_rock[position] = erosion[position] % 3;
    }
}

int main(int argc, char** argv){
    std::string file_name, line;
    getopt(argc, argv, "");
    file_name = std::string(argv[optind]);
    std::ifstream input_file;
    input_file.open(file_name);
    std::getline(input_file, line);
    auto pos = line.find(":");
    int depth = std::stoi(line.substr(pos+2));
    std::getline(input_file, line);
    pos = line.find(":");
    std::string coords = line.substr(pos+2);
    pos = coords.find(",");
    int target_x = std::stoi(coords.substr(0, pos))+1;
    int target_y = std::stoi(coords.substr(pos+1))+1;
    input_file.close();
    grid erosion;
    grid geological;
    grid types;
    std::pair<int,int> position = std::make_pair(0, 0);
    erosion[position] = 0;
    geological[position] = 0;
    types[position] = 0;
    for (int i=0; i<target_y; i++){
        for (int j=0; j<target_x; j++){
            position = std::make_pair(i, j);
            if ((i == (target_y-1)) and (j == (target_x-1))){
                geological[position] = 0;
                erosion[position] = depth % 20183;
                types[position] = erosion[position] % 3;
            }
            else{
                calculate(i, j, erosion, geological, types, depth);
            }
        }
    }
    std::priority_queue<info_path, std::vector<info_path>, std::greater<info_path>> paths;
    // start at 0, 0 with the torch
    paths.push(std::make_tuple(0, 0, 0, 2));
    visited_points visited;
    while (not paths.empty()){
        auto current_step = paths.top();
        paths.pop();
        int time = std::get<0>(current_step);
        int y = std::get<1>(current_step);
        int x = std::get<2>(current_step);
        int tool = std::get<3>(current_step);
        position = std::make_pair(y, x);
        auto point = std::make_tuple(y, x, tool);
        if ((y == (target_y-1)) and (x == (target_x-1)) and tool == 2){
            // need to use the torch to reach the target
            std::cout << "Found target in " << time << " minutes" << std::endl;
            return 0;
        }
        if (visited.find(point) != visited.end()){
            continue;
        }
        visited.insert(point);
        std::vector<std::pair<int, int>> neighbors {std::make_pair(y-1, x), std::make_pair(y, x-1), std::make_pair(y, x+1), std::make_pair(y+1, x)};
        for (const auto& coords: neighbors){
            if (coords.first < 0 or coords.second < 0) continue;
            if (geological.find(coords) == geological.end()){
                calculate(coords.first, coords.second, erosion, geological, types, depth);
            }
            // check if can access with current tool
            // type 0 is rocky, type 1 is wet, type 2 is narrow
            // tool 0 is neither, tool 1 is climbing gear, tool 2 is the torch
            if (types[coords] == 0 and tool == 0) continue;
            if (types[coords] == 1 and tool == 2) continue;
            if (types[coords] == 2 and tool == 1) continue;
            
            if ((coords.first == (target_y-1)) and (coords.second == (target_x-1)) and tool == 2){
                // need to use the torch to reach the target
                std::cout << "Found target in " << time+1 << " minutes" << std::endl;
                return 0;
            }
            paths.push(std::make_tuple(time+1, coords.first, coords.second, tool));
        }
        if (types[position] == 0) {
            if (tool == 0){
                std::cout << "Inappropiate tool, nothing on rocky region" << std::endl;
            }
            if (tool == 1){
                paths.push(std::make_tuple(time+7, position.first, position.second, 2));
            }
            else{
                paths.push(std::make_tuple(time+7, position.first, position.second, 1));
            }
        }
        if (types[position] == 1) {
            if (tool == 2){
                std::cout << "Inappropiate tool, torch on wet region" << std::endl;
            }
            if (tool == 0){
                paths.push(std::make_tuple(time+7, position.first, position.second, 1));
            }
            else{
                paths.push(std::make_tuple(time+7, position.first, position.second, 0));
            }
        }
        if (types[position] == 2) {
            if (tool == 1){
                std::cout << "Inappropiate tool, climbing gear on narrow region" << std::endl;
            }
            if (tool == 0){
                paths.push(std::make_tuple(time+7, position.first, position.second, 2));
            }
            else{
                paths.push(std::make_tuple(time+7, position.first, position.second, 0));
            }
        }
    }
}
