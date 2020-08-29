#include <vector>
#include <fstream>
#include <utility>
#include <iostream>
#include <algorithm>
#include <unordered_map>
#include <unistd.h>

bool overlaps(const std::vector<std::vector<int>>& state, const std::tuple<std::string, int, int, int, int>& info){
    std::string id = std::get<0>(info);
    int corner_x = std::get<1>(info);
    int corner_y = std::get<2>(info);
    int height = std::get<3>(info);
    int width = std::get<4>(info);
    for (int i=corner_x; i<(corner_x+height); i++){
        for (int j=corner_y; j<(corner_y+width); j++){
            if (state[i][j] > 1) return false;
        }
    }
    std::cout << id << std::endl;
    return true;
}

int main(int argc, char** argv){
    std::string file_name;
    getopt(argc, argv, "");
    file_name = std::string(argv[optind]);
    std::vector<std::vector<int>> grid(1200, std::vector<int>(1050, 0));
    std::vector<std::tuple<std::string, int, int, int, int>> saved;
    std::ifstream input_file;
    input_file.open(file_name);
    for (std::string line; std::getline(input_file, line);){
        int corner_x, corner_y, width, height;
        auto pos_at = line.find("@ ");
        std::string id = line.substr(1, pos_at-2);
        auto pos_colon = line.find(": ", pos_at+1);
        std::string corner_info = line.substr(pos_at+2, pos_colon-pos_at-2);
        auto pos_comma = corner_info.find(",");
        corner_y = std::stoi(corner_info.substr(0, pos_comma));
        corner_x = std::stoi(corner_info.substr(pos_comma+1));
        std::string size_info = line.substr(pos_colon+2);
        auto pos_x = size_info.find("x");
        width = std::stoi(size_info.substr(0, pos_x));
        height = std::stoi(size_info.substr(pos_x+1));
        bool overlap = false;
        for (int i=corner_x; i<(corner_x+height); i++){
            for (int j=corner_y; j<(corner_y+width); j++){
                overlap |= grid[i][j] > 1;
                grid[i][j]++;
            }
        }
        if (!overlap){
            saved.push_back(std::make_tuple(id, corner_x, corner_y, height, width));
        }
    }
    for (auto possible: saved){
        if (overlaps(grid, possible)){
            return 0;
        }
    }
}
