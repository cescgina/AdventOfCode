#include <map>
#include <set>
#include <tuple>
#include <queue>
#include <vector>
#include <string>
#include <fstream>
#include <utility>
#include <iostream>
#include <unistd.h>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

typedef std::vector<std::vector<std::pair<int, int>>> cluster;
typedef std::set<std::string> visited;
typedef std::tuple<int, cluster, int, int> info;

void print_state(const cluster& state, const size_t& rows, const size_t& cols){
    for (size_t i = 0; i < rows; i++){
        for (size_t j = 0; j < cols; j++){
            if (state[i][j].first == 0) std::cout<< "_";
            else if (state[i][j].first > 300) std::cout << "#"; 
            else std::cout << ".";
        }
        std::cout << std::endl;
    }
}

void find_empty(const cluster& state, const size_t& rows, const size_t& cols, size_t& x, size_t& y){
    for (size_t i = 0; i < rows; i++){
        for (size_t j = 0; j < cols; j++){
            if (state[i][j].first == 0){
                x = j;
                y = i;
                return;
            }
        }
    }
}

int main(int argc, char** argv){
    std::string file_name, line, node;
    getopt(argc, argv, "");
    file_name = std::string(argv[optind]);
    std::ifstream input_file;
    cluster grid;
    int x = 0, y = 0, used = 0, avail = 0;
    std::vector<std::string> parsed_line;
    std::vector<std::pair<int, int>> row;
    input_file.open(file_name);
    std::getline(input_file, line);
    std::getline(input_file, line);
    for (std::string line; std::getline(input_file, line);){
        boost::split(parsed_line, line, boost::is_any_of(" "), boost::token_compress_on);
        used = std::stoi(parsed_line[2].substr(0, parsed_line[2].size()-1));
        avail = std::stoi(parsed_line[3].substr(0, parsed_line[3].size()-1));
        node = parsed_line[0].substr(16);
        auto pos = node.find("-");
        x = std::stoi(node.substr(0, pos));
        y = std::stoi(node.substr(pos+2));
        if (x == 0){
            row.clear();
            row.push_back(std::make_pair(used, avail));
            grid.push_back(row);
        }
        else {
            grid[y].push_back(std::make_pair(used, avail));
        }
    }
    input_file.close();
    size_t n_rows = grid.size(), n_cols = grid[0].size(), x0, y0;
    print_state(grid, n_rows, n_cols);
    find_empty(grid, n_rows, n_cols, x0, y0);
    std::cout << 1+(x0-1)+y0+(n_cols-3)+(n_cols-2)*5 << std::endl;
}
