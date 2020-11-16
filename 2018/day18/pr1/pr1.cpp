#include <vector>
#include <string>
#include <utility>
#include <fstream>
#include <iostream>
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
    for (int round=0; round < 10; round++){
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

        std::swap(copy_grid, state);
    }
    int lumberyards = 0, wooded = 0;
    for (size_t ii=0; ii<state.size(); ii++){
        for (size_t jj=0; jj<state[0].size(); jj++){
            if (state[ii][jj] == '#'){
                lumberyards++;
            }
            else if (state[ii][jj] == '|'){
                wooded++;
            }
        }
    }
    std::cout << "Total resources are " << lumberyards*wooded << std::endl;
}
