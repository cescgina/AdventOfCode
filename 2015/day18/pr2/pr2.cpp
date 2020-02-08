#include <vector>
#include <string>
#include <fstream>
#include <numeric>
#include <iostream>
#include <unistd.h>

typedef std::vector<std::vector<int>> grid;

void parse_command(std::string& order, grid& light_grid){
    int w = order.length();
    std::vector<int> ligths_row(w, 0);
    for (int i=0; i<w; i++){
        ligths_row[i] = order[i] == '#' ;
    }
    light_grid.push_back(ligths_row);
}

int count_neighbors(grid& ligths_grid, size_t& w, size_t& h, size_t& row, size_t& col){
    int neigh = 0;
    if (row == 0){
        neigh += ligths_grid[row+1][col];
        if (col != w-1){
            neigh += ligths_grid[row][col+1]+ligths_grid[row+1][col+1];
        }
        if (col != 0){
            neigh += ligths_grid[row][col-1]+ligths_grid[row+1][col-1];
        }
    }
    else if (row == h-1){
        neigh += ligths_grid[row-1][col];
        if (col != w-1){
            neigh += ligths_grid[row][col+1]+ligths_grid[row-1][col+1];
        }
        if (col != 0){
            neigh += ligths_grid[row][col-1]+ligths_grid[row-1][col-1];
        }
    }
    else{
        neigh = (ligths_grid[row+1][col]+ligths_grid[row-1][col]);
        if (col != w-1){
            neigh += ligths_grid[row][col+1]+ligths_grid[row-1][col+1]+ligths_grid[row+1][col+1];
        }
        if (col != 0){
            neigh += ligths_grid[row][col-1]+ligths_grid[row-1][col-1]+ligths_grid[row+1][col-1];
        }
    }
    return neigh;
}

void update_lights(grid& light_grid, grid& light_copy){
    size_t w=light_grid[0].size(), h=light_grid.size();
    int neigh;
    for (size_t i=0 ;i<h; i++){
        for (size_t j=0; j<w; j++){
            neigh = count_neighbors(light_grid, w, h, i, j);
            if(light_grid[i][j] == 1){
                light_copy[i][j] = (neigh > 1 and neigh < 4);
            }
            else{
                light_copy[i][j] = (neigh == 3);
            }
        }
    }
    // four corners are bugged and always on
    light_copy[0][w-1] = 1;
    light_copy[h-1][0] = 1;
    light_copy[0][0] = 1;
    light_copy[h-1][w-1] = 1;
    std::swap(light_grid, light_copy);
}

int main(int argc, char** argv){
    std::string file_name, command;
    getopt(argc, argv, "");
    file_name = std::string(argv[optind]);
    grid ligths; 
    int ligths_on = 0, steps, size;
    std::ifstream input_file;
    input_file.open(file_name);
    std::getline(input_file, command);
    steps = std::stoi(command);
    while (std::getline(input_file, command)){
        parse_command(command, ligths);
    }
    input_file.close();
    size = ligths.size();
    // four corners are bugged and always on
    ligths[0][size-1] = 1;
    ligths[size-1][0] = 1;
    ligths[0][0] = 1;
    ligths[size-1][size-1] = 1;
    grid copy_lights(ligths);
    for (int j=0; j<steps; j++){
        update_lights(ligths, copy_lights);
    }
    for (size_t i=0; i < ligths.size(); i++){
        ligths_on += std::accumulate(ligths[i].begin(), ligths[i].end(), 0);
    }
    std::cout << "Answer is " << ligths_on << std::endl;
}
