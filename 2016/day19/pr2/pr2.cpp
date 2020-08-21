#include <cmath>
#include <fstream>
#include <iostream>
#include <unistd.h>

int calc_finish(int elves){
    int k = (int)(std::log2(elves)/std::log2(3));
    int b = std::pow(3, k);
    if (elves == b) return elves; 
    else if (elves - b <= b) return elves - b;
    else return (2*elves-3*b);
}

int main(int argc, char** argv){
    std::string file_name, line;
    getopt(argc, argv, "");
    file_name = std::string(argv[optind]);
    std::ifstream input_file;
    input_file.open(file_name);
    std::getline(input_file, line);
    int n_elves = std::stoi(line); 
    input_file.close();
    std::cout << calc_finish(n_elves) << std::endl;
}
