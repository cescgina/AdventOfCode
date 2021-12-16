#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <unistd.h>

int main(int argc, char** argv){
    std::string file_name, line;
    getopt(argc, argv, "");
    file_name = std::string(argv[optind]);
    std::ifstream input_file(file_name);
    std::getline(input_file, line);
    int numbers = 1;
    std::vector<int> ones(line.size(), 0);
    for (std::string line; std::getline(input_file, line);){
        numbers++;
        for (size_t i = 0; i < line.size(); i++){
            ones[i] += line[i] == '1';
        }
    }
    input_file.close();
    int gamma = 0, epsilon = 0;
    int power = 1;
    for (int i = ones.size()-1; i >= 0; i--){
        if (ones[i] > (numbers-ones[i])){
            gamma += power;
        }
        else{
            epsilon += power;
        }
        power *= 2;
    }
    std::cout << epsilon*gamma << std::endl;
}
