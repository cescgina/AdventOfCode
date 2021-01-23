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
    size_t rows = trees.size(), cols = trees[0].size();
    size_t col = 0;
    size_t tot_trees = 0;
    for (size_t j = 0; j<rows-1; j++){
        col = (col+3) % cols;
        tot_trees += trees[j+1][col];
    }
    std::cout << "Trees found " << tot_trees << std::endl;
}
