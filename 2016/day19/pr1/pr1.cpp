#include <cmath>
#include <fstream>
#include <iostream>
#include <unistd.h>

int main(int argc, char** argv){
    std::string file_name, line;
    getopt(argc, argv, "");
    file_name = std::string(argv[optind]);
    std::ifstream input_file;
    input_file.open(file_name);
    std::getline(input_file, line);
    int elves = std::stoi(line); 
    input_file.close();
    int k = std::log2(elves);
    int ans = 1+(elves-std::exp2(k))*2;
    std::cout << ans << std::endl; 
}
