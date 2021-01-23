#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <unistd.h>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

typedef std::vector<int> row;
typedef std::vector<row> grid;

int main(int argc, char** argv){
    std::string file_name;
    getopt(argc, argv, "");
    file_name = std::string(argv[optind]);
    grid trees;
    std::ifstream input_file(file_name);
    for (std::string line; std::getline(input_file, line);){
        row current;
        for (size_t i = 0; i<line.size(); i++){
            current.push_back(line[i] == '#');
        }
        trees.push_back(current);
    }
    input_file.close();
    std::vector<std::pair<int, int>> all_increments = {std::make_pair(1, 1), std::make_pair(3, 1),std::make_pair(5, 1),std::make_pair(7, 1),std::make_pair(1, 2)};
    size_t rows = trees.size(), cols = trees[0].size();
    long long all_trees = 1;
    for (auto increments: all_increments){
        int inc_x = increments.first;
        int inc_y = increments.second;
        size_t col = 0;
        size_t tot_trees = 0;
        for (size_t j = 0; j<rows-inc_y; j += inc_y){
            col = (col+inc_x) % cols;
            tot_trees += trees[j+inc_y][col];
        }
        all_trees *= tot_trees;
    }
    std::cout << "Trees found " << all_trees << std::endl;
}
