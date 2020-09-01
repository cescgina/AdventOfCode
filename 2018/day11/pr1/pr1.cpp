#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <numeric>
#include <unistd.h>

int calc_fuel(int y, int x, int serial){
    int level = serial+(x+10)*y;
    level *= (x+10);
    int remainder = 0;
    for (int k = 0 ; k < 3; k++){
        remainder = level % 10;
        level /= 10;
    }
    return remainder-5;
}

int main(int argc, char** argv){
    std::string file_name;
    getopt(argc, argv, "");
    file_name = std::string(argv[optind]);
    std::ifstream input_file;
    input_file.open(file_name);
    std::string line; 
    std::getline(input_file, line);
    std::vector<std::vector<int>> grid(300, std::vector<int>(300, 0));
    int serial = std::stoi(line), max_power = 0, max_x = 0, max_y = 0;
    for (int i=0; i<300; i++){
        for (int j=0; j<300; j++){
            grid[i][j] = calc_fuel(i+1, j+1, serial);
        }
    }
    for (int i=0; i<300-3; i++){
        for (int j=0; j<300-3; j++){
            int level = 0;
            for (int k=0; k<3; k++){
                level += std::accumulate(grid[i+k].begin()+j, grid[i+k].begin()+j+3,0);
            }
            if (level > max_power){
                max_power = level;
                max_x = j+1;
                max_y = i+1;
            }
        }
    }
    std::cout << max_x << ","<< max_y << std::endl;
}
