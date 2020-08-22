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
    for (size_t i = 1; i < rows-1; i++){
        for (size_t j = 1; j < cols-1; j++){
            std::cout << state[i][j].first << "T/" << state[i][j].first+state[i][j].second << "T - ";
        }
        std::cout << std::endl;
    }
}

inline int calc_dist(const size_t& x,const size_t& y, const size_t& gx, const size_t& gy){
    return std::abs((int)x-(int)gx)+std::abs((int)y-(int)gy);
}

std::string hash_state(const cluster& state, const size_t& rows, const size_t& cols){
    std::string hash;
    for (size_t i = 1; i < rows-1; i++){
        for (size_t j = 1; j < cols-1; j++){
            hash += std::to_string(state[i][j].first);
        }
    }
    return hash;
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
    std::pair<int, int> null_node = std::make_pair(10000, 0);
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
            row.push_back(null_node);
            row.push_back(std::make_pair(used, avail));
            grid.push_back(row);
        }
        else {
            grid[y].push_back(std::make_pair(used, avail));
        }
    }
    input_file.close();
    for (auto it = grid.begin(); it != grid.end(); it++){
        it->push_back(null_node);
    }
    row = grid[0];
    std::fill(row.begin(), row.end(), null_node);
    grid.push_back(row);
    grid.insert(grid.begin(), row);
    size_t n_rows = grid.size(), n_cols = grid[0].size();
    auto cmp = [](info left, info right) {return std::get<0>(left) > std::get<0>(right);};
    std::priority_queue<info, std::vector<info>, decltype(cmp)> paths(cmp);
    size_t goal_x = n_cols-2, goal_y = 1, steps;
    paths.push(std::make_tuple(steps, grid, goal_x, goal_y));
    info element;
    visited history;
    while (not paths.empty()){
        element = paths.top();
        paths.pop();
        steps = std::get<0>(element);
        grid = std::get<1>(element);
        goal_x = std::get<2>(element);
        goal_y = std::get<3>(element);
        // if (steps == 2) return 0;
        std::cout << steps << " " << goal_x << "," << goal_y << std::endl;
        for (size_t i = 1; i < n_rows-1; i++){
            for (size_t j = n_cols-2; j > 0; j--){
                if (calc_dist(j, i, goal_x, goal_y) > 60) continue;
                if (grid[i][j].first > 0 and grid[i][j].first < grid[i-1][j].second){
                    // up
                    //std::cout << "Up!" << std::endl;
                    cluster grid_cp = grid;
                    grid_cp[i][j].second += grid_cp[i][j].first;
                    grid_cp[i-1][j].second -= grid_cp[i][j].first;
                    grid_cp[i-1][j].first += grid_cp[i][j].first;
                    grid_cp[i][j].first = 0;
                    std::string hash = hash_state(grid_cp, n_rows, n_cols);
                    if (history.find(hash) == history.end()){
                        history.insert(hash);
                        if (goal_y == i and goal_x == j){
                            if ((i-1) == 1 and j == 1){
                                std::cout << steps+1 << std::endl;
                                return 0;
                            }
                            paths.push(std::make_tuple(steps+1, grid_cp, goal_x, goal_y-1));
                        }
                        else{
                            paths.push(std::make_tuple(steps+1, grid_cp, goal_x, goal_y));
                        }
                    }
                }
                if (grid[i][j].first > 0 and grid[i][j].first < grid[i+1][j].second){
                    // down
                    //std::cout << "Down!" << std::endl;
                    cluster grid_cp = grid;
                    grid_cp[i][j].second += grid_cp[i][j].first;
                    grid_cp[i+1][j].second -= grid_cp[i][j].first;
                    grid_cp[i+1][j].first += grid_cp[i][j].first;
                    grid_cp[i][j].first = 0;
                    std::string hash = hash_state(grid_cp, n_rows, n_cols);
                    if (history.find(hash) == history.end()){
                        history.insert(hash);
                        if (goal_y == i and goal_x == j){
                            paths.push(std::make_tuple(steps+1, grid_cp, goal_x, goal_y+1));
                        }
                        else{
                            paths.push(std::make_tuple(steps+1, grid_cp, goal_x, goal_y));
                        }
                    }
                }
                if (grid[i][j].first > 0 and grid[i][j].first < grid[i][j+1].second){
                    // right
                    //std::cout << "Right!" << std::endl;
                    cluster grid_cp = grid;
                    grid_cp[i][j].second += grid_cp[i][j].first;
                    grid_cp[i][j+1].second -= grid_cp[i][j].first;
                    grid_cp[i][j+1].first += grid_cp[i][j].first;
                    grid_cp[i][j].first = 0;
                    std::string hash = hash_state(grid_cp, n_rows, n_cols);
                    if (history.find(hash) == history.end()){
                        history.insert(hash);
                        if (goal_y == i and goal_x == j){
                            paths.push(std::make_tuple(steps+1, grid_cp, goal_x+1, goal_y));
                        }
                        else{
                            paths.push(std::make_tuple(steps+1, grid_cp, goal_x, goal_y));
                        }
                    }
                }
                if (grid[i][j].first > 0 and grid[i][j].first < grid[i][j-1].second){
                    // left
                    //std::cout << "Left!" << std::endl;
                    cluster grid_cp = grid;
                    grid_cp[i][j].second += grid_cp[i][j].first;
                    grid_cp[i][j-1].second -= grid_cp[i][j].first;
                    grid_cp[i][j-1].first += grid_cp[i][j].first;
                    grid_cp[i][j].first = 0;
                    std::string hash = hash_state(grid_cp, n_rows, n_cols);
                    if (history.find(hash) == history.end()){
                        history.insert(hash);
                        if (goal_y == i and goal_x == j){
                            if (i == 1 and (j-1) == 1){
                                std::cout << steps+1 << std::endl;
                                return 0;
                            }
                            paths.push(std::make_tuple(steps+1, grid_cp, goal_x-1, goal_y));
                        }
                        else{
                            paths.push(std::make_tuple(steps+1, grid_cp, goal_x, goal_y));
                        }
                    }
                }
            }
        }
    }
}
