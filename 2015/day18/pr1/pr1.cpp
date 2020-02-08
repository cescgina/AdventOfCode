#include <vector>
#include <string>
#include <fstream>
#include <numeric>
#include <iostream>
#include <unistd.h>

typedef std::vector<std::vector<int>> grid;

void parse_command(std::string& order, grid& light_grid){
    int w = order.length();
    std::vector<int> ligths_row(w+2, 0);
    if (light_grid.size() == 0){
        std::vector<int> ligths_row_extra(w+2, 0);
        light_grid.push_back(ligths_row_extra);
    }
    for (int i=0; i<w; i++){
        ligths_row[i+1] = order[i] == '#' ;
    }
    light_grid.push_back(ligths_row);
    if (light_grid.size() == (size_t)(w+1)){
        std::vector<int> ligths_row_extra(w+2, 0);
        light_grid.push_back(ligths_row_extra);
    }
}

int count_neighbors(grid& ligths_grid, size_t& row, size_t& col){
    int neigh = 0;
    neigh = ligths_grid[row+1][col]+ligths_grid[row-1][col];
    neigh += ligths_grid[row][col+1]+ligths_grid[row-1][col+1]+ligths_grid[row+1][col+1];
    neigh += ligths_grid[row][col-1]+ligths_grid[row-1][col-1]+ligths_grid[row+1][col-1];
    return neigh;
}

void update_lights(grid& light_grid, grid& light_copy){
    size_t w=light_grid[0].size(), h=light_grid.size();
    int neigh;
    for (size_t i=1 ;i<h-1; i++){
        for (size_t j=1; j<w-1; j++){
            neigh = count_neighbors(light_grid, i, j);
            if(light_grid[i][j] == 1){
                light_copy[i][j] = (neigh > 1 and neigh < 4);
            }
            else{
                light_copy[i][j] = (neigh == 3);
            }
        }
    }
    std::swap(light_grid, light_copy);
}

int main(int argc, char** argv){
    std::string file_name, command;
    getopt(argc, argv, "");
    file_name = std::string(argv[optind]);
    grid ligths; 
    int ligths_on = 0, steps;
    std::ifstream input_file;
    input_file.open(file_name);
    std::getline(input_file, command);
    steps = std::stoi(command);
    while (std::getline(input_file, command)){
        parse_command(command, ligths);
    }
    input_file.close();
    grid copy_lights(ligths);
    for (int j=0; j<steps; j++){
        update_lights(ligths, copy_lights);
    }
    for (size_t i=1; i < ligths.size()-1; i++){
        ligths_on += std::accumulate(ligths[i].begin(), ligths[i].end(), 0);
    }
    std::cout << "Answer is " << ligths_on << std::endl;
}
