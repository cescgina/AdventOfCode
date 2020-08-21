#include <set>
#include <fstream>
#include <utility>
#include <iostream>
#include <unistd.h>

int main(int argc, char** argv){
    std::string file_name;
    getopt(argc, argv, "");
    file_name = std::string(argv[optind]);
    std::ifstream input_file;
    std::set<std::pair<unsigned int, unsigned int>> ranges;
    input_file.open(file_name);
    unsigned int left = 0, right = 0;
    for (std::string line; std::getline(input_file, line);){
        auto pos = line.find("-");
        left = std::stoul(line.substr(0, pos));
        right = std::stoul(line.substr(pos+1));
        ranges.insert(std::make_pair(left, right));
    }
    input_file.close();
    unsigned int high = 0, total = 0;
    for (auto it=ranges.begin(); it != ranges.end(); it++){
        if (it->first > high) total += (it->first - high - 1);
        high = std::max(it->second, high);
    }
    std::cout << total + (4294967295-high) << std::endl;
}
