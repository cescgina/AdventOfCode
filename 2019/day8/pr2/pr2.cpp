#include <string>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <unistd.h>

int main(int argc, char** argv){
    std::string file_name;
    getopt(argc, argv, "");
    file_name = std::string(argv[optind]);
    std::ifstream input_file;
    input_file.open(file_name);
    std::string line;
    std::getline(input_file, line);
    int rows = 6, cols = 25;
    int pixels = rows*cols;
    std::vector<std::vector<int>> image(rows, std::vector<int>(cols, 2));
    for (size_t i = 0; i < line.size(); i++){
        int layer = i % pixels;
        int x = layer / cols;
        int y = layer % cols;
        int pixel = line[i]-'0';
        if (image[x][y] == 2){
            image[x][y] = pixel;
        }
    }
    for (auto& row: image){
        for (auto& pixel: row){
            if (pixel == 1) std::cout<<"|";
            else std::cout << " ";
        }
        std::cout << std::endl;
    }
}
