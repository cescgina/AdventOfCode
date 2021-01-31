#include <map>
#include <string>
#include <fstream>
#include <iostream>
#include <unistd.h>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

bool DEBUG = false;

typedef std::map<std::pair<int,int>, int> hex;

void find_extreme_values(int& max_q, int& min_q, int& max_r, int& min_r, hex& state){
    for (auto& el: state){
        max_q = std::max(max_q, el.first.first);
        min_q = std::min(min_q, el.first.first);
        max_r = std::max(max_r, el.first.second);
        min_r = std::min(min_r, el.first.second);
    }
}

int count(int q, int r, hex& grid){
    int neighbors = 0;
    // e, w
    neighbors += (grid[std::make_pair(q+1, r)]+grid[std::make_pair(q-1, r)]);
    //nw, se
    neighbors += (grid[std::make_pair(q, r-1)]+grid[std::make_pair(q, r+1)]);
    //ne, sw
    neighbors += (grid[std::make_pair(q+1, r-1)]+grid[std::make_pair(q-1, r+1)]);
    return neighbors;
}

int main(int argc, char** argv){
    std::string file_name, line;
    getopt(argc, argv, "");
    file_name = std::string(argv[optind]);
    hex grid;
    std::ifstream input_file(file_name);
    for (std::string line; std::getline(input_file, line);){
        int q = 0, r = 0;
        size_t i = 0;
        while (i < line.size()){
            std::string dir = line.substr(i, 1);
            if (line[i] == 's' or line[i] == 'n'){
                dir += line[i+1];
                i++;
            }
            if (dir == "e"){
                q++;
            }
            else if (dir == "w"){
                q--;
            }
            else if (dir == "nw"){
                r--;
            }
            else if (dir == "se"){
                r++;
            }
            else if (dir == "ne"){
                q++;
                r--;
            }
            else if (dir == "sw"){
                q--;
                r++;
            }
            else{
                std::cout << "Strange instruction parsed " <<dir << std::endl;
            }
            i++;
        }
        grid[std::make_pair(q,r)] ^= 1;
    }
    input_file.close();
    int days = 100;
    for (int day = 0; day < days; day++){
        int max_q = -1000, min_q = 1000, max_r = -1000, min_r = 1000;
        find_extreme_values(max_q, min_q, max_r, min_r, grid);
        hex new_grid(grid);
        for (int q = min_q-1; q<= max_q+1; q++){
            for (int r = min_r-1; r<= max_r+1; r++){
                int neigh = count(q, r, grid);
                auto pos = std::make_pair(q,r);
                if (grid[pos] == 1){
                    new_grid[pos] = (neigh > 0 and neigh <= 2);
                }
                else{
                    new_grid[pos] = (neigh == 2);
                }
            }
        }
        std::swap(grid, new_grid);
    }
    int total = 0;
    for (auto& el: grid){
        total += el.second;
    }
    std::cout << total << std::endl;
}
