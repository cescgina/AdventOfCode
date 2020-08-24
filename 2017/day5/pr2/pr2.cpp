#include <set>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <unistd.h>

int main(int argc, char** argv){
    std::string file_name;
    getopt(argc, argv, "");
    file_name = std::string(argv[optind]);
    std::ifstream input_file;
    std::vector<int> instructions;
    int steps = 0, i = 0;
    input_file.open(file_name);
    for (std::string line; std::getline(input_file, line);){
        instructions.push_back(std::stoi(line));
    }
    input_file.close();
    int n = instructions.size(), temp;
    while (i < n){
        temp = instructions[i];
        if (temp > 2){
            instructions[i]--;
        }
        else{
            instructions[i]++;
        }
        steps++;
        i += temp;
    }
    std::cout << steps << std::endl;
}
