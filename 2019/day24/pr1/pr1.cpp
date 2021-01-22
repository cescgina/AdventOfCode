#include <stack>
#include <queue>
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
typedef std::vector<int> row;
typedef std::vector<row> grid;
const std::vector<char> chars = {'.', '#'};

std::ostream& operator<<(std::ostream& out, grid& state){
    for (size_t j = 1; j < 6; j++){
        for (size_t i = 1; i < 6; i++){
           out << chars[state[j][i]]; 
        }
        out << std::endl;
    }
    return out;
}

std::string hash(grid & state){
    std::string hash;
    for (size_t j = 1; j < 6; j++){
        for (size_t i = 1; i < 6; i++){
           hash.push_back(char(48+state[j][i])); 
        }
    }
    return hash;
}

int count(grid& state, size_t j, size_t i){
    return state[j-1][i]+state[j+1][i]+state[j][i+1]+state[j][i-1];
}

int main(int argc, char** argv){
    std::string file_name;
    getopt(argc, argv, "");
    file_name = std::string(argv[optind]);
    grid state;
    std::ifstream input_file;
    input_file.open(file_name);
    row halo(6,0);
    state.push_back(halo);
    for (std::string line; std::getline(input_file, line);){
        row current_row{0};
        for (size_t j = 0; j < line.size(); j++){
            current_row.push_back(line[j] == '#');
        }
        current_row.push_back(0);
        state.push_back(current_row);
    }
    state.push_back(halo);
    grid new_state(state);
    std::string hash_state = hash(state);
    std::unordered_set<std::string> cache;
    while (cache.find(hash_state) == cache.end()){
        cache.insert(hash_state);
        for (size_t j = 1; j < 6; j++){
            for (size_t i = 1; i < 6; i++){
                int neigh = count(state, j, i);
                if (state[j][i]){
                    new_state[j][i] = (neigh == 1); 
                }
                else{
                    new_state[j][i] = (neigh > 0 and neigh < 3); 
                }
            }
        }
        state.swap(new_state);
        hash_state = hash(state);
    }
    std::cout << "Final state:" << std::endl;
    std::cout << state << std::endl;
    long long biodiversity = 0;
    long long power = 1;
    for (size_t j = 1; j < 6; j++){
        for (size_t i = 1; i < 6; i++){
            biodiversity += power*state[j][i];
            power *= 2;
        }
    }
    std::cout << biodiversity << std::endl;
}
