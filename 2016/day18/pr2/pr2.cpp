#include <vector>
#include <fstream>
#include <utility>
#include <iostream>
#include <numeric>
#include <unistd.h>

void print_tiles(const std::vector<bool>& tiles){
    for (size_t j = 1; j<tiles.size()-1; j++){
        if (tiles[j]) std::cout << "^";
        else std::cout << ".";
    }
    std::cout << std::endl;
}

int main(int argc, char** argv){
    std::string file_name, line;
    getopt(argc, argv, "");
    file_name = std::string(argv[optind]);
    std::ifstream input_file;
    input_file.open(file_name);
    std::getline(input_file, line);
    int rows = std::stoi(line); 
    std::getline(input_file, line);
    std::vector<bool> row = {false};
    for (char& x: line){
        row.push_back(x == '^');
    }
    row.push_back(false);
    input_file.close();
    size_t n_tiles = row.size();
    std::vector<bool> next_row = std::vector<bool>(n_tiles, false);
    int sum = n_tiles-std::accumulate(row.begin(), row.end(), 0)-2;
    for (int i=1;i<rows; i++){
        for (size_t j = 1; j<n_tiles-1; j++){
            next_row[j] = row[j-1] ^ row[j+1];
        }
        sum += n_tiles-std::accumulate(next_row.begin(), next_row.end(), 0)-2;
        std::swap(next_row, row);
    }
    std::cout << "There are " << sum << " safe tiles" << std::endl;
}
