#include <vector>
#include <string>
#include <utility>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <unistd.h>

typedef std::vector<char> row;
typedef std::vector<row> grid;

bool in_bounds(int y, int x, int max_y, int max_x){
    return (x >= 0 and x < max_x and y >= 0 and y < max_y);
}

std::vector<int> count_neighbors(int y, int x, grid& current_state){
    std::vector<int> counts = {0, 0, 0};
    std::vector<std::pair<int, int>> neighbors = {std::make_pair(y-1, x-1), std::make_pair(y-1, x), std::make_pair(y-1, x+1), std::make_pair(y, x-1), std::make_pair(y, x+1), std::make_pair(y+1, x-1), std::make_pair(y+1, x), std::make_pair(y+1, x+1)};
    for (auto coords: neighbors){
        int xx = coords.second;
        int yy = coords.first;
        if (not in_bounds(yy, xx, current_state.size(), current_state[0].size())) continue;
        if (current_state[yy][xx] == '.'){
            // open
            counts[0]++;
        }
        else if (current_state[yy][xx] == '#'){
            // lumber
            counts[1]++;
        }
        else{
            //trees
            counts[2]++;
        }
    }

    return counts;
}
    
void print_state(grid& field){
    for (size_t ii=0; ii<field.size(); ii++){
        for (size_t jj=0; jj<field[0].size(); jj++){
            std::cout << field[ii][jj];
        }
        std::cout << std::endl;
    }
}

int count_resources(grid & field){
    int lumberyards = 0, wooded = 0;
    for (size_t ii=0; ii<field.size(); ii++){
        for (size_t jj=0; jj<field[0].size(); jj++){
            if (field[ii][jj] == '#'){
                lumberyards++;
            }
            else if (field[ii][jj] == '|'){
                wooded++;
            }
        }
    }
    return lumberyards*wooded;
}

grid make_grid(std::string & hash){
    grid new_state;
    row current;
    for (auto letter: hash){
        if (letter == '\n'){
            new_state.push_back(current);
            current.clear();
        }
        else{
            current.push_back(letter);
        }
    }
    return new_state;
}

std::string make_string(grid& field){
    std::string hash;
    for (size_t ii=0; ii<field.size(); ii++){
        for (size_t jj=0; jj<field[0].size(); jj++){
            hash.push_back(field[ii][jj]);
        }
        hash.push_back('\n');
    }
    return hash;
}

int main(int argc, char** argv){
    std::string file_name;
    getopt(argc, argv, "");
    grid state;
    file_name = std::string(argv[optind]);
    std::ifstream input_file;
    input_file.open(file_name);
    for(std::string line;std::getline(input_file, line);){
        row current;
        for (size_t i=0; i<line.size(); i++){
            current.push_back(line[i]);
        }
        state.push_back(current);
    }
    input_file.close();
    grid copy_grid(state);
    std::unordered_map<std::string, std::tuple<grid, std::string,int, int>> history;
    std::string state_string = make_string(state);
    int resources = 0, limit = 1000000000, round = 0, period = 0;
    while (round < limit){
        if (history.find(state_string) != history.end()){
            auto info = history[state_string];
            state = std::get<0>(info);
            state_string = std::get<1>(info);
            resources = std::get<2>(info);
            int prev_round = std::get<3>(info);
            period = round-prev_round;
            break;
            continue;
        }
        copy_grid = state;
        for (size_t ii=0; ii<state.size(); ii++){
            for (size_t jj=0; jj<state[0].size(); jj++){
                auto neighbor_counts = count_neighbors(ii, jj, state);
                int lumber = neighbor_counts[1], trees = neighbor_counts[2];
                if (state[ii][jj] == '.'){
                    // open
                    if (trees > 2){
                        copy_grid[ii][jj] = '|';
                    }
                    else{
                        copy_grid[ii][jj] = '.';
                    }
                }
                else if (state[ii][jj] == '#'){
                    // lumber
                    if (lumber > 0 and trees > 0){
                        copy_grid[ii][jj] = '#';
                    }
                    else{
                        copy_grid[ii][jj] = '.';
                    }
                }
                else{
                    //trees
                    if (lumber > 2) {
                        copy_grid[ii][jj] = '#';
                    }
                    else{
                        copy_grid[ii][jj] = '|';
                    }
                }
            }
        }
        resources = count_resources(copy_grid);
        state_string = make_string(copy_grid);
        history[make_string(state)] = std::make_tuple(copy_grid, state_string, resources, round);
        std::swap(copy_grid, state);
        round++;
    }
    int turns = (limit-round)/period;
    round += (turns*period)+1;
    while (round < limit){
        auto info = history[state_string];
        state = std::get<0>(info);
        state_string = std::get<1>(info);
        resources = std::get<2>(info);
        round++;
    }
    std::cout << "Total resources are " << count_resources(state) << std::endl;
}
