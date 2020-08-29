#include <string>
#include <fstream>
#include <utility>
#include <iostream>
#include <algorithm>
#include <unordered_map>
#include <unistd.h>

bool find_twos(std::pair<char, int> in){
    return in.second == 2;
}

bool find_threes(std::pair<char, int> in){
    return in.second == 3;
}

std::pair<bool, bool> find_repeats(const std::string& s){
    std::unordered_map<char, int> counts;
    for (char c: s){
        counts[c]++;
    }
    return std::make_pair(std::find_if(counts.begin(), counts.end(), find_twos)!=counts.end(), std::find_if(counts.begin(), counts.end(), find_threes)!=counts.end());
}

int main(int argc, char** argv){
    std::string file_name;
    getopt(argc, argv, "");
    file_name = std::string(argv[optind]);
    int twos = 0, threes = 0;
    std::ifstream input_file;
    input_file.open(file_name);
    for (std::string line; std::getline(input_file, line);){
        auto results = find_repeats(line);
        if (results.first) twos++;
        if (results.second) threes++;
    }
    std::cout << twos*threes << std::endl;
}
