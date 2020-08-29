#include <vector>
#include <fstream>
#include <utility>
#include <iostream>
#include <algorithm>
#include <unordered_map>
#include <unistd.h>

int main(int argc, char** argv){
    std::string file_name;
    getopt(argc, argv, "");
    file_name = std::string(argv[optind]);
    std::vector<std::vector<int>> grid(1200, std::vector<int>(1050, 0));
    std::ifstream input_file;
    input_file.open(file_name);
    for (std::string line; std::getline(input_file, line);){
        int corner_x, corner_y, width, height;
        auto pos_at = line.find("@ ");
        auto pos_colon = line.find(": ", pos_at+1);
        std::string corner_info = line.substr(pos_at+2, pos_colon-pos_at-2);
        auto pos_comma = corner_info.find(",");
        corner_y = std::stoi(corner_info.substr(0, pos_comma));
        corner_x = std::stoi(corner_info.substr(pos_comma+1));
        std::string size_info = line.substr(pos_colon+2);
        auto pos_x = size_info.find("x");
        width = std::stoi(size_info.substr(0, pos_x));
        height = std::stoi(size_info.substr(pos_x+1));
        for (int i=corner_x; i<(corner_x+height); i++){
            for (int j=corner_y; j<(corner_y+width); j++){
                grid[i][j]++;
            }
        }
    }
    int overlapping = 0;
    for (auto vec: grid){
        overlapping += std::count_if(vec.begin(), vec.end(), [](int x){return x>1;});
    }
    std::cout << overlapping << std::endl;
}
