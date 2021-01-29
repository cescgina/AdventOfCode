#include <map>
#include <vector>
#include <string>
#include <fstream>
#include <numeric>
#include <iostream>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include <unistd.h>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

bool DEBUG = false;
typedef std::tuple<int, int, int, int> indexes;
typedef std::map<indexes, int> grid;
const std::vector<char> chars = {'.',  '#'};

std::ostream& operator<<(std::ostream& out, const indexes& pos){
    int w = std::get<0>(pos);
    int z = std::get<1>(pos);
    int y = std::get<2>(pos);
    int x = std::get<3>(pos);
    out << x<<","<<y<<","<<z <<","<<w;
    return out;
}

std::ostream& operator<<(std::ostream& out, grid& state){
    int prev_w = -200;
    int prev_z = -200;
    int prev_y = -200;
    for (auto& el: state){
        int w = std::get<0>(el.first);
        int z = std::get<1>(el.first);
        int y = std::get<2>(el.first);
        if (w != prev_w){
            out << std::endl << std::endl;
            out << "w=" << w;
            prev_w = w;
        }
        if (z != prev_z){
            out << std::endl << std::endl;
            out << "z=" << z;
            prev_z = z;
        }
        if (y != prev_y){
            out << std::endl;
            prev_y = y;
        }
        out << chars[el.second];
    }
    return out;
}


int count(grid& state, indexes pos){
    int w = std::get<0>(pos);
    int z = std::get<1>(pos);
    int y = std::get<2>(pos);
    int x = std::get<3>(pos);
    int neigh = 0;
    for (int w_val = w-1; w_val <= w+1; w_val++){
        for (int z_val = z-1; z_val <= z+1; z_val++){
            for (int y_val = y-1; y_val <= y+1; y_val++){
                for (int x_val = x-1; x_val <= x+1; x_val++){
                    neigh += state[std::make_tuple(w_val, z_val, y_val, x_val)];
                }
            }
        }
    }
    // avoid counting the original cube
    neigh -= state[pos];
    return neigh;
}

void get_extreme_values(grid& state, int& x_min, int& x_max, int& y_min, int& y_max, int& z_min, int& z_max, int& w_min, int& w_max){
    for (auto& el: state){
        int w = std::get<0>(el.first);
        int z = std::get<1>(el.first);
        int y = std::get<2>(el.first);
        int x = std::get<3>(el.first);
        x_min = std::min(x_min, x);
        y_min = std::min(y_min, y);
        z_min = std::min(z_min, z);
        w_min = std::min(w_min, w);
        x_max = std::max(x_max, x);
        y_max = std::max(y_max, y);
        z_max = std::max(z_max, z);
        w_max = std::max(w_max, w);
    }
}

int main(int argc, char** argv){
    std::string file_name;
    getopt(argc, argv, "");
    file_name = std::string(argv[optind]);
    grid state;
    std::ifstream input_file;
    input_file.open(file_name);
    size_t i = 0;
    for (std::string line; std::getline(input_file, line);){
        for (size_t j = 0; j < line.size(); j++){
            switch(line[j]){
                case '.':
                    state[std::make_tuple(0, 0, i, j)] = 0;
                    break;
                case '#':
                    state[std::make_tuple(0, 0, i, j)] = 1;
                    break;
            }
            state[std::make_tuple(0, 1, i, j)] = 0;
            state[std::make_tuple(0, -1, i, j)] = 0;
            state[std::make_tuple(1, 1, i, j)] = 0;
            state[std::make_tuple(1, -1, i, j)] = 0;
            state[std::make_tuple(-1, 1, i, j)] = 0;
            state[std::make_tuple(-1, -1, i, j)] = 0;
        }
        i++;
    }
    grid new_state(state);
    std::unordered_set<std::string> cache;
    if (DEBUG){
        std::cout << state << std::endl;
        std::cout << std::endl;
        std::cout << "Starting simulation" << std::endl;
    }
    int turns = 6;
    for (int turn = 0; turn< turns; turn++){
        int x_min = 100000000, y_min= 100000000, z_min= 100000000, w_min = 100000000;
        int x_max = -100000000, y_max= -100000000, z_max= -100000000, w_max = -100000000;
        get_extreme_values(state, x_min, x_max, y_min, y_max, z_min, z_max, w_min, w_max);
        for (int w_val = w_min-1; w_val <= w_max+1; w_val++){
            for (int z_val = z_min-1; z_val <= z_max+1; z_val++){
                for (int y_val = y_min-1; y_val <= y_max+1; y_val++){
                    for (int x_val = x_min-1; x_val <= x_max+1; x_val++){
                        indexes pos = std::make_tuple(w_val, z_val, y_val, x_val);
                        int neigh = count(state, pos);
                        if (state[pos] == 1){
                            new_state[pos] = (neigh == 2 or neigh == 3); 
                        }
                        else{
                            new_state[pos] = (neigh == 3);
                        }
                    }
                }
            }
        }
        state.swap(new_state);
        if (DEBUG){
            std::cout << "Round " << turn+1 << std::endl;
            std::cout << state << std::endl;
            std::cout << std::endl;
        }
    }
    int occupied = std::accumulate(state.begin(), state.end(), 0, [](int a, std::pair<indexes, int> b){return a+b.second;});
    std::cout << occupied << std::endl;
}
