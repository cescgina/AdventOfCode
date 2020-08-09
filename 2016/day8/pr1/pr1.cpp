#include <map>
#include <vector>
#include <string>
#include <fstream>
#include <numeric>
#include <iostream>
#include <algorithm>
#include <unistd.h>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

typedef std::vector<std::vector<int>> screen;

void print_grid(screen& grid, int rows, int cols){
    std::vector<std::string> elements={".", "#"};
    for (int i=0; i < rows; i++){
        for (int j=0; j < cols; j++){
            std::cout << elements[grid[i][j]];
        }
        std::cout << std::endl;
    }
}

int main(int argc, char** argv){
    std::string file_name, line;
    getopt(argc, argv, "");
    file_name = std::string(argv[optind]);
    std::ifstream input_file;
    input_file.open(file_name);
    int sum_pixels = 0, h = 6, w = 50;
    if (file_name == "data/test.txt"){
        h = 3;
        w = 7;
    }
    screen main_sc;
    for (int i=0; i < h; i++){
        main_sc.push_back(std::vector<int>(w, 0));
    }
    screen copy_sc = main_sc;
    std::vector<std::string>parsed_line;
    for (std::string line; std::getline(input_file, line);){
        std::cout << line << std::endl;
        boost::split(parsed_line, line, boost::is_any_of(" "), boost::token_compress_on);
        if (parsed_line[0] == "rect"){
            std::string limits = parsed_line[1];
            auto pos = limits.find("x");
            int x = std::stoi(limits.substr(0, pos));
            int y = std::stoi(limits.substr(pos+1));
            for (int j=0; j<y; j++){
                for (int i=0; i<x; i++){
                    main_sc[j][i] = 1;
                    copy_sc[j][i] = 1;
                }
            }
        }
        else {
            int offset = std::stoi(parsed_line[4]);
            std::string index = parsed_line[2];
            if (parsed_line[1] == "column"){
                offset %= h;
                auto pos = index.find("=");
                int col = std::stoi(index.substr(pos+1));
                for (int i=0; i<(h-offset); i++){
                    copy_sc[i+offset][col] = main_sc[i][col];
                }
                int ind = 0;
                for (int i=h-offset; i<h; i++){
                    copy_sc[ind][col] = main_sc[i][col];
                    ind++;
                }
            }
            else{
                offset %= w;
                auto pos = index.find("=");
                int row = std::stoi(index.substr(pos+1));
                for (int i=0; i<(w-offset); i++){
                    copy_sc[row][i+offset] = main_sc[row][i];
                }
                int ind = 0;
                for (int i=w-offset; i<w; i++){
                    copy_sc[row][ind] = main_sc[row][i];
                    ind++;
                }
            }
            //main_sc.swap(copy_sc);
            main_sc = copy_sc;
        }
        print_grid(main_sc, h, w);
    }
    for (int i=0; i<h; i++){
        sum_pixels = std::accumulate(main_sc[i].begin(), main_sc[i].end(), sum_pixels);
    }
    std::cout << "There are " << sum_pixels << " lit pixels" << std::endl;
}
