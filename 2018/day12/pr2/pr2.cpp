#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <unordered_map>
#include <unistd.h>

int main(int argc, char** argv){
    std::string file_name;
    getopt(argc, argv, "");
    file_name = std::string(argv[optind]);
    std::ifstream input_file;
    input_file.open(file_name);
    std::unordered_map<std::string, char> rules;
    int buffer_size = 100;
    std::string line, dead(buffer_size, '.');
    std::getline(input_file, line);
    std::string initial = line.substr(15);
    std::string buffer = dead+initial+dead;
    std::getline(input_file, line);
    for(std::string line; std::getline(input_file, line);){
        rules[line.substr(0, 5)] = line[line.size()-1];
    }
    std::string prev_state = buffer;
    std::string sample = ".....";
    int sum = 0;
    for (int generation=1; generation <= 200 ; generation++){
        for (int i=2; i<(int)buffer.size()-2; i++){
            sample = prev_state.substr(i-2, 5);
            if (rules.find(sample) != rules.end()){
                buffer[i] = rules[sample];    
            }
            else{
                buffer[i] = '.';
            }
        }
        std::rotate(buffer.begin(), buffer.begin()+1, buffer.end());
        std::swap(buffer, prev_state);
        buffer_size--;
        sum = 0;
        for (int i=0; i<(int)prev_state.size(); i++){
            if (prev_state[i] == '#'){
                sum += (i-buffer_size);
            }
        }
    }
    // pattern converge to a difference of 75 after generation 168
    std::cout << (50000000000-200)*75+sum << std::endl;
}
