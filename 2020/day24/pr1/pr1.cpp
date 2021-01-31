#include <map>
#include <string>
#include <fstream>
#include <iostream>
#include <unistd.h>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

bool DEBUG = false;

typedef std::map<std::pair<int,int>, int> hex;

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
    int total = 0;
    for (auto& el: grid){
        total += el.second;
    }
    std::cout << total << std::endl;
}
