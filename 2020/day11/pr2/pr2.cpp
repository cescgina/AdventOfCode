#include <stack>
#include <queue>
#include <vector>
#include <bitset>
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
const std::vector<char> chars = {'.',  'L', '#'};

std::ostream& operator<<(std::ostream& out, grid& state){
    size_t rows = state.size();
    size_t cols = state[0].size();
    for (size_t j = 1; j < rows-1; j++){
        for (size_t i = 1; i < cols-1; i++){
           out << chars[state[j][i]]; 
        }
        out << std::endl;
    }
    return out;
}

int count(grid& state, size_t j, size_t i){
    size_t rows = state.size();
    size_t cols = state[0].size();
    size_t distance = 1;
    std::bitset<8> neighbors(0);
    while (j-distance > 0 and state[j-distance][i]==0){
        distance++;
    }
    neighbors[0] = (j-distance > 0 and state[j-distance][i]==2);
    distance = 1;
    while (j+distance < rows-1 and state[j+distance][i]==0){
        distance++;
    }
    neighbors[1] = (j+distance < rows-1 and state[j+distance][i]==2);
    distance = 1;
    while (i+distance < cols-1 and state[j][i+distance]==0){
        distance++;
    }
    neighbors[2] = (i+distance < cols-1 and state[j][i+distance]==2);
    distance = 1;
    while (i-distance > 0 and state[j][i-distance]==0){
        distance++;
    }
    neighbors[3] = (i-distance > 0 and state[j][i-distance]==2);
    distance = 1;
    while (j-distance > 0 and i-distance > 0 and state[j-distance][i-distance]==0){
        distance++;
    }
    neighbors[4] = (j-distance > 0 and i-distance > 0 and state[j-distance][i-distance]==2);
    distance = 1;
    while (j+distance < rows-1 and i+distance < cols-1 and state[j+distance][i+distance]==0){
        distance++;
    }
    neighbors[5] = (j+distance < rows-1 and i+distance < cols-1 and state[j+distance][i+distance]==2);
    distance = 1;
    while (i+distance < cols-1 and j-distance > 0 and state[j-distance][i+distance]==0){
        distance++;
    }
    neighbors[6] = (i+distance < cols-1 and j-distance > 0 and state[j-distance][i+distance]==2);
    distance = 1;
    while (i-distance > 0 and j+distance < rows-1 and state[j+distance][i-distance]==0){
        distance++;
    }
    neighbors[7] = (i-distance > 0 and j+distance < rows-1 and state[j+distance][i-distance]==2);
    return neighbors.count();
}

int main(int argc, char** argv){
    std::string file_name;
    getopt(argc, argv, "");
    file_name = std::string(argv[optind]);
    grid state;
    std::ifstream input_file;
    input_file.open(file_name);
    for (std::string line; std::getline(input_file, line);){
        row current_row{0};
        for (size_t j = 0; j < line.size(); j++){
            switch(line[j]){
                case '.':
                    current_row.push_back(0);
                    break;
                case 'L':
                    current_row.push_back(1);
                    break;
                case '#':
                    current_row.push_back(2);
                    break;
            }
        }
        current_row.push_back(0);
        if (state.size() == 0){
            row halo(current_row.size(),0);
            state.push_back(halo);
        }
        state.push_back(current_row);
    }
    row halo(state[0].size(), 0);
    state.push_back(halo);
    grid new_state(state);
    size_t rows = state.size();
    size_t cols = state[0].size();
    if (DEBUG){
        std::cout << state << std::endl;
        std::cout << std::endl;
    }
    while (true){
        for (size_t j = 1; j < rows-1; j++){
            for (size_t i = 1; i < cols-1; i++){
                int neigh = count(state, j, i);
                if (state[j][i] == 1){
                    new_state[j][i] = 1+(neigh == 0); 
                }
                else if(state[j][i] == 2){
                    new_state[j][i] = (neigh > 4) ? 1 : 2; 
                }
                else{
                    new_state[j][i] = state[j][i];
                }
            }
        }
        if (new_state == state) break;
        state.swap(new_state);
        if (DEBUG){
            std::cout << state << std::endl;
            std::cout << std::endl;
        }
    }
    int occupied = 0;
    for (size_t j = 1; j < rows-1; j++){
        for (size_t i = 1; i < cols-1; i++){
            occupied += (state[j][i]==2);
        }
    }
    std::cout << occupied << std::endl;
}
